//
// Copyright 2018 Horizon Robotics.
//

#ifndef MATRIX_DEV_SDK_EXPORT_MATRIX_ERRNO_H_
#define MATRIX_DEV_SDK_EXPORT_MATRIX_ERRNO_H_

enum MATRIX_API PackLoaderErrorCode {
  /* load current frame for pack file successfully */
  PACK_LOAD_OK = 0,

  /* image list has been seeked to the end */
  IMAGE_LIST_END = -1,

  /* pack file has been seeked to the end*/
  PACK_FILE_END = -1,

  /* protobuf data length is illegal, no bigger than zero */
  ILLEGAL_META_LEN = -2,

  /* image length is illegal, no bigger than zero */
  ILLEGAL_IMG_LEN = -3,

  /* seek out of file length */
  INVALID_FILE_END = -4,

  /* protobuf parse api return error */
  PARSE_PROTOBUF_FAILED = -5,

  /* compatible with no format */
  NO_FRAME_COUNT = -6,

  /* turbo jpeg api decode error */
  JPEG_DECODE_ERROR = -7,

  /* decoded image size not match with meta */
  JPEG_SIZE_ERROR = -8,

  /* deprecated */
  IMAGE_LEN_ERROR = -9,
  PARSING_LEN_ERROR = -10,

  /* open jpeg file from image list failed */
  IMAGE_OPEN_ERROR = -11
};

enum MATRIX_API NetworkReceiverErrorCode {
  /* receive frame from matrix successfully */
  ZMQ_RECV_OK = 0,

  /* zmq receive api return error */
  ZMQ_MSG_RECV_ERROR = -1,

  /* zmq receive incomplete */
  ZMQ_RECV_INCOMPLETE = -2,

  /* turbo jpeg api decode error */
  RECV_JPEG_DECODE_ERROR = -3,

  /* proto version not supported */
  FORMAT_NOT_SUPPORT = -4,
};

#endif  // MATRIX_DEV_SDK_EXPORT_MATRIX_ERRNO_H_

