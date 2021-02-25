#include <iostream>
#include <matrix-dev-sdk/export/network_receiver_intf.h>
#include <time.h>
#include <chrono>
#include <functional>

static int64_t getCurrentTimeStamp(){
    std::chrono::system_clock::time_point now =
      std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(
      now.time_since_epoch()).count();
}

int main(){
  /* set Matrix's ip address. */
  /* set Matrix's ip address. */
  const char *matrix_ip = "192.168.1.10";
  NetworkReceiverIntf *network_receiver = new NetworkReceiverIntf();
  std::function<void(std::string)> updLogFunc_ =
    [](std::string log) {
    std::cout << "updLogFunc_ called at local time:"
      << getCurrentTimeStamp() << std::endl;
    std::cout << "======start udp log======" << std::endl;
    std::cout << log << std::endl;
    std::cout << "======end udp log======" << std::endl;
  };

  std::function<void(ContiSt::ObstacleMsg*)> obsFunc_ =
    [](ContiSt::ObstacleMsg* obs) {
      std::cout << "obsFunc_ called at local time:"
        << getCurrentTimeStamp() << std::endl;
      for (auto o : obs->obstacles) {

        std::cout << "obs.timestamp:" << o.timestamp
          << ",obs.id:" << o.id
          << ",obs.type:" << o.type
          << ",obs.confidence:" << o.confidence
          << ",obs.length:" << o.length
          << ",obs.width:" << o.width
          << ",obs.height:" << o.height << std::endl
          << "imgRect:" << std::endl
          << "  obs.imgRect.left:" << o.imgRect.left << std::endl
          << "  obs.imgRect.top:" << o.imgRect.top << std::endl
          << "  obs.imgRect.right:" << o.imgRect.right << std::endl
          << "  obs.imgRect.bottom:" << o.imgRect.bottom << std::endl
          << "box3d:" << std::endl
          << "  obs.box3d.lower_lt:" << o.box3d.lower_lt.x << std::endl
          << "  obs.box3d.lower_lt:" << o.box3d.lower_lt.y << std::endl
          << "  obs.box3d.lower_lb:" << o.box3d.lower_lb.x << std::endl
          << "  obs.box3d.lower_lb:" << o.box3d.lower_lb.y << std::endl
          << "  obs.box3d.lower_rb:" << o.box3d.lower_rb.x << std::endl
          << "  obs.box3d.lower_rb:" << o.box3d.lower_rb.y << std::endl
          << "  obs.box3d.lower_rt:" << o.box3d.lower_rt.x << std::endl
          << "  obs.box3d.lower_rt:" << o.box3d.lower_rt.y << std::endl
          << "  obs.box3d.upper_lt:" << o.box3d.upper_lt.x << std::endl
          << "  obs.box3d.upper_lt:" << o.box3d.upper_lt.y << std::endl
          << "  obs.box3d.upper_lb:" << o.box3d.upper_lb.x << std::endl
          << "  obs.box3d.upper_lb:" << o.box3d.upper_lb.y << std::endl
          << "  obs.box3d.upper_rb:" << o.box3d.upper_rb.x << std::endl
          << "  obs.box3d.upper_rb:" << o.box3d.upper_rb.y << std::endl
          << "  obs.box3d.upper_rt:" << o.box3d.upper_rt.x << std::endl
          << "  obs.box3d.upper_rt:" << o.box3d.upper_rt.y
          << std::endl;
      }
      std::cout << "Obstacle perception_done_ts:" << obs->perception_done_ts
        << std::endl;
  };

  std::function<void(ContiSt::LineMsg*)> laneFunc_ =
    [](ContiSt::LineMsg* lanes){
      std::cout << "laneFunc_ called at local time:"
        << getCurrentTimeStamp() << std::endl;

      for (auto lane : lanes->lines){
        std::cout << " lane.id:" << lane.id
          << ", lane.type:" << lane.type
          << ", lane.life_time:" << lane.life_time
          << ", lane.width:" << lane.width
          << ", lane.conf:" << lane.conf << std::endl;
          for (auto point : lane.end_points){
            std::cout << " point.x= " << point.x
              << ", point.y=" << point.y << std::endl;
          }
          for (auto point : lane.end_points){
            std::cout << " point.x= " << point.x
              << ", point.y=" << point.y << std::endl;
          }
          for (auto cof : lane.coeffs){
            std::cout << " coffe:" << cof << std::endl;
          }
      }
      std::cout << "Lane perception_done_ts:" << lanes->perception_done_ts
        << std::endl;
  };

  std::function<void(ContiSt::ImageMsg*)> imgFunc_ =
    [](ContiSt::ImageMsg* img) {
    std::cout << "imgFunc_ called at local time:"
      << getCurrentTimeStamp() << std::endl;
    std::cout << "image height:" << img->height
      << ", image width:" << img->width
      << ", image timestamp:" << img->timestamp
      << std::endl;

#if 0 // save jpeg test
    std::string fName = std::to_string(img->timestamp) + ".jpeg";
    FILE *fjpeg_test = fopen(fName.c_str(), "wb");
    fwrite(img->img_data, 1, img->len, fjpeg_test);
    fclose(fjpeg_test);
#endif
  };

  network_receiver->RegisterObstacleCallback(obsFunc_);
  //network_receiver->RegisterUdpLogCallback(updLogFunc_);
  network_receiver->RegisterImageCallback(imgFunc_);
  network_receiver->RegisterLaneCallback(laneFunc_);
  network_receiver->RunNetworkRecv(matrix_ip);
  return 1;
}
