#include "ht32.h"
#include "ht32_board.h"
#include "ht32f5xxxx_01.h"
#include "syn6288.h"
#include "usart.h"
#include "string.h"
#include "delay.h"

#if 	DEBUG_USART0
//����0 USART - 0
#define USART_COM0_CLK(CK)                CK.Bit.USART0
#define USART_COM0_PORT                   HT_USART0
#define USART_COM0_IRQn                  	USART0_IRQn
#define USART_COM0_IRQHandler             USART0_IRQHandler
 
#define USART_COM0_TX_GPIO_ID             GPIO_PA
#define USART_COM0_TX_AFIO_PIN            AFIO_PIN_2
#define USART_COM0_TX_AFIO_MODE           AFIO_FUN_USART_UART
 
#define USART_COM0_RX_GPIO_ID             GPIO_PA
#define USART_COM0_RX_AFIO_PIN            AFIO_PIN_3
#define USART_COM0_RX_AFIO_MODE           AFIO_FUN_USART_UART 


	/*	//ѡ�񱳾�����2��(0���ޱ�������  1-15���������ֿ�ѡ)
		//m[0~16]:0��������Ϊ������16���������������
		//v[0~16]:0�ʶ�����Ϊ������16�ʶ��������
		//t[0~5]:0�ʶ�����������5�ʶ��������
		//���������ù�����ο������ֲ�
		SYN_FrameInfo(2, "[v10][m1][t4]���� ��ʲôʱ�������");
		delay_ms(1500);
		delay_ms(1500);
	*/


//Music:ѡ�񱳾����֡�0:�ޱ������֣�1~15��ѡ�񱳾�����
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
  Usart0_SendStr(USART_COM0_PORT��Frame_Info, 5 + HZ_Length + 1);
}


/***********************************************************
* ��    �ƣ� YS_SYN_Set(u8 *Info_data)
* ��    �ܣ� ������	�������
* ��ڲ����� *Info_data:�̶���������Ϣ����
* ���ڲ�����
* ˵    �����������������ã�ֹͣ�ϳɡ���ͣ�ϳɵ����� ��Ĭ�ϲ�����9600bps��
* ���÷�����ͨ�������Ѿ�������������������á�
**********************************************************/
void YS_SYN_Set(u8 *Info_data)
{
  u8 Com_Len;
  Com_Len = strlen((char*)Info_data);
  Usart0_SendStr(USART_COM0_PORT��Info_data, Com_Len);
}
#endif