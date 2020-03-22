#include <ecal/ecal.h>
#include <thread>
#include <string>
#include <iostream>
using namespace std;

void thread_ecal(const string& t)
{
  eCAL::Initialize(0, nullptr, "power mointor subscriber ");
  eCAL::CSubscriber sub(t, "std::string");
  string msg;
  while(1)
  {
      msg.clear();
      sub.Receive(msg, nullptr, -1);    
      cout << "get msg for topic:" << msg << endl;
      
  }
  eCAL::Finalize();
}

int main(int argc, char **argv)
{
  // initialize eCAL API
 eCAL::Initialize(0, nullptr, "minimal c++ publisher");

  //thread th1(thread_ecal, "camerapower");
  //thread th2(thread_ecal, "foo2");
 // th1.join();
  //th2.join();
  // finalize eCAL API
  cout << __FUNCTION__ << endl;
  eCAL::Initialize(0, nullptr, "power mointor subscriber ");
  cout << "log level:" << eCAL::Logging::GetLogLevel() << endl;
 // eCAL::CSubscriber sub("testecal123", "std::string");
  string msg;
  int len = 0;
  while(1)
  {
   //   msg.clear();
      //len = sub.Receive(msg, nullptr, -1); 
      cout << "len: " << len << "msg:" << msg << endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));	
      
  }
  eCAL::Finalize();  

}
