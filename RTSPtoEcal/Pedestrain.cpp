/*
 * @Description: 
 * @Author: 
 * @Github: 
 * @Date: 2021-02-25 11:36:11
 * @LastEditors: yululi
 * @LastEditTime: 2021-02-25 15:13:32
 * @FilePath: /RTSPtoEcal/Pedestrain.cpp
 */
#include "Pedestrain.h"
#ifdef OPEN_MATRIX
Pedestrain::Pedestrain(ContiSt::Rect& rect)
{
    Rect.left = rect.left;
    Rect.top = rect.top;
    Rect.right = rect.right;
    Rect.bottom = rect.bottom;
}


bool Pedestrain::IsOnRoad()
{
    //if right botton on left of righLine
    return (SideOfLine(RightStartX, RightStartY, RightEndX, RightEndY, Rect.right, Rect.top) > 0) &&
        //and if left top on right side of left line
        (SideOfLine(LeftStartX, LeftStartY, LeftEndX, LeftEndY, Rect.left, Rect.top) < 0) &&
        //top left is bellow the top line
        (SideOfLine(LeftEndX, LeftEndY, RightEndX, RightEndY, Rect.left, Rect.top) < 0);
}

//positive if P is on left side of line (x1, y1) (x2, y2)
//  ^
//  |  . P
//  |             . B x2, y2
//  |
//  |    . A x1, y1
//  |
//  +------------------>
// Matrix, cross product
//   --`   --`
//   AB  x AP   = (b.X2 - a.X1)*(P.y - A.y1) - (B.y2 - A.y1)*(P.x - A.x1)
int Pedestrain::SideOfLine(float x1, float y1, float x2, float y2, float pointX, float pointY)
{
    return (x2 - x1)*(pointY - y1) - (y2 - y1)*(pointX - x1);
};


#endif