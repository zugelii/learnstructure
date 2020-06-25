#include "radar_data.h"

using namespace std;

uint8_t buf_201[] = {0x40, 0x19, 0x00, 0x00, 0x00, 0xf4, 0x00, 0x00};
uint8_t buf_700[] = {0x40, 0x1e, 0x01, 0x00};
uint8_t buf_60a[] = {0x0f, 0x06, 0x4d, 0x10};
uint8_t buf_60b[] = {0x00, 0x4e, 0x6b, 0xfd, 0x7f, 0x9f, 0xc1, 0x96};
uint8_t buf_60c[] = {0x00, 0x84, 0xa5, 0x3a, 0x02, 0x20, 0xe8};
uint8_t buf_60d[] = {0x00, 0x74, 0x6f, 0xa0, 0x70, 0x80, 0x05, 0x05};


int main(int agrc, char **argv)
{
    RADAR_READ_DATA test_data;
    //0x201  40 19 00 00 00 f4 00 00  
    test_data.payload.data = 0;    
    test_data.payload.data = (uint64_t)((uint64_t)0x40 << 56 | (uint64_t)0x19 << 48 | (uint64_t)0xf4 << 16);
    test_data.header = 0x201;
    PrintfInfo(test_data);
    //0x60a  0f 06 4d 10   
    test_data.payload.data = 0;    
    test_data.payload.data = (uint64_t)((uint64_t)0x0f << 24 | (uint64_t)0x06 << 16 | (uint64_t)0x4d << 8 | (uint64_t)0x10 << 0);
    test_data.header = 0x60a;
    PrintfInfo(test_data);
    //<0x60b> [8] 00 4e 6b fd 7f 9f c1 96    
    test_data.payload.data = 0;    
    test_data.payload.data = (uint64_t)((uint64_t)0x00 << 56 | (uint64_t)0x4e << 48 | (uint64_t)0x6b << 40 | (uint64_t)0xfd << 32 |
                                        (uint64_t)0x7f << 24 | (uint64_t)0x9f << 16 | (uint64_t)0xc1 << 8 | (uint64_t)0x96 << 0);
    test_data.header = 0x60b;
    PrintfInfo(test_data);
    //<0x60c> [7] 01 84 a5 3a 02 20 e8
    test_data.payload.data = 0;    
    test_data.payload.data = (uint64_t)((uint64_t)0x00 << 56 | (uint64_t)0x01 << 48 | (uint64_t)0x84 << 40 | (uint64_t)0xa5 << 32 |
                                        (uint64_t)0x3a << 24 | (uint64_t)0x02 << 16 | (uint64_t)0x20 << 8 | (uint64_t)0xe8 << 0);
    test_data.header = 0x60c;
    PrintfInfo(test_data);  
    //<0x60d> [8] 00 74 6f a0 70 80 05 05 
    test_data.payload.data = 0;    
    test_data.payload.data = (uint64_t)((uint64_t)0x00 << 56 | (uint64_t)0x74 << 48 | (uint64_t)0x6f << 40 | (uint64_t)0xa0 << 32 |
                                        (uint64_t)0x70 << 24 | (uint64_t)0x80 << 16 | (uint64_t)0x05 << 8 | (uint64_t)0x05 << 0);
    test_data.header = 0x60d;
    PrintfInfo(test_data);     
    printf("data: %lX\r\n", test_data.payload.data);
    return 0;
}