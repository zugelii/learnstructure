/*
 * @Description: 
 * @Author: 
 * @Github: 
 * @Date: 2021-02-25 11:36:10
 * @LastEditors: yululi
 * @LastEditTime: 2021-02-25 15:22:26
 * @FilePath: /RTSPtoEcal/matrixFilter.cpp
 */
#include "matrixFilter.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Log.h"
#include "Helper.h"

std::shared_ptr<MatrixJson> MatrixFilter::OpenJson()
{
	std::string jsonPath = GetConfigFilePath("rtsptoecal", "matrixFilter.json");
	spdlog::info("Use matrixFilter.json in " + jsonPath);

	//local config file has higher priority
	std::ifstream inLocal(jsonPath, ios::in);
	std::stringstream buffer;
	buffer << inLocal.rdbuf();

	jsonDocument = make_unique<rapidjson::Document>();
	jsonDocument->Parse(buffer.str().c_str());	//c_str() can't be saved


	auto matrixJson = std::make_shared<MatrixJson>();
	matrixJson->LoadJson(this);
	return matrixJson;
}

uint16_t MatrixFilter::GetIntConfig(std::string config)
{
	uint16_t value = 0;
	rapidjson::Type t = jsonDocument->GetType();
	if (t == rapidjson::kObjectType)
	{
		rapidjson::Value::ConstMemberIterator itr = jsonDocument->FindMember(config.c_str());
		if (itr != jsonDocument->MemberEnd())
		{
			value = itr->value.GetInt();
		}
	}
	return value;
};

std::string MatrixFilter::GetStringConfig(std::string config)
{
	string value;
	rapidjson::Type t = jsonDocument->GetType();
	if (t == rapidjson::kObjectType)
	{
		rapidjson::Value::ConstMemberIterator itr = jsonDocument->FindMember(config.c_str());
		if (itr != jsonDocument->MemberEnd())
		{
			value = itr->value.GetString();
		}
	}
	return value;
};

uint16_t MatrixFilter::GetImageInterval()
{
	return this->GetIntConfig("imageInterval");
};

uint16_t MatrixFilter::GetCaptureFlag()
{
	return this->GetIntConfig("captureFlag");
}
std::string   MatrixFilter::GetRtsp()
{
	return this->GetStringConfig("rtsp");
}
uint16_t MatrixFilter::GetCaptureFrame()
{
	return this->GetIntConfig("caputreFrame");
}
