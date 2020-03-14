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

int main(int argc, char**argv)
{
 
    get_aimed_format_time();//
    format_string();
    file_operate();//

}
