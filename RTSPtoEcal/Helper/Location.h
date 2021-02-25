#pragma once



class Location
{
private:
    /* data */
public:
    Location(/* args */);
    ~Location();
public:
    double Lat;
    double Lon;
    float Elevation;
    float Installation;
    float Azimuth;
    float Lane1Offset;
    int Direction = 1;      //by default: camera/radar and vehicle are in the same direction
public:
    float CongestionSpeed;
    float SpeedOverHigh;
    float SpeedOverLow;
    int TunnelSpeedLimitHigh;
    int TunnelHeightLimitHigh; //unit dm
    int SpeedLimitSignMax;
    int SpeedLimitSignMin;
    int WidthLimitSign;   // unit dm
    int HeightLimitSign;  // unit dm
    int RampType;
    int TotalLanes;
    int BridgeSpeedLimitHigh;
    int BridgeHeightLimitHigh; //unit dm
};


