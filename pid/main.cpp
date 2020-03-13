#include <iostream>
#include <stdio.h>
#include <thread>
#include <sys/msg.h>
#include <ecal/ecal.h>
#include <ecal/msg/protobuf/publisher.h>
#include <sys/time.h>
#include <mutex>
#include <chrono>
#include <vector>
#include <csignal>
#include <atomic>
#include "PID.h"
using namespace::std;


static std::atomic<bool> main_thread_run(true);
void on_signal_int(int signum)
{
	(void)signum;
	main_thread_run = false;
}
int main()
{
    double Setpoint, Input, Output;
    double consKp=8, consKi=0.2, consKd=0.25;
    string msg;
    eCAL::Initialize(0, nullptr, "power monitor subscriber ");
    eCAL::CSubscriber sub("boxpower", "std::string");

    PID boxPID(&Input, &Output, &Setpoint, consKp, consKi, consKd);
	signal(SIGINT, on_signal_int);
	signal(SIGTERM, on_signal_int);
    Setpoint = 75;
    Output = 0;
    Input = 65;
    while(main_thread_run)
    {
        msg.clear();     
        if(sub.Receive(msg, nullptr, 1000) > 0)
        {
            Input = stoi(msg);
        }
        boxPID.Compute();
            //write pwm
        cout << "setpoint: " << Setpoint << "  Input: " << Input << "  Output: " << Output << endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(CHECK_TIME));
    }
    eCAL::Finalize();

}
