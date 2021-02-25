#include "capture_picture.h"
using namespace std;
using namespace std::chrono_literals;


static std::atomic<bool> main_thread_run(true);
string watch_file_name = "/tmp/pic/";  //watch directory

 extern "C" {
 #include <libavcodec/avcodec.h>
 #include <libavformat/avformat.h>
 #include <libavutil/avutil.h>
 #include <libavutil/log.h>
 #include <libavdevice/avdevice.h>
 }

extern int64_t CurrentTimeStamp();

int writeJPEG(AVFrame* frame,int width,int height)
{
	static int i = 0;
	char buf[50] = {0};
	sprintf(buf, "/tmp/pic/tmp%d.jpg", i);
	i++;
    const char* out_file = buf;
    //新建一个输出的AVFormatContext 并分配内存
    AVFormatContext* output_cxt = avformat_alloc_context();
    avformat_alloc_output_context2(&output_cxt,NULL,"singlejpeg",out_file);

    //设置输出文件的格式
    output_cxt->oformat = av_guess_format("mjpeg",NULL,NULL);
    //output_cxt->control_message_cb = mem_data;
    //创建和初始化一个和该URL相关的AVIOContext
    if(avio_open(&output_cxt->pb,out_file,AVIO_FLAG_READ_WRITE) < 0)
	{
        av_log(NULL,AV_LOG_ERROR,"不能打开文件  \n");
        return -1;
    }

    //构建新的Stream
    AVStream* stream = avformat_new_stream(output_cxt,NULL);
    if(stream == NULL)
	{
        av_log(NULL,AV_LOG_ERROR,"创建AVStream失败  \n");
        return -1;
    }
    //初始化AVStream信息
    AVCodecContext* codec_cxt = stream->codec;

    codec_cxt->codec_id = output_cxt->oformat->video_codec;
    codec_cxt->codec_type = AVMEDIA_TYPE_VIDEO;
    codec_cxt->pix_fmt = AV_PIX_FMT_YUVJ420P;
    codec_cxt->height = height;
    codec_cxt->width = width;
    codec_cxt->time_base.num = 1;
    codec_cxt->time_base.den = 25;

    //打印输出文件信息
    av_dump_format(output_cxt,0,out_file,1);
    AVCodec* codec = avcodec_find_encoder(codec_cxt->codec_id);
    if(!codec){
        av_log(NULL,AV_LOG_ERROR,"找不到编码器  \n");
        return -1;
    }
    if(avcodec_open2(codec_cxt,codec,NULL) < 0){
        av_log(NULL,AV_LOG_ERROR,"不能打开编码器  \n");
        return -1;
    }
    avcodec_parameters_from_context(stream->codecpar,codec_cxt);

    //写入文件头
    avformat_write_header(output_cxt,NULL);
    int size = codec_cxt->width * codec_cxt->height;

    AVPacket* packet = (AVPacket *) malloc(sizeof(AVPacket)); //分配一个packet;
    int r = av_new_packet(packet,size * 3);
	if(r != 0)
	{
		AVERROR(r);
	}
    int got_picture = 0;
    int result = avcodec_encode_video2(codec_cxt, packet, frame, &got_picture);
    if(result < 0)
	{
        av_log(NULL,AV_LOG_ERROR,"编码失败  \n");
        return -1;
    }
    printf("got_picture %d \n",got_picture);
    if(got_picture == 1)
	{
        //将packet中的数据写入本地文件
        result = av_write_frame(output_cxt,packet);
    }
    av_free_packet(packet);
    //将流尾写入输出媒体文件并释放文件数据
    av_write_trailer(output_cxt);
    if(frame)
	{
        av_frame_unref(frame);
    }
    avio_close(output_cxt->pb);
    avformat_free_context(output_cxt);
    return 0;
}


int test_ffmpeg(string rtsp, int f)
{

    av_register_all();
    avformat_network_init();
    AVDictionary* options = NULL;
    av_dict_set(&options, "buffer_size", "2024000", 0); //设置缓存大小，1080p可将值调大
    av_dict_set(&options, "rtsp_transport", "udp", 0); //以udp方式打开，如果以tcp方式打开将udp替换为tcp
    av_dict_set(&options, "stimeout", "20000000", 0); //设置超时断开连接时间，单位微秒
    av_dict_set(&options, "max_delay", "5000000", 0); //设置最大时延
	//av_dict_set(&options, "fps", "5", 0); //设置帧数

    //如果本地没有视频可以使用这个直播流地址
    char filename[100] = {0};
	memcpy(filename, rtsp.c_str(), rtsp.length());
    AVFormatContext* avf_cxt = avformat_alloc_context();

    int ret = avformat_open_input(&avf_cxt,filename,NULL,&options);
    if(ret < 0){
        av_log(NULL,AV_LOG_ERROR,"不能打开文件\n");
        return -1;
    }

	avf_cxt->probesize = 1000;
	avf_cxt->max_analyze_duration = AV_TIME_BASE;

    ret = avformat_find_stream_info(avf_cxt,NULL);
    if(ret < 0)
	{
        av_log(NULL,AV_LOG_ERROR,"找不到流数据\n");
    	avformat_free_context(avf_cxt);
    	return 0;
    }
    //打印视频信息
    av_dump_format(avf_cxt,0,filename,0);
    int video_index = -1;
    for(int i = 0 ; i < avf_cxt->nb_streams; i++)
	{
        if(avf_cxt->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
            video_index = i;
            break;
        }
    }
    if(video_index == -1)
	{
        av_log(NULL,AV_LOG_ERROR,"没有找到视频流\n");
		avformat_free_context(avf_cxt);

		return 0;
    }
    AVCodecContext* avc_cxt = avf_cxt->streams[video_index]->codec;
    enum AVCodecID codecId = avc_cxt->codec_id;
    AVCodec* codec = avcodec_find_decoder(codecId);
    if(!codec)
	{
        av_log(NULL,AV_LOG_ERROR,"没有找到解码器\n");
		avcodec_close(avc_cxt);
    	avformat_free_context(avf_cxt);
    	return 0;
    }
    ret = avcodec_open2(avc_cxt,codec,NULL);
    if(ret < 0)
	{
        av_log(NULL,AV_LOG_ERROR,"解码器无法打开\n");
		avcodec_close(avc_cxt);
    	avformat_free_context(avf_cxt);
    	return 0;
    }
    //为avpacket分配内存
    AVPacket* packet = av_packet_alloc();
    //为avFrame分配内存
    AVFrame* frame = av_frame_alloc();
	int64_t last_time = CurrentTimeStamp();
	int fps = 1000 / f;
    while(1)
	{
		if(av_read_frame(avf_cxt, packet) >= 0)
		{
			if(packet && packet->stream_index == video_index)
			{
				int gotFrame = 0;
				ret = avcodec_decode_video2(avc_cxt,frame,&gotFrame,packet);
				if(ret < 0)
				{
					printf("\r\nav codec error\r\n");
					AVERROR(ret);
				}
				if(gotFrame)
				{
					int64_t now_time = CurrentTimeStamp();
					if(now_time - last_time > fps)
					{
						printf("gotFrame: %d dt:%ldms  width:%d, height:%d video_index:%d\r\n", gotFrame, now_time - last_time, avc_cxt->width,avc_cxt->height, video_index);
						last_time = now_time;
						ret = writeJPEG(frame,avc_cxt->width,avc_cxt->height);
						if(ret < 0)
						{
							printf("exit av read\r\n");
							break;
						}	
					}					
				}
				this_thread::sleep_for(20ms);
			}
		}
		else
		{
			printf(" read error\r\n");
			break;
		}
    }
	av_frame_free(&frame);
	avcodec_close(avc_cxt);
	avformat_free_context(avf_cxt);
    return 0;
}

static void captureImage(string rtsp, int frame)
{
	/*
    std::ostringstream   ostr; 
	ostr << "ffmpeg -loglevel quiet -i rtsp://admin:Contismartrd@" << ip << ":554/h265/ch1/sub/av_stream -y -f image2 -r " << frame 
		 << "/1 -s 640x480 /tmp/pic/im%d.jpg";
	string cmd = ostr.str();
	int ret = system(cmd.c_str());
	//ffmpeg -i rtsp://admin:Contismartrd@10.8.5.93:554/mpeg/ch1/sub/av_stream -y -f mjpeg -t 0.001 -s 640x480 ./test.jpg
	if (!(WIFEXITED(ret) && !WEXITSTATUS(ret)))
	{
		printf("ffmpeg  execution failed\n"); //error
		main_thread_run = false;
	}
	*/
	test_ffmpeg(rtsp, frame);
}

static void sendHKImage(eCAL::protobuf::CPublisher<CommProto::Image>& HKImagePub, CommProto::Image& image, string sendFileName)
{
	ifstream inFile;
	inFile.open(sendFileName, std::fstream::binary | std::fstream::in);
	if(!inFile.is_open())
	{
		cout << "failed to open " << sendFileName << '\n';
	}
	else
	{
		inFile.seekg(0, inFile.end);
		int64_t file_size = inFile.tellg();
		//cout << "send file length : " << file_size << '\n';
		inFile.seekg(inFile.beg);
		auto image_data = new char[file_size];
		inFile.read(image_data, file_size);
		inFile.close();
		remove(sendFileName.c_str());
		if(file_size > 0)
		{
			image.set_width(640);
			image.set_height(480);
			image.set_time_stamp(chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());
			image.set_format((CommProto::ImageFormat)2);
			image.set_data((const char*)image_data, file_size);	
			HKImagePub.Send(image);
		}
		delete image_data;			
	}
	
}


static void handle_events(int &fd, int &wd, eCAL::protobuf::CPublisher<CommProto::Image>& HKImagePub, CommProto::Image& image)
{
	char buf[4096] __attribute__ ((aligned(__alignof__(struct inotify_event))));
	const struct inotify_event *event;
	int i = 0;
	ssize_t len = 0;
	char *ptr;
	char name_buf[100] = {0};
	for (;;)
	{
		len = read(fd, buf, sizeof(buf));
		if (len == -1 && errno != EAGAIN)
		{
			perror("handle_events inotify read");
			exit(EXIT_FAILURE);
		}

		if (len <= 0)
			break;

		for (ptr = buf; ptr < buf + len; ptr += sizeof(struct inotify_event) + event->len)
		{
			event = (const struct inotify_event *) ptr;																			
			if (wd == event->wd)
			{
				//printf("name:%s time: %ld\n", event->name, chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());
				string file_name = watch_file_name;
				file_name.append(event->name);
				sendHKImage(HKImagePub, image, file_name);
			}
			if (event->mask & IN_IGNORED)
			{
				usleep(100);
				wd = inotify_add_watch(fd, watch_file_name.c_str(), IN_CLOSE_WRITE);
			}
		}
	}
}



void local_init(int& wd_fd, int& notify_fd, string watch_file_name)
{
	if(opendir(watch_file_name.c_str()) == nullptr)
	{
		if(mkdir(watch_file_name.c_str(), S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP|S_IWOTH) == -1)
		{
			printf("failed to create directory\n");
			exit(EXIT_FAILURE);
		}	
	}
	notify_fd = inotify_init1(IN_NONBLOCK);
	wd_fd = inotify_add_watch(notify_fd, watch_file_name.c_str(), IN_CLOSE_WRITE);
    if ((notify_fd == -1) || wd_fd < 0)
	{
        printf("inotify_init1 and watch file failed.\n");
        exit(EXIT_FAILURE);
    }	
}

void captureLoop(std::string rtsp, int frame)
{
	//inotify 
	int poll_num;
	int wd_fd;
	struct pollfd fds;
	errno = 0;	
	int notify_fd;
	local_init(wd_fd, notify_fd, watch_file_name);
	fds.fd = notify_fd;
	fds.events = POLLIN;			
	//
	//ecal	
	eCAL::protobuf::CPublisher<CommProto::Image> HKImagePub;
	HKImagePub.Create("HKImage");
	CommProto::Image image;		
	std::thread CaptureCameraThread(captureImage, rtsp, frame);
	sleep(1);
	while ((eCAL::Ok()) && main_thread_run)
	{
		poll_num = poll(&fds, 1, 2000);
		if (poll_num == -1)
		{
			if (errno == EINTR)
				continue;
			printf("watch file poll");
			exit(EXIT_FAILURE);
		}

		if (poll_num > 0)
		{
			if (fds.revents & POLLIN)
			{
 				handle_events(notify_fd, wd_fd, HKImagePub, image);
			}
		}
	}
	inotify_rm_watch(notify_fd, wd_fd);	
	CaptureCameraThread.join();
	cout << "capture thread off" << '\n';
}