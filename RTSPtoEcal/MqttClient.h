#pragma once

#include <string>
#include "mosquitto.h"

using namespace std;

class MqttClient
{
protected:
    mosquitto *mqtt = nullptr;
public:
    MqttClient();
    ~MqttClient();
public:
    void Publish(string topic, int payloadlen, const void *payload);
};
