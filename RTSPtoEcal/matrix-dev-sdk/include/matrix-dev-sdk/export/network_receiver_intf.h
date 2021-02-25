//
// Copyright 2018 Horizon Robotics.
//

#ifndef MATRIX_DEV_SDK_EXPORT_NETWORK_RECEIVER_INTF_H_
#define MATRIX_DEV_SDK_EXPORT_NETWORK_RECEIVER_INTF_H_

#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <functional>
#include <opencv2/opencv.hpp>

#include "matrix-dev-sdk/export/frame_intf.h"
#include "matrix-dev-sdk/export/adas_errno.h"
#include "matrix-dev-sdk/export/MATRIX.h"
#include "matrix-dev-sdk/export/Obstacle.h"

class MATRIX_API NetworkReceiverIntf {
 public:
  NetworkReceiverIntf();
  ~NetworkReceiverIntf();

  /**
   * @brief initialize connection to the Matrix
   * @return zero for success, non zero for failure
   * @param endpoint is Matrix's IP address
   */
  int Init(const char *endpoint);

  /**
   * @brief set the Matrix work mode, input from camera or network fillback
   * @return zero for success, non zero for failure
   * @param endpoint is Matrix's IP address
   * @param mode can be CAMERA_MODE or FILLBACK_MODE
   */
  int SetMatrixWorkMode(const char *end_point, int mode);

  /**
   * @brief Reconnect to Matrix
   */
  void Reconnect();

  /**
   * @brief sync receive frame from Matrix, block the thread if connection lost
   * @return zero for success, or error code see NetworkReceiverErrorCode
   * @param frame is every frame data from Matrix
   */
  int RecvFrame(FrameInfoIntf *frame);
  /**
   * @brief corresponding to the api above, stop the blocked thread.
   */
  void StopRecvFrame();

  /**
   * @brief async receive frame from Matrix, return error code if failed
   * @return same with RecvFrame return value
   * @param frame is every frame data from Matrix
   */
  int RecvFrameAsync(FrameInfoIntf *frame);

  /**
   * @brief get the camera_id'th image from the input frame
   * @return zero for success, non zero for failure
   * @param camera_id is camera id, from 0 to camera_count - 1
   * @param output stores the YUV420SP image from the current frame
   * @param frame is the current frame data from Matrix
   */
  int GetImage(int camera_id, std::vector<uint8_t> &output,
    const FrameInfoIntf *frame);

  /**
   * @brief get image size, call after RecvFrame
   * @return image width/height
   */
  int GetImageWidth();
  int GetImageHeight();

  /**
   * @brief get the camera_id'th undistort image from the input frame
   * @return zero for success, non zero for failure
   * @param camera_id is camera id, from 0 to camera_count - 1
   * @param output stores the undistorted YUV420SP image from the current frame
   * @param frame is the current frame data from Matrix
   */
  int GetUndistortImage(int camera_id, std::vector<uint8_t> &output,
    const FrameInfoIntf *frame);

  /**
   * @brief get the parsing label(cv::Mat type) from the current frame
   * @return label mat from current frame
   * @param camera_id is camera id, from 0 to camera_count - 1
   * @param frame is the current frame data from Matrix
   */
  cv::Mat GetParsingLabel(int camera_id, const FrameInfoIntf *frame);

  /**
   * @brief get the lane parsing label(cv::Mat type) from the current frame
   * @return label mat from current frame
   * @param camera_id is camera id, from 0 to camera_count - 1
   * @param frame is the current frame data from Matrix
   */
  cv::Mat GetLaneParsingLabel(int camera_id, const FrameInfoIntf *frame);

  /**
   * @brief if want to store frame received into a pack file, initialize first
   * @return zero for success, non zero for failure
   * @param filename is output pack file name
   */
  int PackFileInit(const char *filename);

  /**
   * @brief write the input frame into pack file
   * @return zero for success, non zero for failure
   * @param frame is the current frame data
   */
  int WriteFrameToPackFile(const FrameInfoIntf *frame);

  /**
   * @brief if want to receive log from Matrix, run the initialization first
   * @return zero for success, non zero for failure
   */
  int UDPLogReceiverInit();

  /**
   * @brief you can receive multiple Matrix log
   * @return zero for success, non zero for failure
   * @param target_server is the IP address of Matrix
   */
  int UDPLogAddTargetServer(const std::string &target_server);

  /**
   * @brief receive one piece of log and copy into string
   * @return zero for success, non zero for failure
   * @param log is content of log received from Matrix
   */
  int UDPLogRecv(std::string &log);

  /**
  * @brief register obstacle callback, call callback obstacle data is received
  * @return zero for success, non zero for failure
  * @param func is callback function
  */
  int RegisterObstacleCallback(std::function<void(ContiSt::ObstacleMsg*)> func);

  /**
  * @brief register Image callback, get image data
  * @return zero for success, non zero for failure
  * @param func is callback function
  */
  int RegisterImageCallback(std::function<void(ContiSt::ImageMsg*)> func);

  /**
  * @brief register UDP log callback, get image data
  * @return zero for success, non zero for failure
  * @param func is callback function
  */
  int RegisterUdpLogCallback(std::function<void(std::string)> func);

  /**
  * @brief register lane callback, get lane data
  * @return zero for success, non zero for failure
  * @param func is callback function
  */
  int RegisterLaneCallback(std::function<void(ContiSt::LineMsg*)> func);

  /**
  * @brief start to receive the net real-time data from matrix
  * @return none
  * @param matrix_ip is the IP address of matrix device
  */
  void RunNetworkRecv(const char * matrix_ip);

  /* cameras number of the Matrix */
  int camera_cnt_;

 protected:
  void *network_receiver_impl_;

 private:
  std::ofstream ofs;
  bool stop_receive_;
};

#endif  // MATRIX_DEV_SDK_EXPORT_NETWORK_RECEIVER_INTF_H_
