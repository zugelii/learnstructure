#pragma once

#include <iostream>
#include <stdio.h>
#include <thread>
#include <sys/msg.h>
#include <sys/time.h>
#include <mutex>
#include <chrono>
#include <vector>
#include <csignal>
#include <atomic>
#include <unistd.h>
#include <sys/syscall.h>//Linux system call for thread id
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <map>
#include <fstream>
#include <cmath>
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

typedef struct{
    float x;
    float y;
}POINT;


typedef struct{
    POINT left_top;
    POINT right_bottom;
}RECT;

typedef struct{
    POINT p1;
    POINT p2;
}LINE;

bool init_lines();
//return 0: on the line 1:down  2:up
int32_t check_point_position(POINT check_piont, LINE line);
//check_point: 0: left top; 1: right  bottom; 2: left bottom; 3; right top
int32_t get_lane(RECT test_rect, int16_t check_point);
//check_point: 0: p1  1: p2
int32_t get_lane(LINE test_line, int16_t check_start_flag);

bool init_lines(int16_t max);
//check_point: 0: p1  1: p2
int32_t get_lane_multi_point(LINE test_line, int16_t check_start_flag);