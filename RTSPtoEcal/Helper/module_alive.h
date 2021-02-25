#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>

#define MAX_LEN 20

struct msg_live
{
	int32_t  msg_type;
	uint8_t d[MAX_LEN];
};

enum
{
    MODULE_NONE,
    MODULE_NOMAL,
    MODULE_ON,
    MODULE_OFF,
    MODULE_ERROR
};

#define ECUPOWER_KEY   0X1230
#define CANNET_KEY     0X1233

#define ECAL2RSU_MODULE   0x5234
#define SMARTRAODEM_MODULE  0x5236
#define CV2ECAL_MODULE    0x5237
#define CANNET_MODULE     0x5239
#pragma pack(1)
typedef struct
{
    uint16_t module;
    uint64_t hb_count;
    uint8_t  act;
}ALVIE_VALUE;
#pragma pack()
class ModAlive
{
public:
    ModAlive(uint16_t);
    ~ModAlive();
    bool send_alive(uint16_t module_num, uint64_t v, uint8_t act);
    bool rev_msg(uint8_t*);
    int32_t msgid;
    
};

