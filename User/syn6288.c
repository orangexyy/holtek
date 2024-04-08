#include "ht32.h"
#include "ht32_board.h"
#include "ht32f5xxxx_01.h"
#include "syn6288.h"
#include "usart.h"
#include "string.h"
#include "delay.h"

#if 	DEBUG_USART0
//串口0 USART - 0
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


	/*	//选择背景音乐2。(0：无背景音乐  1-15：背景音乐可选)
		//m[0~16]:0背景音乐为静音，16背景音乐音量最大
		//v[0~16]:0朗读音量为静音，16朗读音量最大
		//t[0~5]:0朗读语速最慢，5朗读语速最快
		//其他不常用功能请参考数据手册
		SYN_FrameInfo(2, "[v10][m1][t4]烨星 你什么时候回来？");
		delay_ms(1500);
		delay_ms(1500);
	*/


//Music:选择背景音乐。0:无背景音乐，1~15：选择背景音乐
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
  Usart0_SendStr(USART_COM0_PORT，Frame_Info, 5 + HZ_Length + 1);
}


/***********************************************************
* 名    称： YS_SYN_Set(u8 *Info_data)
* 功    能： 主函数	程序入口
* 入口参数： *Info_data:固定的配置信息变量
* 出口参数：
* 说    明：本函数用于配置，停止合成、暂停合成等设置 ，默认波特率9600bps。
* 调用方法：通过调用已经定义的相关数组进行配置。
**********************************************************/
void YS_SYN_Set(u8 *Info_data)
{
  u8 Com_Len;
  Com_Len = strlen((char*)Info_data);
  Usart0_SendStr(USART_COM0_PORT，Info_data, Com_Len);
}
#endif