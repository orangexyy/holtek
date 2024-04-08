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

//����0 UART - 0
#define USART_COM2_PORT                   HT_UART0

//����1 UART - 1
#define USART_COM3_PORT                   HT_UART1

 //����0 USART - 0
#define USART_COM0_PORT                   HT_USART0

//����1 USART - 1
#define USART_COM1_PORT                   HT_USART1

/**********************TH32 oled��ʾ��ʪ��**********************
Author:С��

Date:2022-4-22
******************************************************/
uint8_t data = 0;
uint8_t sendbuf[128] = {0};


void SYN_FrameInfo(u8 Music, u8 *HZdata)
{
  /****************��Ҫ���͵��ı�**********************************/
  unsigned  char  Frame_Info[50];
  unsigned  char  HZ_Length;
  unsigned  char  ecc  = 0;  			//����У���ֽ�
  unsigned  int i = 0;
  HZ_Length = strlen((char*)HZdata); 			//��Ҫ�����ı��ĳ���

  /*****************֡�̶�������Ϣ**************************************/
  Frame_Info[0] = 0xFD ; 			//����֡ͷFD
  Frame_Info[1] = 0x00 ; 			//�������������ȵĸ��ֽ�
  Frame_Info[2] = HZ_Length + 3; 		//�������������ȵĵ��ֽ�
  Frame_Info[3] = 0x01 ; 			//���������֣��ϳɲ�������
  Frame_Info[4] = 0x01 | Music << 4 ; //����������������������趨

  /*******************У�������***************************************/
  for(i = 0; i < 5; i++)   				//���η��͹���õ�5��֡ͷ�ֽ�
  {
    ecc = ecc ^ (Frame_Info[i]);		//�Է��͵��ֽڽ������У��
  }

  for(i = 0; i < HZ_Length; i++)   		//���η��ʹ��ϳɵ��ı�����
  {
    ecc = ecc ^ (HZdata[i]); 				//�Է��͵��ֽڽ������У��
  }
  /*******************����֡��Ϣ***************************************/
  memcpy(&Frame_Info[5], HZdata, HZ_Length);
  Frame_Info[5 + HZ_Length] = ecc;
  Usart0_SendStr(USART_COM0_PORT,Frame_Info, 5 + HZ_Length + 1);
}

/**************оƬ��������*********************/
u8 SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //ֹͣ�ϳ�
u8 SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //��ͣ�ϳ�
u8 SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //�ָ��ϳ�
u8 SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //״̬��ѯ
u8 SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //����POWER DOWN ״̬����

int main()
{
	LED_Init();
	UARTx_Init();
	USARTx_Init();
	OLED_Init();
	GPTM0_Init();
//	Usart_SendStr(HT_USART1,(uint8_t *)"------HT32 oled TEST-------\r\n");//ѭ�������ַ�����������
	OLED_Clear(0);
	OLED_ShowString(20,1,"ht32 oled",8);
	OLED_ShowString(20,3,(unsigned char *)"C_T:",3); // Current_Temperature ��ǰ�¶�
	OLED_ShowCHinese_Three(80,3,11); //C
			
	OLED_ShowString(20,5,(unsigned char *)"C_H:",3); // Current_Humidity ��ǰʪ��
	OLED_ShowCHinese_Three(80,5,13); //.
	while(1)
	{
//		dou_dht11_get_data();//��ȡ�¶�����
//		sprintf((char *)sendbuf,"��ǰ�¶ȣ�%2d ��ǰʪ��:%2d\n",dou_dht11_temperature_data_h ,dou_dht11_humidity_data_h);
//		Usart_SendStr(HT_USART1,sendbuf);
//		delay_ms(500);
//			dou_dht11_get_data();//��ȡ�¶�����
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
			//ѡ�񱳾�����2��(0���ޱ�������  1-15���������ֿ�ѡ)
		//m[0~16]:0��������Ϊ������16���������������
		//v[0~16]:0�ʶ�����Ϊ������16�ʶ��������
		//t[0~5]:0�ʶ�����������5�ʶ��������
		//���������ù�����ο������ֲ�
		SYN_FrameInfo(2, (char*)"[v10][m1][t4]������ţ��");
		delay_ms(1500);
		delay_ms(1500);



		
	}
}


