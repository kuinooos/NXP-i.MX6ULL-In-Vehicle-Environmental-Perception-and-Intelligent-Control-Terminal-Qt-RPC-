#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

static int fd;
static char g_humi, g_temp;


void *dht11_thread(void *arg)
{
    char buf[2];
    while (1)
    {
	    if (2 == read(fd, buf, 2))
	    {
		g_humi = buf[0];
		g_temp = buf[1];
		sleep(1);
	    }
    }
}

void dht11_init(void)
{
    pthread_t tid1;
    fd = open("/dev/mydht11", O_RDWR | O_NONBLOCK);
    if (fd >= 0)
    {
        int ret = pthread_create(&tid1,NULL, dht11_thread,NULL);//创建线程
    }

}

int dht11_read(char *humi, char *temp)
{
    *humi = g_humi;
    *temp = g_temp;
    return 0;
}


