#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>

#include <iostream>



using namespace std;
using namespace cv;

//rtsp://admin:Contismartrd@192.168.1.64:554/264/ch1/main/av_stream
//rtsp://admin:Contismartrd@192.168.1.64:554/mpeg/ch1/sub/av_stream
int main()
{

	VideoCapture cap;
 
	//namedWindow("Camera", CV_WINDOW_AUTOSIZE);	
	//主码流
	//cap.open("rtsp://admin:a12345678@192.168.1.108:554/cam/realmonitor?channel=1&subtype=0");
 
	//辅码流
	cap.open("rtsp://admin:Contismartrd@192.168.1.64:554/mpeg/ch1/sub/av_stream");
	if (!cap.isOpened())
	{
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
	/*	Mat Camera_CImg;
	Mat Camera_GImg;
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	//cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	if (!cap.isOpened())
		return -1;
 
	while (1)
	{
		cap >> Camera_CImg;
		if (Camera_CImg.empty())
			break;
		//cvtColor(Camera_CImg, Camera_GImg, CV_RGB2GRAY);
		imshow("Camera", Camera_CImg);
		//if (cvWaitKey(10) == 'q')
		//	break;
		sleep(1);
	}
	*/
	Mat frame;
	while (1)
	{
		// wait for a new frame from camera and store it into 'frame'
		cap.read(frame);
		// check if we succeeded
		if (frame.empty()) {
			cerr << "ERROR! blank frame grabbed\n";
			break;
		}
		// show live and wait for a key with timeout long enough to show images
		imshow("Live", frame);
		if (waitKey(5) >= 0)
			break;

	}
		

	return 0;
}