//
// Copyright 2018 Horizon Robotics.
//

#ifndef MATRIX_DEV_SDK_EXPORT_FRAME_INTF_H_
#define MATRIX_DEV_SDK_EXPORT_FRAME_INTF_H_

#include <stddef.h>
#include "matrix-dev-sdk/export/MATRIX.h"

#define CAMERA_MODE 0
#define FILLBACK_MODE 1

#define YUV420SP_720P_SIZE (1280 * 720 * 3 >> 1)
#define YUV420SP_1080P_SIZE (1920 * 1080 * 3 >> 1)

class MATRIX_API FrameInfoIntf {
  friend class NetworkReceiverIntf;
  friend class PackLoaderIntf;
 public:
  FrameInfoIntf() {
    meta_ = NULL;
    meta_len_ = 0;
    img_data_ = NULL;
    img_len_ = 0;
    parsing_data_ = NULL;
    parsing_len_ = 0;
    parsing_img_ = NULL;
    parsing_img_len_ = 0;
    lane_parsing_data_ = NULL;
    lane_parsing_len_ = 0;
    lane_parsing_img_ = NULL;
    lane_parsing_img_len_ = 0;
  }

  unsigned char *meta_;
  int meta_len_;
  unsigned char *img_data_;
  int img_len_;
  unsigned char *parsing_data_;
  int parsing_len_;
  unsigned char *parsing_img_;
  int parsing_img_len_;
  unsigned char *lane_parsing_data_;
  int lane_parsing_len_;
  unsigned char *lane_parsing_img_;
  int lane_parsing_img_len_;

 private:
  int format_;
  int buf_id_;
};

#endif  // MATRIX_DEV_SDK_EXPORT_FRAME_INTF_H_
