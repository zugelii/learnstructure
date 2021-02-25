//
// Copyright 2018 Horizon Robotics.
//

#ifndef MATRIX_DEV_SDK_EXPORT_PACK_LOADER_INTF_H_
#define MATRIX_DEV_SDK_EXPORT_PACK_LOADER_INTF_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

#include "matrix-dev-sdk/export/frame_intf.h"
#include "matrix-dev-sdk/export/adas_errno.h"
#include "matrix-dev-sdk/export/MATRIX.h"

/*************************/
/* sensor data structure */
/*************************/
// can raw
#define CAN_MAX_DLEN 8
struct can_frame {
  int can_id;  /* 32 bit CAN_ID + EFF/RTR/ERR flags */
  unsigned char can_dlc;
  /* frame payload length in byte (0 .. CAN_MAX_DLEN) */
  unsigned char data[CAN_MAX_DLEN];
};
struct CANRawData {
  struct can_frame can_frame_;
  int64_t ts_;
};

// imu
struct AccelFrame {
  float acc_x;
  float acc_y;
  float acc_z;
};
struct GyroFrame {
  float gyro_x;
  float gyro_y;
  float gyro_z;
};
struct MagFrame {
  float mag_x;
  float mag_y;
  float mag_z;
};
struct IMUFrame {
  struct AccelFrame acc_frame;
  struct GyroFrame gyro_frame;
  struct MagFrame mag_frame;
  uint8_t frame_valid;
  int64_t timestamp;
  // temperature
  float temp_c;
};

// gnss
struct SensorsDataHeader {
  int64_t sync_ts_;
  char frame_id_[5];
  int64_t seq_;
  int32_t version_;
};
struct GNSSData {
  SensorsDataHeader header_;
  int64_t sensor_ts_;  // gps time
  int64_t sample_ts_;  // os capture time
  // position: x for lat, y for lon, z for alt
  double position_[3];
  float position_std_dev_[3];
  // velocity:
  //   x for east/west direction speed, east positive
  //   y for south/north direction speed, north positive
  //   z for vertical direction speed, up positive
  float velocity_[3];
  float velocity_std_dev_[3];
  // speed: scalar speed
  float speed_;
  // angular: heading direction
  //   x for roll, always 0
  //   y for pitch
  //   z for yaw
  float angular_[3];
  float angular_std_dev_[3];
  char raw_data[300];
};

typedef struct {
  float left;
  float top;
  float right;
  float bottom;
} BoundingBox;

typedef struct {
  BoundingBox bounding_box;
  float conf;
  int32_t model;
} FakeObstacleRaws;

class MATRIX_API PackLoaderIntf {
 public:
  enum {
    INPUT_TYPE_PACK = 0,
    INPUT_TYPE_IMAGE_LIST = 1
  };
  PackLoaderIntf();
  ~PackLoaderIntf();

  /**
   * @brief initialize pack file or image list file's info
   * @return zero for success, non zero for failure
   * @param filename is the input pack file name
   */
  int Init(const char *filename);

  /**
   * @brief get the total number of frames in the pack, takes time to
   *   do the job. NOTE: need to call after Init function
   * @return number of frames in the pack
   */
  int GetTotalFrameCount();

  /**
   * @brief get frame id of the first frame in the pack,
   *   need to call after Init function
   * @return frame id of the first frame
   */
  int GetFirstFrameID();

  /**
   * @brief set the starting frame id, from 0 ~ N
   * @return zero for success, non zero for failure
   * @param frame_id is the start frame id you want to jump
   */
  int SetStartFrameID(int frame_id);

  /**
   * @brief get current frame from the pack file or image list sequentially
   * @return zero for success, or error code see PackLoaderErrorCode
   * @param frame is current frame data from pack file
   */
  int RecvFrame(FrameInfoIntf *frame);

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
   * @brief get the current frame's image file name, only for image list input
   * @return file name or the current frame's image
   */
  std::string GetImageFileName();

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
   * @brief if you want to fillback frame to Matrix, first create connection
   * @return zero for success, non zero for failure
   * @param endpoint is Matrix's IP address
   */
  int InitConnectionToMatrix(const char *end_point);

  /**
   * @brief set the Matrix work mode, input from camera or network fillback
   * @return zero for success, non zero for failure
   * @param endpoint is Matrix's IP address
   * @param mode can be CAMERA_MODE or FILLBACK_MODE
   */
  int SetMatrixWorkMode(const char *end_point, int mode);

  /**
   * @brief for fillback mode, send pack frame to Matrix
   * @return zero for success, non zero for failure
   * @param frame is the frame data from pack file
   */
  int SendFrame(FrameInfoIntf *frame);

  /**
   * @brief the following functions are for image list fillback
   * @return zero for success, non zero for failure
   * @param data serialized into proto sending to Matrix
   */
  int AddObstacleRaws(const std::vector<FakeObstacleRaws> &fake_obs_raw);
  int AddCANRaws(const std::vector<CANRawData> &can_raw_data);
  int AddIMU(const std::vector<IMUFrame> &imu_data);
  int AddGPS(const GNSSData &gnss_data);

  /**
   * @brief for the image list input, you must add image timestamp
   * @return zero for success, non zero for failure
   * @param frame is the frame data from image list
   * @param timestamp -1 for using os time, or pass your own timestamp.
   */
  int SendFrame(FrameInfoIntf *frame,
    int64_t timestamp);

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

  /* cameras number of the pack file */
  int camera_cnt_;

  /* input type, pack or image list */
  int input_type_;

 protected:
  void *pack_loader_impl_;
};

#endif  // MATRIX_DEV_SDK_EXPORT_PACK_LOADER_INTF_H_
