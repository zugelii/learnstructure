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

#include <string>
#include <iostream>
#include "ecal_radar.h"
#pragma pack(1)
struct RadarCfgTag   //header:0x200  8bytes
{
    uint32_t                            : 8;
    uint32_t RCSThreshValid             : 1;
    uint32_t RCSThreshold               : 3;
    uint32_t                            : 4;
    uint32_t CtrlRelayValid             : 1;
    uint32_t CtrlRelay                  : 1;
    uint32_t SendQuaily                 : 1;
    uint32_t SendExtIno                 : 1;
    uint32_t SortIndex                  : 3;
    uint32_t StoreInNVM                 : 1;
    uint32_t SensorID                   : 3;
    uint32_t OutputType                 : 2;
    uint32_t RadarPower                 : 3;
    uint32_t                            : 14;
    uint32_t MaxDistance                : 10;
    uint32_t MaxDistanceValid           : 1;
    uint32_t SendsorIDValid             : 1;
    uint32_t RadarPowerValid            : 1;
    uint32_t OutputTypeValid            : 1;
    uint32_t SendQuailyValid            : 1;
    uint32_t SendExtInoValid            : 1;
    uint32_t SortIndexValid             : 1;
    uint32_t StoreInNVMValid            : 1;
}RadarCfg; 

struct  RadarStateTag  //header:0x201 8 bytes
{
    uint32_t                : 2;
    uint32_t RCSThreshold   : 3;
    uint32_t                : 12;
    uint32_t CtrlRelayCfg   : 1;
    uint32_t OutputTypeCfg  : 2;
    uint32_t SendQuality    : 1;
    uint32_t SendExtinfo    : 1;
    uint32_t                : 2;
    uint32_t SensorID       : 3;
    uint32_t                : 1;
    uint32_t SortIndex      : 3;
    uint32_t RadarPowerCfg  : 3;
    uint32_t                : 7;
    uint32_t VoltageError   : 1;
    uint32_t TemporaryError : 1;
    uint32_t TemperatureError : 1;
    uint32_t Interference   : 1;
    uint32_t PersistentError: 1;
    uint32_t MaxDistaceCfg  : 10;
    uint32_t                : 6;
    uint32_t NVMReadSta     : 1;
    uint32_t NVMWriteSta    : 1;
}RadarState;


struct  FilterCfgTag  //header:0x202  5bytes
{
    uint32_t MaxValue        : 12;
    uint32_t                 : 4;
    uint32_t MinValue        : 12;
    uint32_t                 : 5;
    uint32_t Valid           : 1;
    uint32_t Active          : 1;
    uint32_t Index           : 4;
    uint32_t Type            : 1;
}FilterCfg;


struct  FilterStateHeaderTag  //header:0x203  4bytes
{
    uint32_t                        : 3;
    uint32_t NofObjectFilterCfg     : 5;
    uint32_t                        : 3;
    uint32_t NofClusterFilterCfg    : 5;
}FilterStateHeader; 
struct  FilterStateCfgTag  //header:0x204  5bytes
{
    uint32_t MaxValue        : 13;
    uint32_t                 : 3;
    uint32_t MinValue        : 13;
    uint32_t                 : 5;
    uint32_t Active          : 1;
    uint32_t Index           : 4;
    uint32_t Type            : 1;
}FilterStateCfg; 
enum FILTERSTATEINDEX
{
    NofObj = 0x00,
    Distance = 0x01,
    ProbExists = 0x08,
    Y = 0x09,
    X = 0x0A,
    VXOncome = 0x0C,
    VXDepart = 0x0E,
    ObjectClass = 0x0F
}; 

struct  SoftwareVersionIdentificationTag  //header:0x700  4bytes
{
    uint32_t CountryCode        : 1;
    uint32_t ExtandedRange      : 1;
    uint32_t                    : 6;
    uint32_t PatchLevel         : 8;
    uint32_t MinorRelease       : 8;
    uint32_t MajorRelease       : 8;
}SoftwareVersionIdentification; 

struct ObjectAStatusTag   //header:0x60A  4bytes
{
    uint32_t                : 8;
    uint32_t MeasCounter    : 16;
    uint32_t NofObject      : 8;
}; 

struct  ObjectBStatusTag  //header:0x60B  8bytes
{
    uint32_t RCS              : 8;
    uint32_t DynProp          : 3;
    uint32_t                  : 2;
    uint32_t VrelLat          : 9;
    uint32_t VrelLong         : 10;
    uint32_t DistLat          : 11;
    uint32_t DistLong         : 13;
    uint32_t ID               : 8;
}ObjectBStatus; 

struct  ObjectCStatusTag  //header:0x60C  7bytes
{
    uint32_t                  : 2;
    uint32_t MeasState        : 3;
    uint32_t ProbOfExist      : 3;
    uint32_t                  : 10;
    uint32_t ArelLat_rms      : 5;
    uint32_t ArelLong_rms     : 5;
    uint32_t VrelLat_rms      : 5;
    uint32_t VrelLong_rms     : 5;
    uint32_t DistLat_rms      : 5;
    uint32_t DistLong_rms     : 5;
    uint32_t ID               : 8;
}ObjectCStatus; 

struct ObjectDStatusTag   //header:0x60D  8bytes
{
    uint32_t Width                  : 8;
    uint32_t Length                 : 8;
    uint32_t                        : 6;
    uint32_t OrientationAngle       : 10;
    uint32_t Class                  : 3;
    uint32_t                        : 1;
    uint32_t ArelLat                : 9;
    uint32_t ArelLong               : 11;
    uint32_t ID                     : 8;
}ObjectDStatus; 

typedef struct 
{
    uint16_t header;
    union 
    {
        uint64_t data;
        struct RadarStateTag m_rs;          //0x201  8 bytes
        struct FilterStateHeaderTag m_fs;   //0x203  2 bytes
        struct FilterStateCfgTag m_fsc;      //0x204  5 bytes
        struct ObjectAStatusTag m_oas;       //0x60A  4 bytes
        struct ObjectBStatusTag m_obs;       //0x60B  8 bytes
        struct ObjectCStatusTag m_ocs;       //0x60C  7 bytes
        struct ObjectDStatusTag m_ods;       //0x60D  8 bytes
        struct SoftwareVersionIdentificationTag m_svi; //0x700 4 bytes
    }payload;
}RADAR_READ_DATA;

typedef struct 
{
    uint16_t header;
    union 
    {
        uint64_t data;
        struct RadarCfgTag;            //0x200
        struct FilterCfgTag;           //0x202
    }payload;
}RADAR_WRITE_DATA;


typedef enum
{
	class_point       =	0x0,
	class_car         =	0x1,
	class_truck		  =	0x2,
	class_pedestrian  =	0x3,
	class_motorcycle  =	0x4,
	class_bicycle     = 0x5,
	class_wide        = 0x6,
	class_undefined   = 0x7,
} Obj_3_Extended_e_Obj_Class;

void storeCanData(struct can_frame &canData);
void PrintfInfo(RADAR_READ_DATA &info);
void ProcessInfo(RADAR_READ_DATA &info, CAN_OBJECTS &radarData);