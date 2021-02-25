//
// Copyright 2020 Horizon Robotics.
//
#ifndef MATRIX_DEV_SDK_EXPORT_OBSTACLE_H_
#define MATRIX_DEV_SDK_EXPORT_OBSTACLE_H_

#include <vector>
#include "matrix-dev-sdk/export/MATRIX.h"
namespace ContiSt {
enum MATRIX_API ObstacleType{
  ObstacleType_VehicleRear = 0,
  ObstacleType_VehicleFull = 1,
  ObstacleType_Pedestrian = 2,
  ObstacleType_TrafficSign = 3,
  ObstacleType_TrafficLight = 4,
  ObstacleType_ParkingSlot = 5,
  ObstacleType_RoadSign = 6,
  ObstacleType_CrosswalkLine = 7,
  ObstacleType_TrafficArrow = 8,
  ObstacleType_TrafficCone = 9,
  ObstacleType_Barrel = 10,
  ObstacleType_YieldMark = 11,
  ObstacleType_SpeedMark = 12,
  ObstacleType_Character = 13,
  ObstacleType_StopLine = 14,
  ObstacleType_Diamond = 15,
  ObstacleType_BicycleSign = 16,
  ObstacleType_SpeedBumps = 17,
  ObstacleType_Cyclist = 18,
  ObstacleType_ParkingLock = 19,
  ObstacleType_SpeedCamera = 20,
  ObstacleType_TrafficLightLens = 21
};

struct MATRIX_API Point {
  float x;
  float y;
};

struct MATRIX_API Box3D{
  Point lower_lt;  // lower left top
  Point lower_lb;  // lower left bottom
  Point lower_rb;  // lower right bottom
  Point lower_rt;  // lower right bottom
  Point upper_lt;  // upper left top
  Point upper_lb;  // upper left bottom
  Point upper_rb;  // upper right bottom
  Point upper_rt;  // upper right top
};

struct MATRIX_API Rect {
  float left;
  float top;
  float right;
  float bottom;
};

struct MATRIX_API Obstacle {
  int32_t id;
  int32_t type;
  int32_t confidence;
  int64_t timestamp;
  Rect imgRect;       // image rect area
  Box3D box3d;        // 3D box of the obstacle
  float length;       // size.length
  float width;        // size.width
  float height;       // size.height
  std::vector<int32_t> property;  // e.g. traffic light/sign properties(
                                  // light status/sign types)
  std::vector<std::string> property_name;
  std::vector<int32_t> property_type;
  std::vector<float> property_conf;
  int64_t perception_done_ts; // timestamp while perception done, 0 if not set
};

struct MATRIX_API Image {
  int width;               // image width
  int height;              // image height
  int len;                 // image size after compression
  int64_t timestamp;       // timestamp of the image.
  unsigned char *img_data; // image data
  int32_t format;          // image format, refer "ImageFormat" in protobuf
};

struct MATRIX_API Line{
  int32_t id;
  int32_t life_time;
  std::vector<float> coeffs;
  std::vector<Point> end_points;
  int32_t type;
  float width;
  float conf;
  int64_t perception_done_ts; // timestamp while perception done, 0 if not set
};

struct MATRIX_API ObstacleMsg{
  std::vector<Obstacle> obstacles;
  int64_t perception_done_ts; // timestamp while perception done, 0 if not set
};

struct MATRIX_API LineMsg{
  std::vector<Line> lines;
  int64_t perception_done_ts; // timestamp while perception done, 0 if not set
};

typedef Image ImageMsg;


}  // namespace contiSt
#endif  // MATRIX_DEV_SDK_EXPORT_OBSTACLE_H_
