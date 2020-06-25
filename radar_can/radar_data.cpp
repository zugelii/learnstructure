#include "radar_data.h"
#include "ecal_radar.h"

static void PrintRadarStateTag(RADAR_READ_DATA & info)
{
    printf("header : %0X\r\n", info.header);
    printf("NVM Write Status : %d\r\n", info.payload.m_rs.NVMWriteSta);
    printf("NVM read status : %d\r\n", info.payload.m_rs.NVMReadSta);
    printf("Max distance config : %d\r\n", info.payload.m_rs.MaxDistaceCfg);
    printf("Persistent Error : %d\r\n", info.payload.m_rs.PersistentError);
    printf("Interference : %d\r\n", info.payload.m_rs.Interference);
    printf("Temperature Error : %d\r\n", info.payload.m_rs.TemperatureError);
    printf("Temporary Error : %d\r\n", info.payload.m_rs.TemporaryError);
    printf("Voltage Error : %d\r\n", info.payload.m_rs.VoltageError);
    printf("Radar Power Cfg : %d", info.payload.m_rs.RadarPowerCfg);
    printf("RCS Threshold : %d\r\n", info.payload.m_rs.RCSThreshold);
    printf("Sort index : %d \r\n", info.payload.m_rs.SortIndex);
    printf("Sensor ID : %d\r\n", info.payload.m_rs.SensorID);
    printf("Send ext infor flag : %d\r\n", info.payload.m_rs.SendExtinfo);
    printf("Send Quality flag : %d\r\n", info.payload.m_rs.SendQuality);
    printf("output type configure : %d\r\n", info.payload.m_rs.OutputTypeCfg);
    printf("Control relay configure : %d\r\n", info.payload.m_rs.CtrlRelayCfg);
}

static void PrintFilterStateHeaderTag(RADAR_READ_DATA & info)
{
    printf("header : %0X\r\n", info.header);
    printf("number of cluster filter config : %d\r\n", info.payload.m_fs.NofClusterFilterCfg);
    printf("number of object filter config : %d\r\n", info.payload.m_fs.NofObjectFilterCfg);   
}
const char * FilterName[] = 
{
    "NofObj",
    "Distance",
    "Azimuth",
    "VrelOncome",
    "VrelDepart",
    "RCS",
    "Lifetime",
    "Size",
    "ProbExists",
    "Y",
    "X",
    "VYRightLeft",
    "VXOncome",
    "VYLetfRight",
    "VXDepart",
    "ObjectClass"
};
static void PrintFilterStateCfgTag(RADAR_READ_DATA & info)
{
    printf("header : %0X type : %d\r\n", info.header, info.payload.m_fsc.Type);
    printf("filter index : %0X, name : %s\r\n", info.payload.m_fsc.Index, FilterName[info.payload.m_fsc.Index]);
    printf("filter active : %d\r\n", info.payload.m_fsc.Active);
    printf("minum value : %d\r\n", info.payload.m_fsc.MinValue);
    printf("maxum value : %d\r\n", info.payload.m_fsc.MaxValue);
}

static void PrintObjectAStatusTag(RADAR_READ_DATA &info)
{
    printf("header : %0X\r\n", info.header);
    printf("Meas Counter : %d\r\n", info.payload.m_oas.MeasCounter);
    printf("number of objects : %d\r\n", info.payload.m_oas.NofObject);
}

static void PrintObjectBStatusTag(RADAR_READ_DATA &info)
{
    printf("header : %0X\r\n", info.header);
    printf("objects ID : %d\r\n", info.payload.m_obs.ID);
    printf("dist long : %d\r\n", info.payload.m_obs.DistLong);
    printf("dist lat : %d\r\n", info.payload.m_obs.DistLat);
    printf("vrel long : %d\r\n", info.payload.m_obs.VrelLong);
    printf("vrel lat : %d\r\n", info.payload.m_obs.VrelLat);
    printf("Dyn prop : %d\r\n",info.payload.m_obs.DynProp);
    printf("RCS : %d \r\n", info.payload.m_obs.RCS);
}

static void PrintObjectCStatusTag(RADAR_READ_DATA &info)
{
    printf("header : %0X\r\n", info.header);
    printf("objects ID : %d\r\n", info.payload.m_ocs.ID);
    printf("probility of exist : %d\r\n", info.payload.m_ocs.ProbOfExist);
}

static void PrintObjectDStatusTag(RADAR_READ_DATA &info)
{
    printf("header : %0X\r\n", info.header);
    printf("objects ID : %d\r\n", info.payload.m_ods.ID);
    printf("Arel long : %d\r\n", info.payload.m_ods.ArelLong);
    printf("Arel lat : %d\r\n", info.payload.m_ods.ArelLat);
    printf("class : %d\r\n", info.payload.m_ods.Class);
    printf("orientation angle : %d\r\n", info.payload.m_ods.OrientationAngle);
    printf("length : %d\r\n", info.payload.m_ods.Length);
    printf("width : %d\r\n", info.payload.m_ods.Width);
}

void PrintfInfo(RADAR_READ_DATA &info)
{
    switch (info.header)
    {
    case 0x201:
        PrintRadarStateTag(info);
        break;
    case 0x203:
        PrintFilterStateHeaderTag(info);
        break;
    case 0x204:
        PrintFilterStateCfgTag(info);
        break;
    case 0x60A:
        PrintObjectAStatusTag(info);
        break;
    case 0x60B:
        PrintObjectBStatusTag(info);
        break;
    case 0x60C:
        PrintObjectCStatusTag(info);
        break;
    case 0x60D:
        PrintObjectDStatusTag(info);
        break;
    default:
        printf("no such id : %d\r\n", info.header);
        break;
    }
}

