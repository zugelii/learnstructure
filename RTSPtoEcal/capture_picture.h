#pragma once

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <iostream>
#include <stdio.h>

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <sys/inotify.h>
#include <errno.h>
#include <chrono>
#include <ecal/ecal.h>
#include <ecal/msg/protobuf/publisher.h>
#include <csignal>
#include <atomic>
#include <fstream>
#include <thread>
#include <dirent.h>
#include "ecalpub.h"

void captureLoop(std::string ip, int frame);