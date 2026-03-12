#ifndef RPC_CLIENT_H
#define RPC_CLIENT_H

int RPC_Client_Init(void);
int rpc_dht11_read(char *humi, char *temp);
int rpc_led_control(int on);
int rpc_curtain_control(int on);
int rpc_weather_read(char **text, char **code,char **temperature);

#endif // RPC_CLIENT_H
