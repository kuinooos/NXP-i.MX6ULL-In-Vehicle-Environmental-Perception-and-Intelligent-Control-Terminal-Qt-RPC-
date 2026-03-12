#include "getweather.h"

#include <QDebug>

GetWeather::GetWeather()
{

}

int GetWeather::initWeatherSocket(){
    iSocketClient = socket(AF_INET, SOCK_STREAM, 0);// TCP Socket

    tSocketServerAddr.sin_family      = AF_INET;
    tSocketServerAddr.sin_port        = htons(SERVER_PORT);
    if (0 == inet_aton("192.168.1.102", &tSocketServerAddr.sin_addr))
    {
        qDebug() << "invalid server_ip\n";
        return -1;
    }
    memset(tSocketServerAddr.sin_zero, 0, 8);

    iRet = ::connect(iSocketClient, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
    if (-1 == iRet)
    {
        printf("weather_server connect error!\n");
        return -1;
    }
    return 0;
}

int GetWeather::getWeather(char **weather_text, char **weather_code, char **weather_temperature)
{
        // Recv weather data
        iSendLen = send(iSocketClient, message,strlen(message) , 0);
        if (iSendLen <= 0)
        {
            ::close(iSocketClient);
            return -1;
        }

        // 接收数据
        iRecvLen = recv(iSocketClient, recvBuf, sizeof(recvBuf) - 1, 0);
        if (iRecvLen > 0) {
               recvBuf[iRecvLen] = '\0';  // 添加字符串结束符
               qDebug() << "Received data: " << recvBuf;

               root = cJSON_Parse((const char*)recvBuf);
               if (root == NULL) {
                   qDebug() << "JSON 解析失败！错误信息: " << cJSON_GetErrorPtr();
                   return -1;
               } else {
                   // 获取 "results" 数组
                   cJSON *results = cJSON_GetObjectItem(root, "results");
                   if (results != NULL && cJSON_IsArray(results)) {
                   // 获取数组中的第一个元素
                   cJSON *first_result = cJSON_GetArrayItem(results, 0);
                   if (first_result != NULL) {
                       // 获取 "now" 对象
                       cJSON *now = cJSON_GetObjectItem(first_result, "now");
                       if (now != NULL) {
                       // 获取具体的天气信息字段
                       cJSON *text = cJSON_GetObjectItem(now, "text");
                       cJSON *code = cJSON_GetObjectItem(now, "code");
                       cJSON *temperature = cJSON_GetObjectItem(now, "temperature");

                       // 检查字段是否存在且类型正确
                       if (cJSON_IsString(text) && cJSON_IsString(code) && cJSON_IsString(temperature)) {
                           strcpy(*weather_text, text->valuestring);
                           strcpy(*weather_code, code->valuestring);
                           strcpy(*weather_temperature, temperature->valuestring);


                           qDebug() << weather_text << " " << weather_code << " " << weather_temperature;
                       } else {
                           qDebug() << "JSON 字段缺失或类型错误！";
                       }
                       } else {
                       qDebug() << "未找到 'now' 对象！";
                       }
                   } else {
                       qDebug() << "未找到 'results' 数组的第一个元素！";
                   }
                   } else {
                   qDebug() << "'results' 不是一个有效的数组！";
                   }

                   }
           }else if (iRecvLen == 0) {
               qDebug() << "Connection closed by server";
           } else {
               qDebug() << "recv error!";
           }

    return 0;
}

int GetWeather::destorySocket()
{
    cJSON_Delete(root);
    ::close(iSocketClient);
    return 0;
}
