#ifndef Arduino_h
#define Arduino_h

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h> 

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <fstream>

#include <iomanip>
#include <getopt.h>

#include <sys/ioctl.h>
#include <linux/input.h>

#include <sys/time.h>
#include <stdint.h>

#include <cstring>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>

#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <signal.h>

#include <termios.h> 

#define msleep(n) usleep(n*1000)

using namespace std;

#endif
