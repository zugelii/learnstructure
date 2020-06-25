#include "CSocketCan.h"
#include "radar_data.h"
using namespace std::chrono;

CSocketCan::CSocketCan(std::string name_can) : threadCan(std::bind(&CSocketCan::CanThread, this, name_can))
{
    radarObjectPublisher.Create("ARS4G0_ObjectListPb0");
}

CSocketCan::~CSocketCan()
{
    m_thread_run = false;
    if(threadCan.joinable())
    {
        threadCan.join();
    }
}

void CSocketCan::CanThread(std::string &nameCan)
{
    return;
    if(can_fs = socket(PF_CAN, SOCK_RAW, CAN_RAW) < 0)
    {
        perror("create can scoket error: ");
        exit(1);
    }
    addr.can_family = AF_CAN;
	strncpy(ifr.ifr_name, nameCan.c_str(), nameCan.size());
	if (ioctl(can_fs, SIOCGIFINDEX, &ifr)) 
    {
		perror("ioctl set can error : ");
		exit(1);
	}
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(can_fs, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
    {
		perror("can bind error");
		exit(1);
	}
    start_flag = false;
    poll_fds.fd = can_fs;
    poll_fds.events = POLLIN;
    m_thread_run = true;
    int32_t ret;
    while(m_thread_run)
    {
        ret = poll(&poll_fds, 1, -1);
		if (ret == -1)
		{
			if (errno == EINTR)
            {
                continue;
            }				
			perror("can read poll");
			exit(EXIT_FAILURE);
		}

		if (ret > 0)
		{
			if (poll_fds.revents & POLLIN)
			{
 				readCan();
			}
		}        
    }
}

int32_t CSocketCan::readCan()
{
    int32_t nbytes;
    if ((nbytes = read(can_fs, &frame, sizeof(struct can_frame))) < 0) 
    {
		perror("can read error");
	} else 
    {
        RADAR_READ_DATA radarData;
        radarData.header = frame.can_id;
        radarData.payload.data = 0;
        for(int i = 0; i < frame.can_dlc; i++)
        {
            radarData.payload.data = (radarData.payload.data << 8) + (uint64_t)frame.data[i];
        }
        ProcessCan(radarData);
    }
}

void CSocketCan::testcan(struct can_frame &frame)
{
    RADAR_READ_DATA radarData;
    radarData.header = frame.can_id;
    radarData.payload.data = 0;
    for(int i = 0; i < frame.can_dlc; i++)
    {
        radarData.payload.data = (radarData.payload.data << 8) + (uint64_t)frame.data[i];
    }
    ProcessCan(radarData);
 
}


void CSocketCan::ProcessCan(RADAR_READ_DATA &rd)
{
    PrintfInfo(rd);
    switch (rd.header)
    {
    case 0x201:
        
        break;
    case 0x203:
        
        break;
    case 0x204:
        
        break;
    case 0x60A:
        if(true == start_flag)
        {
            start_flag = false;
            SetHeaderTime();
            //todo  send radar data to ecal
            PublishCanData();
        }
        else//start to store can message
        {
            start_flag = true;
        }
        meas_counter = rd.payload.m_oas.MeasCounter;
        object_number = rd.payload.m_oas.NofObject;        
        break;
    case 0x60B:
        if(true == start_flag)
        {
            CAN_OBJECTS tmp_can_data;
            tmp_can_data.displ_long = rd.payload.m_obs.DistLong * 0.2f - 500.0f;
            tmp_can_data.displ_lat = rd.payload.m_obs.DistLat * 0.2f - 204.6f;
            tmp_can_data.vrel_long = rd.payload.m_obs.VrelLong * 0.25f - 128.0f;
            tmp_can_data.dyn_prob = rd.payload.m_obs.DynProp;
            tmp_can_data.speed_lat = rd.payload.m_obs.VrelLat * 0.25f - 64.0f;
            tmp_can_data.rcs_bm2 = rd.payload.m_obs.RCS;
            if(!m_radar_data.count(rd.payload.m_obs.ID))
            {
                m_radar_data[rd.header] = tmp_can_data;
            }
            else
            {
                m_radar_data[rd.header] = tmp_can_data;
                std::cout << "0x60B id : " << rd.payload.m_obs.ID << "has content in map" << std::endl;
            }          
        }
        else
        {
            std::cout << "0x60B can data is no a completely data" << std::endl;
        }      
        break;
    case 0x60C:
        if(true == start_flag)
        {
            if(m_radar_data.count(rd.payload.m_ocs.ID))
            {
                m_radar_data[rd.payload.m_ocs.ID].displ_long_std = ValueForSignal(rd.payload.m_ocs.DistLong_rms);
                m_radar_data[rd.payload.m_ocs.ID].displ_lat_std = ValueForSignal(rd.payload.m_ocs.DistLat_rms);
                m_radar_data[rd.payload.m_ocs.ID].vrel_long_std = ValueForSignal(rd.payload.m_ocs.VrelLong_rms);
                m_radar_data[rd.payload.m_ocs.ID].lat_speed_std = ValueForSignal(rd.payload.m_ocs.VrelLat_rms);
                m_radar_data[rd.payload.m_ocs.ID].meas_stat = rd.payload.m_ocs.MeasState;
                m_radar_data[rd.payload.m_ocs.ID].prob_exist = rd.payload.m_ocs.ProbOfExist;
            }
            else
            {
                std::cout << "0x60C can data is not a completely data" << std::endl;
            }         
        }  
        break;
    case 0x60D:
        if(true == start_flag)
        {
            if (m_radar_data.count(rd.payload.m_ods.ID))
            {
               m_radar_data[rd.payload.m_ods.ID].acc_long = rd.payload.m_ods.ArelLong * 0.01f - 10.0f;
               m_radar_data[rd.payload.m_ods.ID].acc_lat = rd.payload.m_ods.ArelLat;
               switch (rd.payload.m_ods.Class)
               {
               case class_point:
                   m_radar_data[rd.payload.m_ods.ID].obj_class = pb::ObjectLists::RadarObjectData_TObjClass_OBJCLASS_POINT_OBJECT;
                   break;
               case class_car:
                   m_radar_data[rd.payload.m_ods.ID].obj_class = pb::ObjectLists::RadarObjectData_TObjClass_OBJCLASS_CAR;
                   break;
               case class_truck:
                   m_radar_data[rd.payload.m_ods.ID].obj_class = pb::ObjectLists::RadarObjectData_TObjClass_OBJCLASS_TRUCK;
                   break;
               case class_pedestrian:
                   m_radar_data[rd.payload.m_ods.ID].obj_class = pb::ObjectLists::RadarObjectData_TObjClass_OBJCLASS_PEDESTRIAN;
                   break;
               case class_motorcycle:
               case class_bicycle:
                   m_radar_data[rd.payload.m_ods.ID].obj_class = pb::ObjectLists::RadarObjectData_TObjClass_OBJCLASS_BIKE;
                   break;
               case class_wide:
                   m_radar_data[rd.payload.m_ods.ID].obj_class = pb::ObjectLists::RadarObjectData_TObjClass_OBJCLASS_WIDE_OBJECT;
                   break;
               default:
                   m_radar_data[rd.payload.m_ods.ID].obj_class = pb::ObjectLists::RadarObjectData_TObjClass_OBJCLASS_UNKONWN;
                   break;
               }
               float fLength = rd.payload.m_ods.Length * 0.2f;
               ::google::protobuf::int32 objLength = 0;
               if (fLength < 0.5)	//see RadarObject.proto
                   objLength = 1;
               else if (fLength < 2.0f)
                   objLength = 2;
               else if (fLength < 4.0f)
                   objLength = 3;
               else if (fLength < 6.0f)
                   objLength = 4;
               else if (fLength < 10.0f)
                   objLength = 5;
               else if (fLength < 20.0f)
                   objLength = 6;
               else
                   objLength = 7;                              
               m_radar_data[rd.payload.m_ods.ID].length = objLength;
               float fWidth = rd.payload.m_ods.Width * 0.2f;
               ::google::protobuf::int32 objWidth = 0;    
               if (fWidth < 0.5)
                    objWidth = 1;	//pedestrian
               else if (fWidth < 1.0)
                    objWidth = 2;	//bike
               else if (fWidth < 2.0)
                    objWidth = 3;	//car
               else if (fWidth < 3.0)
                    objWidth = 4;	//truck
               else if (fWidth < 4.0)
                    objWidth = 5;
               else if (fWidth < 6.0)
                    objWidth = 6;
               else
                    objWidth = 7;                           
               m_radar_data[rd.payload.m_ods.ID].width = objWidth;
            }
            else
            {
                std::cout << "0x60D can data is not a completely data" << std::endl;
            }            
        }
        break;
    default:
        printf("no such id : %d\r\n", rd.header);
        break;
    }
}
float CSocketCan::ValueForSignal(uint8_t value)
{
	float signal = 0.0f;
	switch (value)
	{
		case 0x00:
		signal = 0.005f;
		break;
		case 0x01:
		signal = 0.006f;
		break;
		case 0x02:
		signal = 0.008f;
		break;
		case 0x03:
		signal = 0.011f;
		break;
		case 0x04:
		signal = 0.014f;
		break;
		case 0x05:
		signal = 0.018f;
		break;
		case 0x06:
		signal = 0.023f;
		break;
		case 0x07:
		signal = 0.029f;
		break;
		case 0x08:
		signal = 0.038f;
		break;
		case 0x09:
		signal = 0.049f;
		break;
		case 0x0A:
		signal = 0.063f;
		break;
		case 0x0B:
		signal = 0.081f;
		break;
		case 0x0C:
		signal = 0.105f;
		break;
		case 0x0D:
		signal = 0.135f;
		break;
		case 0x0E:
		signal = 0.174f;
		break;
		case 0x0F:
		signal = 0.224f;
		break;
		case 0x10:
		signal = 0.288f;
		break;
		case 0x11:
		signal = 0.371f;
		break;
		case 0x12:
		signal = 0.478f;
		break;
		case 0x13:
		signal = 0.616f;
		break;
		case 0x14:
		signal = 0.794f;
		break;
		case 0x15:
		signal = 1.023f;
		break;
		case 0x16:
		signal = 1.317f;
		break;
		case 0x17:
		signal = 1.697f;
		break;
		case 0x18:
		signal = 2.187f;
		break;
		case 0x19:
		signal = 2.817f;
		break;
		case 0x1A:
		signal = 3.630f;
		break;
		case 0x1B:
		signal = 4.676f;
		break;
		case 0x1C:
		signal = 6.025f;
		break;
		case 0x1D:
		signal = 7.762f;
		break;
		case 0x1E:
		signal = 10.000f;
		break;
	default:
		break;
	};

	return signal;
}

int64_t CSocketCan::SetHeaderTime()
{
	system_clock::time_point now = system_clock::now();
	int64_t msCount = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
	RadarObject.clear_header();
	RadarObject.mutable_header()->set_timestamp(msCount);

	//UTC/GMT
	std::time_t time_t = std::chrono::system_clock::to_time_t(now);
	tm* gmt = gmtime(&time_t);	//to get the local time, use `localtime()`
	RadarObject.mutable_header()->set_timestamp_calendar(std::asctime(gmt));

	return msCount;
}
void CSocketCan::PublishCanData()
{
    //radarObjectPublisher.
    RadarObject.clear_data();
    for (auto itr : m_radar_data)
    {
        /* code */
        pb::ObjectLists::RadarObjectData* objectData = RadarObject.add_data();
        objectData->set_obj_roll_count(meas_counter);
        objectData->set_obj_id(itr.first);
        objectData->set_obj_long_displ_m(itr.second.displ_long);
        objectData->set_obj_lat_displ_m(itr.second.displ_lat);
        objectData->set_obj_vrel_long_ms(itr.second.vrel_long);
        objectData->set_obj_lat_speed_ms(itr.second.speed_lat);
        objectData->set_obj_dyn_prob(itr.second.dyn_prob);
        objectData->set_obj_rcs_value_d_bm2(itr.second.rcs_bm2);
        objectData->set_obj_long_displ_m_std(itr.second.displ_long_std);
        objectData->set_obj_lat_displ_m_std(itr.second.displ_lat_std);
        objectData->set_obj_vrel_long_ms_std(itr.second.vrel_long_std);
        objectData->set_obj_lat_speed_ms_std(itr.second.lat_speed_std);
        objectData->set_obj_meas_stat(itr.second.meas_stat);
        objectData->set_obj_prob_of_exist(itr.second.prob_exist);
        objectData->set_obj_accel_long_ms2(itr.second.acc_long);
        objectData->set_obj_class((pb::ObjectLists::RadarObjectData_TObjClass)itr.second.obj_class);
        objectData->set_obj_length(itr.second.length);
        objectData->set_obj_width(itr.second.width);
    }
    
}