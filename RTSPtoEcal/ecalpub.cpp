#include "ecalpub.h"


void eCalPub::Init()
{
    this->ObstaclesPub.Create("MatrixObstacles");
    this->ImagePub.Create("MatrixImage");
    this->LinePub.Create("MatrixLine");
}

void eCalPub::Publish(const CommProto::Obstacles &obstacles)
{
    this->ObstaclesPub.Send(obstacles);
}

void eCalPub::Publish(const CommProto::Image &image)
{
    ImagePub.Send(image);
}

void eCalPub::Publish(const CommProto::Lines &line)
{
    LinePub.Send(line);
}

