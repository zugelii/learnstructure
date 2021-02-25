//============================================================================
// Name        : cv2ecal.cpp
// Author      : yulu.li
// Version     : 0.1.0
// Copyright   : Your copyright notice
// Description : cv2ecal in C++, Ansi-style
//============================================================================
#include <iostream>
#include <thread>
#include <chrono>

#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <thread>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#include <ecal/ecal.h>
#include <ecal/msg/protobuf/publisher.h>
#include <csignal>
#include <atomic>
#if defined(WIN32) || defined(_WIN32)
	#include <conio.h>	//for _kbhit
#else
	#include <sys/prctl.h>
#endif
#ifdef OPEN_MATRIX
#include "matrix-dev-sdk/export/network_receiver_intf.h"
#endif
#include "Log.h"

#include "rsm.pb.h"
#include "ecalpub.h"
#include "matrixFilter.h"
#include "capture_picture.h"
#include "MqttClient.h"
#include "Pedestrain.h"

using namespace std;
using namespace std::chrono_literals;
eCalPub eCalPublisher;
#ifdef OPEN_MATRIX
extern CommProto::Obstacle& Convert(CommProto::Obstacle& ob, const ContiSt::Obstacle& stob);
extern CommProto::Line& Convert(CommProto::Line& line, const ContiSt::Line& lane);
extern CommProto::Image& Convert(CommProto::Image& image, const ContiSt::ImageMsg& img);
#endif
int64_t CurrentTimeStamp()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

int main(int argc, char**argv)
{
#if defined(WIN32) || defined(_WIN32)
#else
	prctl(PR_SET_NAME, "rtsptoecal");
#endif
	Log::Init("rtsptoecal");

	MatrixFilter filter;
	auto matrixJson = filter.OpenJson();

	eCAL::Initialize(0, nullptr, "rtsptoecal provider init");
#ifdef NO_TYPE_DESCRIPTION
	eCAL::Util::PubShareDescription(false);
	eCAL::Util::PubShareType(false);
#endif

	//default to 0, due to CPU load
	if(matrixJson->CaptureFlag > 0) //check if capture image
	{
		std::thread captureThread(captureLoop, matrixJson->Rtsp, matrixJson->CaptureFrame);
		sleep(1);
		captureThread.detach();
	}
	while(1)	
	{
		sleep(10);
	}
	
#if ON_MATRIX
	eCalPublisher.Init();

	CommProto::Obstacles obs;
	CommProto::Lines lines;
	CommProto::Image image;
	MqttClient mqtt;

	NetworkReceiverIntf network_receiver;
	std::function<void(ContiSt::ObstacleMsg*)> obsFunc = [&obs](ContiSt::ObstacleMsg* obsmsg)
	{
		obs.set_received_done_ts(CurrentTimeStamp());
		obs.set_perception_done_ts(obsmsg->perception_done_ts);
		for(auto obmsg : obsmsg->obstacles)
		{
			//add pedestrain only if he is on the road
			if(obmsg.type == ContiSt::ObstacleType_Pedestrian)
			{
				Pedestrain p(obmsg.imgRect);
				if(!p.IsOnRoad())
				{
					std::cout << "pedestrian outside of road" << std::endl;
					continue;	//filter out this pedestrain
				}
				else
				{
					std::cout << "pedestrian on road" << std::endl;
				}
			}
			auto obstacle = obs.add_obstacle();
			Convert(*obstacle, obmsg);
		}

		eCalPublisher.Publish(obs);
		obs.clear_obstacle();
	};
	network_receiver.RegisterObstacleCallback(obsFunc);

	std::function<void(ContiSt::LineMsg*)> laneFunc = [&lines](ContiSt::LineMsg* lanes)
	{
		lines.set_received_done_ts(CurrentTimeStamp());
		lines.set_perception_done_ts(lanes->perception_done_ts);

		for (auto lane : lanes->lines)
		{
			auto line = lines.add_lines();
			Convert(*line, lane);
		}
		//std::cout << "Lane perception_done_ts:" << lanes->perception_done_ts << std::endl;

		eCalPublisher.Publish(lines);
		lines.clear_lines();
	};
	network_receiver.RegisterLaneCallback(laneFunc);

	//if ImageInterval == 0, don't publish matrix image
	//default 15: 15*33ms = 500ms
	if (matrixJson->ImageInterval > 0)
	{
		std::function<void(ContiSt::ImageMsg*)> imgFunc = [&image, &matrixJson, &mqtt](ContiSt::ImageMsg* img)
		{
			static int imageCount = 0;
			imageCount ++;

			//if ImageInterval == 1, publish all image, if ImageInterval == 10, publish once every 10 images
			if(imageCount >= matrixJson->ImageInterval || matrixJson->ImageInterval == 8888)
			{
				imageCount = 0;

				image.set_received_done_ts(CurrentTimeStamp());
				Convert(image, *img);
				eCalPublisher.Publish(image);

				mqtt.Publish("matrix/jpg", img->len, img->img_data);
			}
		};
		network_receiver.RegisterImageCallback(imgFunc);

		//magic number to simulate and send file to mqtt
		//it's ok for the following while() to block the whole thread
		if (matrixJson->ImageInterval = 8888)
		{
			ContiSt::ImageMsg img ={0};

			while(true)
			{
				//files like "/data/tf1.jpt"
				std::string path = "/data";
				for (const auto & entry : fs::directory_iterator(path))
				{
					if (entry.path().extension().string() == ".jpg")
					{
						std::cout << entry.path() << std::endl;
						std::ifstream ifs (entry.path(), std::ifstream::binary);

						// get pointer to associated buffer object
						std::filebuf* pbuf = ifs.rdbuf();

						// get file size using buffer's members
						img.len = pbuf->pubseekoff (0,ifs.end,ifs.in);
						pbuf->pubseekpos (0,ifs.in);

						// allocate memory to contain file data
						img.img_data = new unsigned char[img.len];

						// get file data
						pbuf->sgetn ((char*)img.img_data, img.len);
						ifs.close();

						imgFunc(&img);
						delete[] img.img_data;

						this_thread::sleep_for(500ms);
					}
				}
			}
		}
	}

	const char *matrix_ip = "192.168.1.10";
	network_receiver.RunNetworkRecv(matrix_ip);	//!! this will block the thread by libmatrix-dev-sdk.so
#endif
	while (1)
	{
		this_thread::sleep_for(5000ms);
	}
	
	eCAL::Finalize();
}

