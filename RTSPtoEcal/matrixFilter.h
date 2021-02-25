/*
 * @Description: 
 * @Author: 
 * @Github: 
 * @Date: 2021-02-25 11:36:10
 * @LastEditors: yululi
 * @LastEditTime: 2021-02-25 15:22:40
 * @FilePath: /RTSPtoEcal/matrixFilter.h
 */
#pragma once

#include <memory>
#include <tuple>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "MatrixJson.h"

using namespace std;

class MatrixFilter
{
private:
	unique_ptr<rapidjson::Document> jsonDocument;
public:
	std::shared_ptr<MatrixJson> OpenJson();
public:
    uint16_t GetImageInterval();

	uint16_t GetCaptureFlag();
	std::string   GetRtsp();
	uint16_t   GetCaptureFrame();
protected:	
	uint16_t GetIntConfig(std::string config);
	std::string GetStringConfig(std::string config);
};
