#include <ecal/ecal.h>
#include <thread>
#include <string>
#include <iostream>

using namespace::std;

int main(int argc, char **argv)
{
  int tmp;
  std::string msg;
  eCAL::Initialize(0, nullptr, "minimal c++ publisher");

  // create publisher
  eCAL::CPublisher cam_pub("testecal123", "std::string");
  tmp = 90;
  while(1)
  {
    msg = std::to_string(tmp);
 
	  cam_pub.Send(msg);

	  std::this_thread::sleep_for(std::chrono::milliseconds(4000));	
  }

  // finalize eCAL API
  eCAL::Finalize();
}
