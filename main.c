/********************************* 深圳市航太电子有限公司 *******************************
* 实 验 名 ：wifi连接实验实验
* 实验说明 ：在电脑端设置一个服务器，实现wifi模块自动连接，并通过AT指令与单片机连接
* 实验平台 ：航太ARM单片机开发板
* 连接方式 ：请参考interface.h文件
* 注    意 ：ESP8266.c对数据类型的操作待完善
* 作    者 ：航太电子产品研发部    QQ ：1909197536
* 店    铺 ：http://shop120013844.taobao.com/
****************************************************************************************/

#include "stm32f10x.h"
#include "interface.h"
#include "uart.h"
#include "ESP8266.h"
#include "serialportAPI.h"
#include <stdlib.h>
#include <stdio.h>

#define HOST_NAME   "192.168.31.136"
#define HOST_PORT   8080

#define RECVBUF_SIZE 64

uint32_t tick_3s=0;
uint32_t tick_5ms=0;
uint32_t tick_500ms=0;
unsigned char buffer[RECVBUF_SIZE] = {0};

//全局变量定义

int main(void)
{
	int len=0;
	UserLEDInit();
	USART1Conf(9600);
	DBG("system begin");
	TIM2_Init();

	while(1)
	{
		DBG("chang baud");
		delay(500);
		UartBegin(115200,&USART3Conf,&PutChar);
		SetBaud(19200);
		UartBegin(19200,&USART3Conf,&PutChar);//更改波特率到19200
		if(0 != SetBaud(19200))//在新波特率下检查是否成功
		{
			break;//成功就退出
		}	
	}

	while(0 == WifiInitSmart(HOST_NAME,HOST_PORT));
	
	while(1)
	{	
		if(SerialAvailable() > 5)
		{
			len = recv(buffer, RECVBUF_SIZE, 100);
			if (len > 0)
			{
				tick_3s = 0;
				send(buffer, len);
			} 
		}
		if(tick_5ms >= 5)
		{
			tick_5ms = 0;
			tick_3s++;
			tick_500ms++;
			if(tick_500ms >= 100)
			{
				tick_500ms = 0;
				LEDToggle(LED_PIN);
			}
			if(tick_3s >= 600)
			{
				tick_3s = 0;
				//定时检查连接是否正常，如果不正常,wifi复位重新连接
				if (getSystemStatus() != STATUS_GETLINK)
				{
					DBG("TCP unlink");
					while(!WifiInitSmart(HOST_NAME,HOST_PORT));
				}else
				{
					DBG("TCP link tick");
				}			
			}			
		}

	}
}

