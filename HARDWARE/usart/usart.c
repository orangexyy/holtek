#include "usart.h"
#include "timer.h"
#include <stdio.h>
#include <stdarg.h>

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

//串口1 USART - 1
#define USART_COM1_CLK(CK)                CK.Bit.USART1
#define USART_COM1_PORT                   HT_USART1
#define USART_COM1_IRQn                   USART1_IRQn
#define USART_COM1_IRQHandler             USART1_IRQHandler
 
#define USART_COM1_TX_GPIO_ID             GPIO_PA
#define USART_COM1_TX_AFIO_PIN            AFIO_PIN_4
#define USART_COM1_TX_AFIO_MODE           AFIO_FUN_USART_UART
 
#define USART_COM1_RX_GPIO_ID             GPIO_PA
#define USART_COM1_RX_AFIO_PIN            AFIO_PIN_5
#define USART_COM1_RX_AFIO_MODE           AFIO_FUN_USART_UART
 


static void USART_CKCU_Config()
{
	CKCU_PeripClockConfig_TypeDef CCLOCK;
	
	CCLOCK.Bit.AFIO = 1;
	USART_COM0_CLK(CCLOCK) = 1;
	USART_COM1_CLK(CCLOCK) = 1;
	
	CKCU_PeripClockConfig(CCLOCK, ENABLE);
}

//static void USART1_CKCU_Config()
//{
//	CKCU_PeripClockConfig_TypeDef CCLOCK;
//	
//	CCLOCK.Bit.AFIO = 1;
//	COM1_CLK(CCLOCK) = 1;
//	
//	CKCU_PeripClockConfig(CCLOCK, ENABLE);
//}
 
static void USART_AFIO_Config()
{
	AFIO_GPxConfig(USART_COM0_TX_GPIO_ID, USART_COM0_TX_AFIO_PIN, AFIO_FUN_USART_UART);
	AFIO_GPxConfig(USART_COM0_RX_GPIO_ID, USART_COM0_RX_AFIO_PIN, AFIO_FUN_USART_UART);
	
	AFIO_GPxConfig(USART_COM1_TX_GPIO_ID, USART_COM1_TX_AFIO_PIN, AFIO_FUN_USART_UART);
	AFIO_GPxConfig(USART_COM1_RX_GPIO_ID, USART_COM1_RX_AFIO_PIN, AFIO_FUN_USART_UART);
}

static void USARTx_Config(void)
{
	USART_InitTypeDef USART_InitStructure0;
	USART_InitTypeDef USART_InitStructure1;
	
	/*USART初始化///////USART0
	*波特率：115200
	*字节长度：8位
	*停止位：1位
	*校验位：无			
	*模式：正常模式
	**/
	USART_InitStructure0.USART_BaudRate = 9600;
	USART_InitStructure0.USART_WordLength = USART_WORDLENGTH_8B;
	USART_InitStructure0.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure0.USART_Parity = USART_PARITY_NO;
	USART_InitStructure0.USART_Mode = USART_MODE_NORMAL;
	USART_Init(USART_COM0_PORT  , &USART_InitStructure0);
	
	/*USART初始化///////USART1
	*波特率：115200
	*字节长度：8位
	*停止位：1位
	*校验位：无			
	*模式：正常模式
	**/
	USART_InitStructure1.USART_BaudRate = 115200;
	USART_InitStructure1.USART_WordLength = USART_WORDLENGTH_8B;
	USART_InitStructure1.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure1.USART_Parity = USART_PARITY_NO;
	USART_InitStructure1.USART_Mode = USART_MODE_NORMAL;
	USART_Init(USART_COM1_PORT  , &USART_InitStructure1);
	
	/* 设置USART中断标志 */
	USART_IntConfig(USART_COM0_PORT, USART_INT_RXDR, ENABLE);
	USART_IntConfig(USART_COM1_PORT, USART_INT_RXDR, ENABLE);
	
	/* 使能USART接收、发送 */
	USART_TxCmd(USART_COM0_PORT, ENABLE);
	USART_RxCmd(USART_COM0_PORT, ENABLE);
	
	USART_TxCmd(USART_COM1_PORT, ENABLE);
	USART_RxCmd(USART_COM1_PORT, ENABLE);
	/* 初始化中断 */
	NVIC_EnableIRQ(USART_COM0_IRQn);
	NVIC_EnableIRQ(USART_COM1_IRQn);
}

void USARTx_Init()
{
	USART_CKCU_Config();
	USART_AFIO_Config();
	USARTx_Config();
}
 
// 发送一个字节
void Usart0_Sendbyte(HT_USART_TypeDef* USARTx, u8 Data)
{
	USART_SendData(USART_COM0_PORT, Data);
	// 等待发送数据寄存器清空
	while (USART_GetFlagStatus(USART_COM0_PORT, USART_FLAG_TXDE) == RESET);		
}
 
void Usart1_Sendbyte(HT_USART_TypeDef* USARTx, u8 Data)
{
	USART_SendData(USART_COM1_PORT, Data);
	// 等待发送数据寄存器清空
	while (USART_GetFlagStatus(USART_COM1_PORT, USART_FLAG_TXDE) == RESET);		
}

//发送每个元素数据长度为8位的数组
void Usart0_SendArray(HT_USART_TypeDef* USARTx, u8 *array,u8 num)
{
	u8 i;
	for( i = 0;i < num;i++)
	{
		Usart0_Sendbyte(USARTx,*array);
		array++;
	}
}

void Usart1_SendArray(HT_USART_TypeDef* USARTx, u8 *array,u8 num)
{
	u8 i;
	for( i = 0;i < num;i++)
	{
		Usart1_Sendbyte(USARTx,*array);
		array++;
	}
}
 
// 发送字符串
void Usart0_SendStr(HT_USART_TypeDef* USARTx, u8 *DAT, u8 len)		//syn6288
{
	uint8_t i;
	for(i = 0; i < len; i++)
	{
		Usart0_Sendbyte(USARTx,*DAT++);
	}
}

void Usart1_SendStr(HT_USART_TypeDef* USARTx, uint8_t *str)
{
	uint8_t i;
	for(i = 0;str[i] != '\0';i++)
	{
		Usart1_Sendbyte(USARTx,str[i]);
	}
}


/////////串口USART0////////
#if EN_USART0_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
u8 USART0_RX_BUF[USART0_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
u16 USART0_RX_STA = 0;     //接收状态标记

void USART0_IRQHandler(void)		//中断接收数据
{
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	u8 RX0_data;
	if( USART_GetFlagStatus(USART_COM0_PORT, USART_FLAG_RXDR ) )         //接收中断
	{
		RX0_data = USART_ReceiveData(USART_COM0_PORT);      		//读取接收到的数据
		if((USART0_RX_STA & 0x8000) == 0) //接收未完成
		{
			if(USART0_RX_STA < USART0_REC_LEN)	//还可以接收数据
			{
				TM_SetCounter(HT_GPTM0, 0); //计数器清空          				//计数器清空
				if(USART0_RX_STA == 0) 				//使能定时器7的中断
				{
					TM_Cmd(HT_GPTM0, ENABLE);; //使能定时器
				}
				USART0_RX_BUF[USART0_RX_STA++] = RX0_data;	//记录接收到的值
			}
			else
			{
				USART0_RX_STA |= 1 << 15;				//强制标记接收完成
			}
		}		
	}

//	if( USART_GetFlagStatus(COM1_PORT, USART_FLAG_RXDR ) )         //接收中断
//	{
//		RX_data = USART_ReceiveData(COM1_PORT);                         //读取接收到的数据
//		printf("data = %c\n",RX_data);                                  //把收到的数据发送回电脑		
//	}
	
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	


/////////串口USART1////////
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART1_RX_BUF[USART1_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART1_RX_STA=0;       //接收状态标记	 

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 RX1_data;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetFlagStatus(USART_COM1_PORT, USART_FLAG_RXDR ))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		RX1_data =USART_ReceiveData(USART_COM1_PORT);	//读取接收到的数据
		
		if((USART1_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART1_RX_STA&0x4000)//接收到了0x0d
				{
				if(RX1_data!=0x0a)USART1_RX_STA=0;//接收错误,重新开始
				else USART1_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(RX1_data==0x0d)USART1_RX_STA|=0x4000;
				else
					{
					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=RX1_data ;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
		} 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	

//void COM1_IRQHandler(void)
//{
//	u8 data;
//	
//	if( USART_GetFlagStatus(COM1_PORT, USART_FLAG_RXDR ) )         //接收中断
//	{
//		data = USART_ReceiveData(COM1_PORT);                         //读取接收到的数据
//		printf("data = %c\n",data);                                  //把收到的数据发送回电脑		
//	}
//}


// 重定向c库函数printf到串口，重定向后可使用printf函数
int fputc0(int ch, FILE *f)
{
	/* 发送一个字节数据到串口 */
	USART_SendData(USART_COM0_PORT, (uint8_t) ch);
		
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(USART_COM0_PORT, USART_FLAG_TXDE) == RESET);		
	
	return (ch);
}

int fputc1(int ch, FILE *f)
{
	/* 发送一个字节数据到串口 */
	USART_SendData(USART_COM1_PORT, (uint8_t) ch);
		
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(USART_COM1_PORT, USART_FLAG_TXDE) == RESET);		
	
	return (ch);
}
 
// 重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc0(FILE *f)
{
	/* 等待串口输入数据 */
	while (USART_GetFlagStatus(USART_COM0_PORT, USART_FLAG_RXDNE) == RESET);
 
	return (int)USART_ReceiveData(USART_COM0_PORT);
}

int fgetc1(FILE *f)
{
	/* 等待串口输入数据 */
	while (USART_GetFlagStatus(USART_COM1_PORT, USART_FLAG_RXDNE) == RESET);
 
	return (int)USART_ReceiveData(USART_COM1_PORT);
}

