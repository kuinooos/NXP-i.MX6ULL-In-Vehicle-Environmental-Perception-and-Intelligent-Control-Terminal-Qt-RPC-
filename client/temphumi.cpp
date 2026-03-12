#include "temphumi.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QDebug>
#include <QTimer>

Temphumi::Temphumi(QObject *parent) : QObject(parent)
{
    fd = open("/dev/mydht11",O_RDWR | O_NONBLOCK);
    if(fd == -1){

        qDebug() << "can not open /dev/mydht11";
        running = false;
    }
    qDebug() << "open /dev/mydht11 success! running: " << running;
}

Temphumi::~Temphumi()
{
    running = false;
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}

void Temphumi::startWork()
{
    char buf[2];
    qDebug() << "startWork!" ;
    while (running) {
        if (read(fd, buf, 2) == 2) {
            qDebug() << "temp:" << (int)buf[1] << " humi: "<< (int)buf[0];
            emit newData(buf[1], buf[0]);
            usleep(1000000); // 1秒读取一次
        }
    }

    emit finished();
}
