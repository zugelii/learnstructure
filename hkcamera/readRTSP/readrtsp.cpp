#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>

#include <iostream>
#include <stdio.h>

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <sys/inotify.h>
#include <errno.h>
#include <chrono>
using namespace std::chrono_literals;


using namespace std;

static void handle_events(int &fd, int & wd, string wd_file)
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
			if (event->mask & IN_OPEN)
				printf("IN_OPEN: \r\n");
			if (event->mask & IN_CLOSE_NOWRITE)
				printf("IN_CLOSE_NOWRITE: \r\n");
			if (event->mask & IN_CLOSE_WRITE)
				printf("IN_CLOSE_WRITE: \r\n");
			if (event->mask & IN_MODIFY)
				printf("IN_MODIFY: \r\n");
			if (event->mask & IN_IGNORED)
				printf("IN_IGNORED:\r\n");
			if (event->mask &(IN_MOVE))
				printf("IN_MOVE:\r\n");
			if (event->mask &(IN_ACCESS))
				printf("IN_ACCESS:\r\n");	
			if (event->mask &(IN_ATTRIB))
				printf("IN_ATTRIB:\r\n");
			if (event->mask &(IN_DELETE))
				printf("IN_DELETE:\r\n");
			if (event->mask &(IN_DELETE_SELF))
				printf("IN_DELETE_SELF:\r\n");
			if (event->mask &(IN_CREATE))
			{
				printf("IN_CREATE:\r\n");				
			}
																						

			if (wd == event->wd)
			{
				
				printf("name:%s time: %ld\n", event->name, chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());
				//cout << "time : " << chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count() << '\n';
			}
			if (event->mask & IN_IGNORED)
			{
				usleep(100);
				wd = inotify_add_watch(fd, wd_file.c_str(), IN_CLOSE_NOWRITE);
			}
		}
	}
}
//rtsp://admin:Contismartrd@192.168.1.64:554/264/ch1/main/av_stream
//rtsp://admin:Contismartrd@192.168.1.64:554/mpeg/ch1/sub/av_stream
int main()
{ 
	//namedWindow("Camera", CV_WINDOW_AUTOSIZE);	
	//主码流
	//cap.open("rtsp://admin:a12345678@192.168.1.108:554/cam/realmonitor?channel=1&subtype=0");
 	//ffmpeg -i rtsp://admin:Contismartrd@192.168.1.64:554/mpeg/ch1/sub/av_stream -y -f mjpeg -t 0.001 -s 640x480 test.jpg
	//辅码流
	//cap.open("rtsp://admin:Contismartrd@192.168.1.64:554/mpeg/ch1/sub/av_stream", CAP_GSTREAMER);
	int poll_num;
	int wd_fd;
	struct pollfd fds;
	errno = 0;	
	static int notify_fd;
	string watch_file_name = "/tmp/pic/";  //watch directory

	notify_fd = inotify_init1(IN_NONBLOCK);
	wd_fd = inotify_add_watch(notify_fd, watch_file_name.c_str(), IN_CLOSE_WRITE);
    if ((notify_fd == -1) || wd_fd < 0)
	{
        printf("inotify_init1");
        exit(EXIT_FAILURE);
    }
	fds.fd = notify_fd;
	fds.events = POLLIN;
	do
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
 				handle_events(notify_fd, wd_fd, watch_file_name);
			}
		}
		//check_wd_file(notify_fd, wd);
	}while(1);
	inotify_rm_watch(notify_fd, wd_fd);	
		

	return 0;
}