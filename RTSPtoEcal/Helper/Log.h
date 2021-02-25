#pragma once

#include <string>

#include <spdlog/spdlog.h>

class Log
{
public:
	static int MAXIMUM_ROTATING_FILES;
	static int ROTATING_FILE_SIZE;
protected:
    static std::shared_ptr<spdlog::logger> spdlogger;
public:
    std::string static GetPath(std::string dir = "/data/log");
    bool static Init(std::string appName);
};
