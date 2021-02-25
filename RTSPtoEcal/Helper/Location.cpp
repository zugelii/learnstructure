#include "Location.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Helper.h"
#include "Log.h"

Location::Location(/* args */)
{
    std::string jsonPath = GetConfigFilePath("SmartRoadEM", "smartroad.json");
	spdlog::info("Use smartroad.json in " + jsonPath);

    std::ifstream jsonStream(jsonPath, std::ios::in);
    std::stringstream buffer;
    buffer << jsonStream.rdbuf();

    auto jsonDocument = std::make_unique<rapidjson::Document>();
    jsonDocument->Parse(buffer.str().c_str());

    rapidjson::Type t = jsonDocument->GetType();
	if (t == rapidjson::kObjectType)
	{
		rapidjson::Value::ConstMemberIterator itr = jsonDocument->FindMember("latitude");
		if (itr != jsonDocument->MemberEnd())
		{
			this->Lat = itr->value.GetDouble();
		}

        itr = jsonDocument->FindMember("longitude");
		if (itr != jsonDocument->MemberEnd())
		{
			this->Lon = itr->value.GetDouble();
		}

        itr = jsonDocument->FindMember("elevation");
		if (itr != jsonDocument->MemberEnd())
		{
			this->Elevation = itr->value.GetDouble();
		}

        itr = jsonDocument->FindMember("installationHeight");
		if (itr != jsonDocument->MemberEnd())
		{
			this->Installation = itr->value.GetFloat();
		}
		// set radar direction 1 for along -1 for reverse
		itr = jsonDocument->FindMember("Direction");
		if (itr != jsonDocument->MemberEnd())
		{
			this->Direction = itr->value.GetInt();
		}

		itr = jsonDocument->FindMember("Congestion");
		if (itr != jsonDocument->MemberEnd())
		{
			this->CongestionSpeed = itr->value.GetFloat();
		}

		itr = jsonDocument->FindMember("speedOverLow");
		if (itr != jsonDocument->MemberEnd())
		{
			this->SpeedOverLow = itr->value.GetFloat();
		}

		itr = jsonDocument->FindMember("speedOverHigh");
		if (itr != jsonDocument->MemberEnd())
		{
			this->SpeedOverHigh = itr->value.GetFloat();
		}

		//lane 1's offset from reference position
		itr = jsonDocument->FindMember("lane1Offset");
		if (itr != jsonDocument->MemberEnd())
		{
			this->Lane1Offset = itr->value.GetFloat();
		}

		//Same as RadarEM's rtx azimuth
		itr = jsonDocument->FindMember("azimuth");
		if (itr != jsonDocument->MemberEnd())
		{
			this->Azimuth = itr->value.GetFloat();
		}
		
		//Tunnel
		itr = jsonDocument->FindMember("TunnelSpeedLimitHigh");
		if (itr != jsonDocument->MemberEnd())
		{
			this->TunnelSpeedLimitHigh = itr->value.GetInt();
		}
		
		itr = jsonDocument->FindMember("TunnelHeightLimitHigh");
		if (itr != jsonDocument->MemberEnd())
		{
			this->TunnelHeightLimitHigh = itr->value.GetInt();
		}		

		//Tunnel
		itr = jsonDocument->FindMember("TunnelSpeedLimitHigh");
		if (itr != jsonDocument->MemberEnd())
		{
			this->TunnelSpeedLimitHigh = itr->value.GetInt();
		}

		itr = jsonDocument->FindMember("TunnelHeightLimitHigh");
		if (itr != jsonDocument->MemberEnd())
		{
			this->TunnelHeightLimitHigh = itr->value.GetInt();
		}

		itr = jsonDocument->FindMember("SpeedLimitSignMax");
		if (itr != jsonDocument->MemberEnd())
		{
			this->SpeedLimitSignMax = itr->value.GetInt();
		}

		itr = jsonDocument->FindMember("SpeedLimitSignMin");
		if (itr != jsonDocument->MemberEnd())
		{
			this->SpeedLimitSignMin = itr->value.GetInt();
		}

		itr = jsonDocument->FindMember("WidthLimitSign");
		if (itr != jsonDocument->MemberEnd())
		{
			this->WidthLimitSign = itr->value.GetInt();
		}

		itr = jsonDocument->FindMember("HeightLimitSign");
		if (itr != jsonDocument->MemberEnd())
		{
			this->HeightLimitSign = itr->value.GetInt();
		}

		//config ramp type
	    itr = jsonDocument->FindMember("RampType");
		if (itr != jsonDocument->MemberEnd())
		{
			this->RampType = itr->value.GetInt();
		}

		//config total lanes
	    itr = jsonDocument->FindMember("TotalLanes");
		if (itr != jsonDocument->MemberEnd())
		{
			this->TotalLanes = itr->value.GetInt();
		}

		//Bridge
		itr = jsonDocument->FindMember("BridgeSpeedLimitHigh");
		if (itr != jsonDocument->MemberEnd())
		{
			this->BridgeSpeedLimitHigh = itr->value.GetInt();
		}
		
		itr = jsonDocument->FindMember("BridgeHeightLimitHigh");
		if (itr != jsonDocument->MemberEnd())
		{
			this->BridgeHeightLimitHigh = itr->value.GetInt();
		}
	}

}

Location::~Location()
{
}
