#ifndef DETECTPEOPLE_H
#define DETECTPEOPLE_H

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <QObject>
#include <QDebug>

class DetectPeople : public QObject
{
    Q_OBJECT
public:
    DetectPeople(){
        fd=open("/dev/100ask_gpio_sr501", O_RDONLY | O_NONBLOCK);
        if (fd < 0) {
                perror("Open /dev/100ask_gpio_sr501 failed");
                exit(1);
            }
    };
    ~DetectPeople(){
         close(fd);
    };

    static int fd;
    static int g_peopleSignal;

    int get_people_coming(){
        return g_peopleSignal;
    };
    void sigio_handler(){
        if (read(fd, &g_peopleSignal, sizeof(g_peopleSignal)) == 4) {
            if(g_peopleSignal!=0){
                qDebug() << "Get g_peopleSignal:" << g_peopleSignal;
            }
        }
        sleep(1);
        g_peopleSignal = 0;
    };
};


#endif // DETECTPEOPLE_H
