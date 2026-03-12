#include "jsonrpc-c.h"
#include <cstddef>
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
#include "led.h"	
#include "dht11.h"
#include "curtain.h"
#include <pthread.h>
#include "weather.h"

static struct jrpc_server my_server;

/* 参数: {"params" : null} */
cJSON * server_dht11_read(jrpc_context * ctx, cJSON * params, cJSON *id) {
    int array[2];

    array[0] = array[1] = 0;

    while (0 != dht11_read((char *)&array[0], (char *)&array[1]));

    return cJSON_CreateIntArray(array, 2);
}

// /* 参数: {"params" : [0|1]} */
// cJSON * server_curtain_control(jrpc_context * ctx, cJSON * params, cJSON *id) {
//     cJSON * status = cJSON_GetArrayItem(params,0);
//     curtain_control(status->valueint);
//     return cJSON_CreateNumber(0);
// }

/* 参数: {"params" : [0|1]} */
cJSON * server_led_control(jrpc_context * ctx, cJSON * params, cJSON *id) {
    cJSON * status = cJSON_GetArrayItem(params,0);
    led_control(status->valueint);
    return cJSON_CreateNumber(0);
}

/*
类型为
typedef cJSON* (*jrpc_function)(jrpc_context *context, cJSON *params, cJSON* id);
函数指针的实例
*/
// cJSON* server_weather_read(jrpc_context * ctx, cJSON * params, cJSON *id) {
//     char* array[3];//接收text,code,temperature 

//     while (0 != weather_read(&array[0], &array[1],&array[2]));

//     cJSON* arr = cJSON_CreateArray();
//     cJSON_AddItemToArray(arr, cJSON_CreateString(array[0]));
//     cJSON_AddItemToArray(arr, cJSON_CreateString(array[1]));
//     cJSON_AddItemToArray(arr, cJSON_CreateString(array[2]));

//     // 清理内存
//     free(array[0]);
//     free(array[1]);
//     free(array[2]);

//     return arr;
// }


int RPC_Server_Init(void) 
{
    int err;
	/* 绑定ip，port */
    err = jrpc_server_init(&my_server, PORT);
    if (err)
    {
        printf("jrpc_server_init err : %d\n", err);
    }
    
	/* 注册调用函数 */
    char name0[] = "curtain_control";
    char name1[] = "led_control";
    char name2[] = "dht11_read";
    char name3[] = "weather_read";
    // jrpc_register_procedure(&my_server, server_curtain_control, name0, NULL );
    jrpc_register_procedure(&my_server, server_led_control, name1, NULL );
    jrpc_register_procedure(&my_server, server_dht11_read, name2, NULL );
   
    jrpc_server_run(&my_server);
    jrpc_server_destroy(&my_server);

    return 0;
}

int main(int argc, char **argv)
{
    // curtain_init();
    led_init();
    dht11_init();
    RPC_Server_Init();  
    //weather_init(); 
    return 0;
}


