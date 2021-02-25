#include "MqttClient.h"

#include <Log.h>

static void OnConnect(mosquitto *mqtt, void *obj, int rc);
static void OnDisConnect(mosquitto *mqtt, void *obj, int result);
static void OnPublish(mosquitto *mqtt, void *obj, int mid);
static void OnSubscribe(mosquitto *mqtt, void *obj, const mosquitto_message *msg);

MqttClient::MqttClient()
{
   mqtt = mosquitto_new("matrixjpeg",true,NULL);

    mosquitto_connect_callback_set(mqtt, OnConnect);
    mosquitto_disconnect_callback_set(mqtt, OnDisConnect);
    mosquitto_publish_callback_set(mqtt, OnPublish);
    mosquitto_message_callback_set(mqtt, OnSubscribe);

    int err = mosquitto_connect(mqtt,"localhost",1883,60);
    if(err == MOSQ_ERR_SUCCESS){
        spdlog::error("mosquitto connect successful");
    }

    // mosquitto will start a new thread
    err = mosquitto_loop_start(mqtt);
    if(err == MOSQ_ERR_SUCCESS){
        spdlog::error("mosquitto loop start successful");
    }
}

MqttClient::~MqttClient()
{
    if(mqtt != nullptr){
        mosquitto_loop_stop(mqtt,true);
        mosquitto_destroy(mqtt);
    }
}

void MqttClient::Publish(string topic, int payloadlen, const void *payload)
{
	int ret = mosquitto_publish(mqtt, NULL, topic.c_str(), payloadlen, payload, 0, false);
    if(ret != MOSQ_ERR_SUCCESS)
    {
        spdlog::error("mosquitto_publish error: " + to_string(ret));
    }
}

void OnConnect(mosquitto *mqtt, void *obj, int rc)
{
    spdlog::info("mosquitto is connected");
}

void OnDisConnect(mosquitto *mqtt, void *obj, int result)
{
    spdlog::warn("mosquitto is disconnected");
}

void OnPublish(mosquitto *mqtt, void *obj, int mid)
{
    spdlog::info("mosquitto publish ok");
}

void OnSubscribe(mosquitto *mqtt, void *obj, const struct mosquitto_message *msg)
{
    spdlog::info("mosquitto subscribe ok");
}
