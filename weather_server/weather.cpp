#include <iostream>
#include <curl/curl.h>
#include "cJSON.h"
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <atomic>


#define SERVER_PORT 8888
#define BACKLOG     10

static std::string api_key;
static std::string city;
static std::string url;
static CURL* curl;
static std::string response_data;
static pthread_mutex_t g_tMutex  = PTHREAD_MUTEX_INITIALIZER;
static int iClientNum = -1;
static int iSocketServer;
static std::atomic<bool> getWeather_thread_running(true);
static std::atomic<bool> main_running(true);
static std::atomic<bool> network_thread_running(true);
static struct sockaddr_in tSocketServerAddr;
static std::string text_1;
static std::string code_1;
static std::string temperature_1;

/* 处理结束信号*/
void signal_handler(int sig) {
    std::cout << "\nCaught signal " << sig << ", shutting down...\n";
    main_running = false;
	network_thread_running = false;
	getWeather_thread_running = false;
    close(iSocketServer); // 主动关闭服务端 socket，accept 会报错返回
	curl_easy_cleanup(curl);// 清理curl资源,套接字资源
	curl_global_cleanup();
}

/*初始化服务端套接字*/
int init_Socket_imx6ull(){
	int iRet;
	iSocketServer = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == iSocketServer)
	{
		printf("socket error!\n");
		return -1;
	}

	tSocketServerAddr.sin_family	  = AF_INET;
	tSocketServerAddr.sin_port        = htons(SERVER_PORT);
    tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
	
	memset(tSocketServerAddr.sin_zero, 0, 8);

	iRet = bind(iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
	if (-1 == iRet)
	{
		printf("bind error!\n");
		return -1;
	}

	iRet = listen(iSocketServer, BACKLOG);
	if (-1 == iRet)
	{
		printf("listen error!\n");
		return -1;
	}

	return 0;
}


// curl回调函数，用于将接收到的数据写入response_data
size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
	pthread_mutex_lock(&g_tMutex);
	response_data.append(ptr, size * nmemb);			
	pthread_mutex_unlock(&g_tMutex);

    return size * nmemb;
}
/*初始化curl*/
int init_curl(){
	// 替换为API密钥
	api_key = "S0uPdXrJFOwqu2VZ9";
	// 替换为你要查询的城市，建议加上省份避免歧义
	city = "fuzhou";
	// 构建请求URL
	url = std::string("https://api.seniverse.com/v3/weather/now.json?")+
				  "key=" + api_key +
				  "&location=" + city +
				  "&language=zh-Hans&unit=c";

	curl = curl_easy_init();
	if (curl == NULL) {
    std::cerr << "curl init failed!" << std::endl;
    return -1;
	}
	// 设置请求URL
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	// 设置写入回调函数
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

	return 0;
}

static void* getWeather(void* arg) {
	cJSON *root;
	while (getWeather_thread_running) {

		/*当修改天气数据时，需要对数据上锁*/
		pthread_mutex_lock(&g_tMutex);
		response_data.clear();
		pthread_mutex_unlock(&g_tMutex);
		
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
					continue;
				}

		// 使用 cJSON 解析 JSON 数据
		root = cJSON_Parse(response_data.c_str());
		if (root == NULL) {
		    std::cerr << "JSON 解析失败！错误信息: " << cJSON_GetErrorPtr() << std::endl;
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
				    text_1 = text->valuestring;
				    code_1 = code->valuestring;
				    temperature_1 = temperature->valuestring;

				    std::cout << text_1 << " " << code_1 << " " << temperature_1 << std::endl;
				} else {
				    std::cerr << "JSON 字段缺失或类型错误！" << std::endl;
				}
			    } else {
				std::cerr << "未找到 'now' 对象！" << std::endl;
			    }
			} else {
			    std::cerr << "未找到 'results' 数组的第一个元素！" << std::endl;
			}
		    } else {
			std::cerr << "'results' 不是一个有效的数组！" << std::endl;
		    	}
		}			
	
		// 每一次循环完成释放 cJSON 对象
	    cJSON_Delete(root);
		sleep(5);
     }

	return NULL;
}
int dht11_read(char *text, char *code,char *temperature){
	text = text_1.c_str();
	code = code_1;
	temperature = temperature_1;
};

// void* handle_client(void* arg){
// 	int client_socket = *(int*)arg;
// 	delete (int*)arg;
// 	unsigned char RecvBuf[10];
// 	int RecvLen;
// 	int	Sendlen;
// 	while(network_thread_running)
// 		{
// 			RecvLen = recv(client_socket, RecvBuf, 999, 0);
// 			if (RecvLen <= 0)
// 			{
// 				// 检查连接是否关闭
// 			    if (RecvLen == 0) {
// 				std::cout << "Client disconnected!" << std::endl;
// 			    } else {
// 				std::cerr << "Recv error: " << strerror(errno) << std::endl;
// 			    }
// 			    close(client_socket); // 关闭连接
// 			    break;
// 			}
// 			else
// 			{
// 				RecvBuf[RecvLen] = '\0';
// 				printf("Get Msg From Client %d: %s\n", iClientNum, RecvBuf);
// 			}

// 			if(strcmp((const char*)RecvBuf,"weather") == 0){
// 				Sendlen = send(client_socket,response_data.c_str(),strlen(response_data.c_str()),0);
// 				if(Sendlen < 0){
// 					printf("send %s to client fail!\n",response_data.c_str());
// 				}
// 			}
// 		}
// 	close(client_socket);

// 	return NULL;
// }

void weather_init(void){
	signal(SIGINT, signal_handler); // 捕捉 Ctrl+C (SIGINT)

	init_curl();//初始化curl
	init_Socket_imx6ull();//初始化服务端套接字

	/*创建天气线程*/
	{
		pthread_t weather_thread;
		pthread_create(&weather_thread,NULL,getWeather,NULL);
		pthread_detach(weather_thread);
	}

	// /*多线程持续接收多个客户端*/
	// while(main_running){
	// 	struct sockaddr_in client_addr;// 客户端地址结构体
	//     socklen_t addr_len = sizeof(client_addr); // 客户端结构体长度 
	//     int* client_socket = new int;
	//     *client_socket = accept(iSocketServer, (struct sockaddr*)&client_addr, &addr_len);//获取套接字
	//     iClientNum++;
	//     if (*client_socket == -1) {
	//         std::cerr << "Accept failed!" << std::endl;
	//         delete client_socket;
	//         continue;
	//     }
		
	// 	std::cout << "New client connected from " << inet_ntoa(client_addr.sin_addr) << std::endl;
		
	// 	pthread_t client_thread;
	// 	pthread_create(&client_thread,NULL,handle_client,(void*)client_socket);
	// 	pthread_detach(client_thread);// 分离客户端线程，不用join回收
	// }
}
