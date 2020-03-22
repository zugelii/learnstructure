#include <iostream>
#include <stdio.h>
#include <thread>
#include <sys/msg.h>
#include <sys/time.h>
#include <mutex>
#include <chrono>
#include <vector>
#include <csignal>
#include <atomic>
#include <unistd.h>
#include <sys/syscall.h>//Linux system call for thread id
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <map>
#include <fstream>


#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

#include "lane.h"

#include "share_mem.h"

using namespace::std;


static std::atomic<bool> main_thread_run(true);
void on_signal_int(int signum)
{
	(void)signum;
	main_thread_run = false;
}



// Get current date/time, format is YYYY-MM-DD.HH:mm:ss , this can translate long time to aimed format
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return buf;
}


void get_aimed_format_time()
{
    string st(__TIMESTAMP__);
    cout << "st:" << st << endl;  //get system time from compiler
    auto t = chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto a = put_time(localtime(&t), "%Y-%m-%d %X");
    cout << a << endl; //get system time from chrono ,then change to aimed format
    cout << currentDateTime() << endl;
}


void format_string()  //linke snprinf
{
    double a=35.000,b=14.967;	
    std::ostringstream   ostr; 
	ostr   <<   "a+b="   <<   a   <<   "+"   <<   b   <<   "="   <<   a+b;
	string str = ostr.str();
    cout << str << endl;
}

void file_operate()
{
    ofstream outfile;
    outfile.open("afile.dat", ios::app);
    string wf("write data to a file");
    outfile << wf << endl;
    outfile.flush(); //write date to disk
    outfile.close();
}

void get_mac(char * mac_a)
{
    int                 sockfd;
    struct ifreq        ifr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket error");
        exit(1);
    }
    strncpy(ifr.ifr_name, "ens34", IFNAMSIZ);      //Interface name

    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == 0) {  //SIOCGIFHWADDR 获取hardware address
        memcpy(mac_a, ifr.ifr_hwaddr.sa_data, 6);
    }
}

int main(int argc, char**argv)
{
 
    //get_aimed_format_time();//
    //format_string();
    //file_operate();//s
    /*
    LINE l;
    l.p1.x = 453.0;
    l.p1.y = 229.0;
    l.p2.x = 908.0;
    l.p2.y = 436.0;
    init_lines();
    int32_t lane_number;
    lane_number = get_lane(l, 1);
    cout << "the car's lane is : " << lane_number << endl;
    */
    //test_share_mem();
    char this_mac[6];
    char buf[20];
    get_mac(this_mac);
    sprintf(buf, "%02X%02X%02X%02X%02X%02X", this_mac[0],this_mac[1], this_mac[2], this_mac[3], this_mac[4], this_mac[5]);
    string mc = buf;
    cout << mc << endl;

}
