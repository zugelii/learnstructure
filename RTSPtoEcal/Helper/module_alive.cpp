#include "module_alive.h"

#ifdef __linux__
    #include <unistd.h>
    #include <sys/msg.h>
#endif

#include "Log.h"

using namespace std;

ModAlive::ModAlive(uint16_t k)
{
    if(k > 0)
    {
#ifdef __linux__
        msgid = msgget((key_t)k, 0666 | IPC_CREAT);
        if(msgid < 0)
        {
            spdlog::error("ModAlive msgget failed");
        }
#endif
    }
    else
    {
        msgid = -1;
    }    

}

ModAlive::~ModAlive()
{

}


bool ModAlive::send_alive(uint16_t module_num, uint64_t v, uint8_t act)
{
    bool ret = true;
    struct msg_live data;
    ALVIE_VALUE *input = (ALVIE_VALUE *)data.d;
    data.msg_type = 1;
    input->module = module_num;
    input->hb_count = v;
    input->act = act;
    if(msgid >= 0)
    {
#ifdef __linux__
        if(msgsnd(msgid, (void*)&data, MAX_LEN, 0) == -1)
        {
            spdlog::error("alive msgsnd failed");
            ret = false;			
        }
#endif
    }
    else
    {
        spdlog::error("alive message queue is no exist");
        ret = false;
    }
    return ret;
}

//return false on error
bool ModAlive::rev_msg(uint8_t* buf)
{
#ifdef __linux__
    int32_t msgtype = 0;
	ssize_t ret = msgrcv(msgid, (void*)buf, BUFSIZ, msgtype, IPC_NOWAIT);  //no  wait    
    return (ret != -1);
#else
    return true;
#endif
}
