
#include "comm.pb.h"
#ifdef OPEN_MATRIX
#include "matrix-dev-sdk/export/Obstacle.h"

CommProto::Obstacle& Convert(CommProto::Obstacle& ob, const ContiSt::Obstacle& stob)
{
    ob.set_id(stob.id);
    ob.set_type((CommProto::ObstacleType)stob.type);
    ob.set_conf(stob.confidence);
    ob.set_time_stamp(stob.timestamp);

    ob.mutable_img_info()->mutable_rect()->set_left(stob.imgRect.left);
    ob.mutable_img_info()->mutable_rect()->set_top(stob.imgRect.top);
    ob.mutable_img_info()->mutable_rect()->set_right(stob.imgRect.right);
    ob.mutable_img_info()->mutable_rect()->set_bottom(stob.imgRect.bottom);

    ob.mutable_img_info()->mutable_box()->mutable_lower_lt()->set_x(stob.box3d.lower_lt.x);
    ob.mutable_img_info()->mutable_box()->mutable_lower_lt()->set_y(stob.box3d.lower_lt.y);
    ob.mutable_img_info()->mutable_box()->mutable_lower_lb()->set_x(stob.box3d.lower_lb.x);
    ob.mutable_img_info()->mutable_box()->mutable_lower_lb()->set_y(stob.box3d.lower_lb.y);
    ob.mutable_img_info()->mutable_box()->mutable_lower_rb()->set_x(stob.box3d.lower_rb.x);
    ob.mutable_img_info()->mutable_box()->mutable_lower_rb()->set_y(stob.box3d.lower_rb.y);
    ob.mutable_img_info()->mutable_box()->mutable_lower_rt()->set_x(stob.box3d.lower_rt.x);
    ob.mutable_img_info()->mutable_box()->mutable_lower_rt()->set_y(stob.box3d.lower_rt.y);

    ob.mutable_img_info()->mutable_box()->mutable_upper_lt()->set_x(stob.box3d.upper_lt.x);
    ob.mutable_img_info()->mutable_box()->mutable_upper_lt()->set_y(stob.box3d.upper_lt.y);
    ob.mutable_img_info()->mutable_box()->mutable_upper_lb()->set_x(stob.box3d.upper_lb.x);
    ob.mutable_img_info()->mutable_box()->mutable_upper_lb()->set_y(stob.box3d.upper_lb.y);
    ob.mutable_img_info()->mutable_box()->mutable_upper_rb()->set_x(stob.box3d.upper_rb.x);
    ob.mutable_img_info()->mutable_box()->mutable_upper_rb()->set_y(stob.box3d.upper_rb.y);
    ob.mutable_img_info()->mutable_box()->mutable_upper_rt()->set_x(stob.box3d.upper_rt.x);
    ob.mutable_img_info()->mutable_box()->mutable_upper_rt()->set_y(stob.box3d.upper_rt.y);

    ob.mutable_world_info()->set_length(stob.length);
    ob.mutable_world_info()->set_width(stob.width);
    ob.mutable_world_info()->set_height(stob.height);

    for(int32_t p : stob.property)
    {
        ob.add_property(p);
    }

    for(std::string name : stob.property_name)
    {
        ob.add_property_name(name);
    }

    for(int32_t t : stob.property_type)
    {
        ob.add_property_type(t);
    }

    for(float conf : stob.property_conf)
    {
        ob.add_property_conf(conf);
    }

    ob.set_perception_done_ts(stob.perception_done_ts);
    return ob;
}

CommProto::Line& Convert(CommProto::Line& line, const ContiSt::Line& lane)
{
    line.set_id(lane.id);
    line.set_type(lane.type);
    line.set_life_time(lane.life_time);
    line.set_width(lane.width);
    line.set_conf(lane.conf);

    for (auto point : lane.end_points)
    {
        auto pt = line.add_end_points();
        pt->set_x(point.x);
        pt->set_y(point.y);
    }

    for (auto cof : lane.coeffs)
    {
        line.add_coeffs(cof);
    }

    line.set_perception_done_ts(lane.perception_done_ts);

    return line;
}

CommProto::Image& Convert(CommProto::Image& image, const ContiSt::ImageMsg& img)
{
    image.set_width(img.width);
    image.set_height(img.height);
    image.set_time_stamp(img.timestamp);
    image.set_format((CommProto::ImageFormat)img.format);
    image.set_data((const char*)img.img_data, img.len);

    return image;
}


#endif