#include "ht32.h"
#include "ht32_board.h"
#include "ht32f5xxxx_01.h"
#include <stdio.h>
#include "sys.h"
#include "led.h"
#include "timer.h"
#include "delay.h"
#include "dht11.h"
#include "uart.h"
#include "usart.h"
#include "oled.h"
#include "syn6288.h"

//串口0 UART - 0
#define USART_COM2_PORT                   HT_UART0

//串口1 UART - 1
#define USART_COM3_PORT                   HT_UART1

 //串口0 USART - 0
#define USART_COM0_PORT                   HT_USART0

//串口1 USART - 1
#define USART_COM1_PORT                   HT_USART1

/**********************TH32 oled显示温湿度**********************
Author:小殷

Date:2022-4-22
******************************************************/
uint8_t data = 0;
uint8_t sendbuf[128] = {0};


void SYN_FrameInfo(u8 Music, u8 *HZdata)
{
  /****************需要发送的文本**********************************/
  unsigned  char  Frame_Info[50];
  unsigned  char  HZ_Length;
  unsigned  char  ecc  = 0;  			//定义校验字节
  unsigned  int i = 0;
  HZ_Length = strlen((char*)HZdata); 			//需要发送文本的长度

  /*****************帧固定配置信息**************************************/
  Frame_Info[0] = 0xFD ; 			//构造帧头FD
  Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
  Frame_Info[2] = HZ_Length + 3; 		//构造数据区长度的低字节
  Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令
  Frame_Info[4] = 0x01 | Music << 4 ; //构造命令参数：背景音乐设定

  /*******************校验码计算***************************************/
  for(i = 0; i < 5; i++)   				//依次发送构造好的5个帧头字节
  {
    ecc = ecc ^ (Frame_Info[i]);		//对发送的字节进行异或校验
  }

  for(i = 0; i < HZ_Length; i++)   		//依次发送待合成的文本数据
  {
    ecc = ecc ^ (HZdata[i]); 				//对发送的字节进行异或校验
  }
  /*******************发送帧信息***************************************/
  memcpy(&Frame_Info[5], HZdata, HZ_Length);
  Frame_Info[5 + HZ_Length] = ecc;
  Usart0_SendStr(USART_COM0_PORT,Frame_Info, 5 + HZ_Length + 1);
}

/**************芯片设置命令*********************/
u8 SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //停止合成
u8 SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //暂停合成
u8 SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //恢复合成
u8 SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //状态查询
u8 SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //进入POWER DOWN 状态命令

int main()
{
	LED_Init();
	UARTx_Init();
	USARTx_Init();
	OLED_Init();
	GPTM0_Init();
//	Usart_SendStr(HT_USART1,(uint8_t *)"------HT32 oled TEST-------\r\n");//循环发送字符串，测试用
	OLED_Clear(0);
	OLED_ShowString(20,1,"ht32 oled",8);
	OLED_ShowString(20,3,(unsigned char *)"C_T:",3); // Current_Temperature 当前温度
	OLED_ShowCHinese_Three(80,3,11); //C
			
	OLED_ShowString(20,5,(unsigned char *)"C_H:",3); // Current_Humidity 当前湿度
	OLED_ShowCHinese_Three(80,5,13); //.
	while(1)
	{
//		dou_dht11_get_data();//获取温度数据
//		sprintf((char *)sendbuf,"当前温度：%2d 当前湿度:%2d\n",dou_dht11_temperature_data_h ,dou_dht11_humidity_data_h);
//		Usart_SendStr(HT_USART1,sendbuf);
//		delay_ms(500);
//			dou_dht11_get_data();//获取温度数据
//			
//			Usart0_SendStr(USART_COM0_PORT,"cxy666");
//			Usart1_SendStr(USART_COM0_PORT,"cxy777");
//			Uart0_SendStr(USART_COM0_PORT,"cxy888");
//			Uart1_SendStr(USART_COM0_PORT,"cxy999");
//		
//			OLED_ShowString(20,5,(unsigned char *)"Cxy666",6);
//		
////			Usart0_SendStr(COM0_PORT,sendbuf);
////			Usart1_SendStr(COM1_PORT,sendbuf);
////			Uart0_SendStr(COM2_PORT,sendbuf);
////			Uart1_SendStr(COM3_PORT,sendbuf);
//			OLED_ShowNum(50,3,T_H/10,3,3);
//			OLED_ShowNum(60,3,T_L%10,3,3);
//			OLED_ShowNum(50,5,H_H/10,3,3);
//			OLED_ShowNum(60,5,H_L%10,3,3);
//			
			//选择背景音乐2。(0：无背景音乐  1-15：背景音乐可选)
		//m[0~16]:0背景音乐为静音，16背景音乐音量最大
		//v[0~16]:0朗读音量为静音，16朗读音量最大
		//t[0~5]:0朗读语速最慢，5朗读语速最快
		//其他不常用功能请参考数据手册
		SYN_FrameInfo(2, (char*)"[v10][m1][t4]真他妈牛逼");
		delay_ms(1500);
		delay_ms(1500);



		
	}
}


