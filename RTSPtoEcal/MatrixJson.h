/*
 * @Description: 
 * @Author: 
 * @Github: 
 * @Date: 2021-02-25 11:36:10
 * @LastEditors: yululi
 * @LastEditTime: 2021-02-25 15:21:43
 * @FilePath: /RTSPtoEcal/MatrixJson.h
 */
#pragma once

#include <string>
#include <vector>

class MatrixFilter;

class MatrixJson
{
public:
    int ImageInterval;
    int CaptureFlag;
    std::string Rtsp;
    int CaptureFrame;
public:
    void LoadJson(MatrixFilter* matrixFilter);
};