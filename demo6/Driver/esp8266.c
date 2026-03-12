#include "debug.h"
#include "uart.h"

u8 string1[] = "AT+RST\r\n";
u8 string2[] = "AT+CWMODE=1\r\n";
u8 string3[] = "AT+CWDHCP=1,1\r\n";
u8 string4[] = "AT+CWJAP=\"wifitest\",\"bietouwowifi\"\r\n";
u8 string5[] = "AT+MQTTUSERCFG=0,1,\"ch32\",\"ij02LHn7yu\",\"version=2018-10-31&res=products%2Fij02LHn7yu%2Fdevices%2Fch32&et=9999999999&method=md5&sign=oGh5bvAK5shoYpkkn2aa4A%3D%3D\",0,0,\"\"\r\n";
u8 string6[] = "AT+MQTTCONN=0,\"mqtts.heclouds.com\",1883,1\r\n";
u8 string7[] = "AT+MQTTSUB=0,\"$sys/ij02LHn7yu/ch32/thing/property/set\",1\r\n";
u8 string8[] = "AT+MQTTPUB=0,\"$sys/ij02LHn7yu/ch32/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"password\\\":{\\\"value\\\":32}}}\",0,0\r\n";

// lock状态上报命令
u8 lock_true_cmd[]  = "AT+MQTTPUB=0,\"$sys/ij02LHn7yu/ch32/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"lock\\\":{\\\"value\\\":true}}}\",0,0\r\n";
u8 lock_false_cmd[] = "AT+MQTTPUB=0,\"$sys/ij02LHn7yu/ch32/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"lock\\\":{\\\"value\\\":false}}}\",0,0\r\n";

void esp8266_init()
{
    Uart6_Init();
}


void onenet_init()
{
    uart6_send_string(string1,sizeof(string1)-1);
    Delay_Ms(1000);
    uart6_send_string(string2,sizeof(string2)-1);
    Delay_Ms(1000);
    uart6_send_string(string3,sizeof(string3)-1);
    Delay_Ms(1000);
    uart6_send_string(string4,sizeof(string4)-1);
    Delay_Ms(1000);
    Delay_Ms(1000);
    uart6_send_string(string5,sizeof(string5)-1);
    Delay_Ms(1000);
    Delay_Ms(1000);
    uart6_send_string(string6,sizeof(string6)-1);
    Delay_Ms(1000);
    Delay_Ms(1000);
    uart6_send_string(string7,sizeof(string7)-1);
    Delay_Ms(1000);
    Delay_Ms(1000);
    uart6_send_string(string8,sizeof(string8)-1);
    Delay_Ms(1000);
    Delay_Ms(1000);

    uart6_send_string(lock_true_cmd, sizeof(lock_true_cmd)-1);
    Delay_Ms(1000);
    // 新增: 上报初始 lock 状态（默认关门 lock_flag=1 → lock:true）                                                                                   │
}
