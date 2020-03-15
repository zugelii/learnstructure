#include "lane.h"



using namespace::std;
using namespace rapidjson;


#define FLOAT_ZER0 0.001

vector<LINE> lines_vector;

bool init_lines()
{
	std::string jsonPath = "line.json";

    std::ifstream jsonStream(jsonPath, std::ios::in);
    std::stringstream buffer;
    buffer << jsonStream.rdbuf();
    Document jc;
    jc.Parse(buffer.str().c_str());
    rapidjson::Type t = jc.GetType();
    if (t == rapidjson::kObjectType)
    {
        string key = "l";
        int16_t c = 1;
        key.append(to_string(c));        
        while(jc.HasMember(key.c_str()))
        {
            LINE l;
            if(jc[key.c_str()].HasMember("x1"))
            {
                l.p1.x = jc[key.c_str()]["x1"].GetFloat();
            }
            if(jc[key.c_str()].HasMember("y1"))
            {
                l.p1.y = jc[key.c_str()]["y1"].GetFloat();
            }
            if(jc[key.c_str()].HasMember("x2"))
            {
                l.p2.x = jc[key.c_str()]["x2"].GetFloat();
            }
            if(jc[key.c_str()].HasMember("y2"))
            {
                l.p2.y = jc[key.c_str()]["y2"].GetFloat();
            }
            lines_vector.push_back(l);
            c++;
            key.replace(1, 1, to_string(c));
        }
        cout << "lines vertor size:" << lines_vector.size() << endl;

    }
}

map<int32_t, string> point_position_description = {
    {0, " "},
    {1, "left"},
    {2, "right"}
};
enum {
    ON_LINE,
    LEFT_LINE,
    RIGHT_LINE
};
//return 0: on the line 1:left  2:right
int32_t check_point_position(POINT check_piont, LINE line)
{
    float x = 0.0,y = 0.0, a = 0.0, b = 0.0, slope = 0.0;
    float re = 0.0;
    int32_t pos;
    if((fabs(line.p1.x - line.p2.x) < FLOAT_ZER0) || (fabs(line.p1.y - line.p2.y) < FLOAT_ZER0))
    {
        return -1;  //x/y axle
    }
    slope = (line.p2.y - line.p1.y) / (line.p2.x - line.p1.x);
    x = check_piont.x;
    y = check_piont.y;
    b = line.p2.y - line.p1.y;
    a = line.p2.x - line.p1.x;
    re = (y - line.p1.y) / b - (x - line.p1.x) / a;
    //cout << "re : " << re << endl;
    if(slope > 0.0)
    {
        if(re > FLOAT_ZER0)
        {
            pos = RIGHT_LINE;
        }else if(re < -FLOAT_ZER0)
        {
            pos = LEFT_LINE;
        }
        else
        {
            pos = ON_LINE;
        }
    }else if(slope < 0.0)
    {
        if(re > FLOAT_ZER0)
        {
            pos = LEFT_LINE;
        }else if(re < -FLOAT_ZER0)
        {
            pos = RIGHT_LINE;
        }
        else
        {
            pos = ON_LINE;
        }
    }
    
    return pos;    
}
//check_point: 0: left top; 1: right  bottom; 2: left bottom; 3; right top
int32_t get_lane(RECT test_rect, int16_t check_point)
{


}

//check_point: 0: p1  1: p2
int32_t get_lane(LINE test_line, int16_t check_start_flag)
{
    POINT p =  (check_start_flag == 0 )? test_line.p1 : test_line.p2;
    vector<int32_t> point_pos;
    int32_t ret;
    int32_t lane_num = -1;
    for(int16_t i = 0; i < lines_vector.size(); i++)
    {
        ret = check_point_position(p, lines_vector[i]);
        point_pos.push_back(ret);
        cout << "point is on the l" << i + 1<< " " << point_position_description[ret] << endl;
    }
    if((LEFT_LINE == point_pos[0]) || (RIGHT_LINE == point_pos[lines_vector.size() - 1]))
    {
        cout << "point is out of the road range!" << endl;
        lane_num = -1;
    }
    else
    {
        bool position_flag = false;
        int32_t m;
        for(m = 0; m < point_pos.size() - 1; m++)
        {
            if(RIGHT_LINE == point_pos[m])
            {
                if(LEFT_LINE == point_pos[m + 1])
                {
                    position_flag = true;
                    break;
                }
            }
            if(ON_LINE == point_pos[m]) //need to add way to confirm whether car is changing way
            {
                position_flag = true;
                break;
            }
        }
        if(true == position_flag)
        {
            lane_num = m + 1;
        }
    }
    
    return lane_num;
}
