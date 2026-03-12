#ifndef GETWEATHER_H
#define GETWEATHER_H

#include <QString>
#include <cJSON.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8888
class GetWeather
{
public:
    GetWeather();

    int initWeatherSocket();
    int getWeather(char** weather_text,char** weather_code,char** weather_temperature);
    int destorySocket();

private:
    int iSocketClient;
    struct sockaddr_in tSocketServerAddr;
    int iRet;
    unsigned char recvBuf[1000];
    int iSendLen;
    int iRecvLen;
    const char* message = "weather";
    cJSON *root = nullptr;
};

#endif // GETWEATHER_H
