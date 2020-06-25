#pragma once
#include <errno.h>
#include <getopt.h>
#include <libgen.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>

#include <net/if.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/error.h>
#include <poll.h>
#include <thread>
#include <string>
#include <iostream>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <map>
#include <chrono>
#include "radar_data.h"
#include "ecal_radar.h"

#include <ecal/ecal.h>
#include <ecal/msg/protobuf/publisher.h>
#include "ObjectLists/RadarObject.pb.h"
#include "ObjectLists/RDISRR320SC.pb.h"
class CSocketCan
{
private:
    struct can_frame frame;
	struct ifreq ifr;
	struct sockaddr_can addr;
    int can_fs;
    struct pollfd poll_fds;
    std:: thread threadCan;
    std::atomic<bool> m_thread_run;
    bool start_flag;
    std::map<int32_t, CAN_OBJECTS> m_radar_data;
    int32_t meas_counter;
    uint8_t object_number;
    eCAL::protobuf::CPublisher<pb::ObjectLists::RadarObject> radarObjectPublisher;
    pb::ObjectLists::RadarObject RadarObject;
    void ProcessCan(RADAR_READ_DATA &rd);
    void PublishCanData();
    float ValueForSignal(uint8_t value);
    int64_t SetHeaderTime();
    
protected:
    void CanThread(std::string &nameCan);
    
public:
    CSocketCan(std::string name_can);
    bool wirteCan(char *buf, int32_t len);
    int32_t readCan();
    void testCan(struct can_frame &frame);
    ~CSocketCan();
    
};



