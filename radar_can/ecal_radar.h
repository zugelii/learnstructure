#pragma once
#include "radar_data.h"


typedef struct CAN_OBJECTS_TAG
{
    int32_t dyn_prob;
    int32_t meas_stat;
    int32_t prob_exist;
    int32_t obj_class;
    int32_t length;
    int32_t width; 
    float displ_long;
    float displ_lat;
    float vrel_long;
    float speed_lat;    
    float rcs_bm2;
    float displ_long_std;
    float displ_lat_std;
    float vrel_long_std;
    float lat_speed_std;
    float acc_long;
    float acc_lat;
}CAN_OBJECTS;
