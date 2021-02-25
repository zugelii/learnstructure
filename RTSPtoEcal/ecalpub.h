#pragma once

#include <ecal/ecal.h>
#include <ecal/msg/protobuf/publisher.h>

#include "comm.pb.h"


class eCalPub
{
public:
    void Init();
    void Publish(const CommProto::Obstacles &obstacles);
    void Publish(const CommProto::Image &image);
    void Publish(const CommProto::Lines &line);
protected:
    eCAL::protobuf::CPublisher<CommProto::Obstacles> ObstaclesPub;
    eCAL::protobuf::CPublisher<CommProto::Image> ImagePub;
    eCAL::protobuf::CPublisher<CommProto::Lines> LinePub;
};
