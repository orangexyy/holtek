#ifndef _USART_H
#define _USART_H
 
#include "ht32f5xxxx_01.h"
#include <stdio.h>
 
#define DEBUG_USART0					   (0)
#define DEBUG_USART1					   (1)
 
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


#elif	DEBUG_USART1 
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
 
#endif
 
#define USART0_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART0_RX 			1		//使能（1）/禁止（0）串口1接收  	
extern u8  USART0_RX_BUF[USART0_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART0_RX_STA;         		//接收状态标记	
extern u8 RX0_data;


#define USART1_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART1_RX_STA;         		//接收状态标记	
extern u8 RX1_data;
 
 
void USARTx_Init(void);
//串口 0		USART 0 
void Usart0_Sendbyte(HT_USART_TypeDef* USARTx, u8 Data);
void Usart0_SendArray(HT_USART_TypeDef* USARTx, u8 *array,u8 num);
void Usart0_SendStr(HT_USART_TypeDef* USARTx, u8 *DAT, u8 len);		//syn6288
//串口 1		USART 1 
void Usart1_Sendbyte(HT_USART_TypeDef* USARTx, u8 Data);
void Usart1_SendArray(HT_USART_TypeDef* USARTx, u8 *array,u8 num);
void Usart1_SendStr(HT_USART_TypeDef* USARTx, uint8_t *str);

void USART_IRQHandler(void);
//void COM1_IRQHandler(void)

#endif
