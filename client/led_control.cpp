#include <unistd.h>
#include "led_control.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QDebug>

static int fd;
char buf[2];

led_control::led_control()
{
     fd = open("/dev/100ask_led",O_RDWR);

    if(fd == -1) {
        printf("can not open file /dev/100ask_led\n");
        return;
    }
    

}

void led_control::led_sign()
{
    sign = !sign;

    buf[0] = 0;
    if(sign){
        buf[1] = 0;
        qDebug() << "led on";
    }else{
        buf[1] = 1;
        qDebug() << "led off";
    }
    write(fd,buf,2);

}
