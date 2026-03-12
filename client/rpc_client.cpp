#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "rpc.h"
#include <cJSON.h>
#include "rpc_client.h"

int g_iSocketClient;

int rpc_led_control(int on)
{
    char buf[100];
    int iLen;
    int ret = -1;

    sprintf(buf, "{\"method\": \"led_control\", \"params\": [%d], \"id\": \"2\" }", on); // 构造json请求
    iLen = send(g_iSocketClient, buf, strlen(buf), 0);// 发送json
    if (iLen ==  (int)strlen(buf))
    {
        while (1) 
        {
            iLen = read(g_iSocketClient, buf, sizeof(buf));// 读取服务端回复
            buf[iLen] = 0;
            if (iLen == 1 && (buf[0] == '\r' || buf[0] == '\n'))
                continue;
            else
                break;
        } 
        
        if (iLen > 0)
        {
		/* 解析json */
            cJSON *root = cJSON_Parse(buf);
            cJSON *result = cJSON_GetObjectItem(root, "result");
            ret = result->valueint;
            cJSON_Delete(root);
            return ret;
        }
        else
        {
            printf("read rpc reply err : %d\n", iLen);
            return -1;
        }
    }
    else
    {
        printf("send rpc request err : %d, %s\n", iLen, strerror(errno));
        return -1;
    }
}

int rpc_curtain_control(int on)
{
    char buf[100];
    int iLen;
    int ret = -1;

    sprintf(buf, "{\"method\": \"curtain_control\", \"params\": [%d], \"id\": \"2\" }", on); // 构造json请求
    iLen = send(g_iSocketClient, buf, strlen(buf), 0);// 发送json
    if (iLen ==  (int)strlen(buf))
    {
        while (1)
        {
            iLen = read(g_iSocketClient, buf, sizeof(buf));// 读取服务端回复
            buf[iLen] = 0;
            if (iLen == 1 && (buf[0] == '\r' || buf[0] == '\n'))
                continue;
            else
                break;
        }

        if (iLen > 0)
        {
        /* 解析json */
            cJSON *root = cJSON_Parse(buf);
            cJSON *result = cJSON_GetObjectItem(root, "result");
            ret = result->valueint;
            cJSON_Delete(root);
            return ret;
        }
        else
        {
            printf("read rpc reply err : %d\n", iLen);
            return -1;
        }
    }
    else
    {
        printf("send rpc request err : %d, %s\n", iLen, strerror(errno));
        return -1;
    }
}

int rpc_dht11_read(char *humi, char *temp)
{
    char buf[300];
    int iLen;

    sprintf(buf, "{\"method\": \"dht11_read\"," \
                   "\"params\": [0], \"id\": \"2\" }");        
            
    iLen = send(g_iSocketClient, buf, strlen(buf), 0);
    if (iLen ==  (int)strlen(buf))
    {
        while (1) 
        {
            iLen = read(g_iSocketClient, buf, sizeof(buf));
            buf[iLen] = 0;
            if (iLen == 1 && (buf[0] == '\r' || buf[0] == '\n'))
                continue;
            else
                break;
        } 
        
        if (iLen > 0)
        {
            cJSON *root = cJSON_Parse(buf);
            cJSON *result = cJSON_GetObjectItem(root, "result");
            if (result)
            {
                cJSON * a = cJSON_GetArrayItem(result,0);
                cJSON * b = cJSON_GetArrayItem(result,1);

                *humi = a->valueint;
                *temp = b->valueint;
                
                cJSON_Delete(root);
                return 0;
            }
            else
            {
                cJSON_Delete(root);
                return -1;
            }
        }
        else
        {
            printf("read rpc reply err : %d\n", iLen);
            return -1;
        }
    }
    else
    {
        printf("send rpc request err : %d, %s\n", iLen, strerror(errno));
        return -1;
    }
}

//int rpc_weather_read(char **text, char **code,char **temperature)
//{
//    char buf[300];
//    int iLen;

//    sprintf(buf, "{\"method\": \"weather_read\"," \
//                   "\"params\": [0], \"id\": \"2\" }");

//    iLen = send(g_iSocketClient, buf, strlen(buf), 0);
//    if (iLen ==  (int)strlen(buf))
//    {
//        while (1)
//        {
//            iLen = read(g_iSocketClient, buf, sizeof(buf));
//            buf[iLen] = 0;
//            if (iLen == 1 && (buf[0] == '\r' || buf[0] == '\n'))
//                continue;
//            else
//                break;
//        }

//        if (iLen > 0)
//        {
//            // 解析 JSON 响应
//           cJSON *json = cJSON_Parse(buf);
//           if (json == NULL) {
//               printf("JSON解析错误: %s\n", cJSON_GetErrorPtr());
//               return -1;
//           }

//           // 提取 text, code, temperature
//           cJSON *textJson = cJSON_GetObjectItemCaseSensitive(json, "text");
//           cJSON *codeJson = cJSON_GetObjectItemCaseSensitive(json, "code");
//           cJSON *temperatureJson = cJSON_GetObjectItemCaseSensitive(json, "temperature");

//           if (cJSON_IsString(textJson) && textJson->valuestring) {
//               *text = strdup(textJson->valuestring);  // 将 text 复制到传入的指针
//           } else {
//               printf("text 数据不正确\n");
//               cJSON_Delete(json);
//               return -1;
//           }

//           if (cJSON_IsString(codeJson) && codeJson->valuestring) {
//               *code = strdup(codeJson->valuestring);  // 将 code 复制到传入的指针
//           } else {
//               printf("code 数据不正确\n");
//               cJSON_Delete(json);
//               return -1;
//           }

//           if (cJSON_IsString(temperatureJson) && temperatureJson->valuestring) {
//               *temperature = strdup(temperatureJson->valuestring);  // 将 temperature 复制到传入的指针
//           } else {
//               printf("temperature 数据不正确\n");
//               cJSON_Delete(json);
//               return -1;
//           }

//           // 释放 JSON 对象
//           cJSON_Delete(json);

//           return 0;
//        }
//        else
//        {
//            printf("send rpc request err : %d, %s\n", iLen, strerror(errno));
//            return -1;
//        }
//    }else{
//        return -1;
//    }
//}

/* 连接RPC Server
 * 返回值: (>0)socket, (-1)失败
 */
int RPC_Client_Init(void) 
{
    struct sockaddr_in tSocketServerAddr;
    int iRet;

    g_iSocketClient = socket(AF_INET, SOCK_STREAM, 0);

    tSocketServerAddr.sin_family      = AF_INET;
    tSocketServerAddr.sin_port        = htons(PORT);  /* host to net, short */
    //tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
    inet_aton("127.0.0.1", &tSocketServerAddr.sin_addr);
    memset(tSocketServerAddr.sin_zero, 0, 8);

    iRet = connect(g_iSocketClient, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
    if (-1 == iRet)
    {
        printf("connect error!\n");
        return -1;
    }

    return g_iSocketClient;
}

