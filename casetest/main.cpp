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
using namespace::std;


static std::atomic<bool> main_thread_run(true);
void on_signal_int(int signum)
{
	(void)signum;
	main_thread_run = false;
}
int main(int argc, char**argv)
{
    pid_t p = getpid();
    std::cout << "onradar pid:" << p << std::endl;
    printf("child thread lwpid = %u\n", syscall(SYS_gettid));
    printf("child thread tid = %u\n", pthread_self());
    sleep(10);

}
