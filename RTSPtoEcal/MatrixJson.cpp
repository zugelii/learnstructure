/*
 * @Description: 
 * @Author: 
 * @Github: 
 * @Date: 2021-02-25 11:36:09
 * @LastEditors: yululi
 * @LastEditTime: 2021-02-25 15:28:30
 * @FilePath: /RTSPtoEcal/MatrixJson.cpp
 */
#include "MatrixJson.h"

#include "matrixFilter.h"

void MatrixJson::LoadJson(MatrixFilter* matrixFilter)
{
    this->ImageInterval = matrixFilter->GetImageInterval();
    this->CaptureFlag   = matrixFilter->GetCaptureFlag();
    this->Rtsp        = matrixFilter->GetRtsp();
    this->CaptureFrame  = matrixFilter->GetCaptureFrame();
}
