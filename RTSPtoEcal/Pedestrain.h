/*
 * @Description: 
 * @Author: 
 * @Github: 
 * @Date: 2021-02-25 11:36:11
 * @LastEditors: yululi
 * @LastEditTime: 2021-02-25 15:13:15
 * @FilePath: /RTSPtoEcal/Pedestrain.h
 */
#pragma once

#include "comm.pb.h"
#ifdef OPEN_MATRIX
#include "matrix-dev-sdk/export/Obstacle.h"

class Pedestrain
{
private:
    ContiSt::Rect Rect;
    //LeftTop is (0,0); x --> right; y --> bottom
    //the following points are copied from CFusion/data/line_point_pc720.txt
    const float LeftStartX = 12;
    const float LeftStartY = 688;
    const float LeftEndX = 504;
    const float LeftEndY = 360;
    const float RightStartX = 923;
    const float RightStartY = 709;
    const float RightEndX = 676;
    const float RightEndY = 358;
public:
    Pedestrain(ContiSt::Rect& rect);

public:
    bool IsOnRoad();
protected:
    int SideOfLine(float x1, float y1, float x2, float y2, float pointX, float pointY);
};
#endif