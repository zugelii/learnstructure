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

using namespace::std;


static std::atomic<bool> main_thread_run(true);
void on_signal_int(int signum)
{
	(void)signum;
	main_thread_run = false;
}
// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
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


int main(int argc, char**argv)
{
    string st(__TIMESTAMP__);
    auto t = chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto a = put_time(localtime(&t), "%Y-%m-%d %X");
    cout << a << endl;
    cout << currentDateTime() << endl; 

}
