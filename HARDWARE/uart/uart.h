#ifndef _UART_H
#define _UART_H
 
#include "ht32f5xxxx_01.h"
#include <stdio.h>
 
#define DEBUG_UART0					   (0)
#define DEBUG_UART1					   (1)
 
#if 	DEBUG_UART0
//串口0 UART - 0
#define UART_COM0_CLK(CK)                CK.Bit.UART0
#define UART_COM0_PORT                   HT_UART0
#define UART_COM0_IRQn                   UART0_IRQn
#define UART_COM0_IRQHandler             UART0_IRQHandler
 
#define UART_COM0_TX_GPIO_ID             GPIO_PB
#define UART_COM0_TX_AFIO_PIN            AFIO_PIN_2
#define UART_COM0_TX_AFIO_MODE           AFIO_FUN_USART_UART
 
#define UART_COM0_RX_GPIO_ID             GPIO_PB
#define UART_COM0_RX_AFIO_PIN            AFIO_PIN_3
#define UART_COM0_RX_AFIO_MODE           AFIO_FUN_USART_UART
 
#elif	DEBUG_UART1 
//串口1 UART - 1
#define UART_COM1_CLK(CK)                CK.Bit.UART1
#define UART_COM1_PORT                   HT_UART1
#define UART_COM1_IRQn                   UART1_IRQn
#define UART_COM1_IRQHandler             UART1_IRQHandler
 
#define UART_COM1_TX_GPIO_ID             GPIO_PB
#define UART_COM1_TX_AFIO_PIN            AFIO_PIN_4
#define UART_COM1_TX_AFIO_MODE           AFIO_FUN_USART_UART
 
#define UART_COM1_RX_GPIO_ID             GPIO_PB
#define UART_COM1_RX_AFIO_PIN            AFIO_PIN_5
#define UART_COM1_RX_AFIO_MODE           AFIO_FUN_USART_UART
 
#endif
 
void UARTx_Init(void);
//串口 2		UART 0 
void Uart0_Sendbyte(HT_USART_TypeDef* UARTx, u8 Data);
void Uart0_SendArray(HT_USART_TypeDef* UARTx, u8 *array,u8 num);
void Uart0_SendStr(HT_USART_TypeDef* UARTx, uint8_t *str);
//串口 3		UART 1 
void Uart1_Sendbyte(HT_USART_TypeDef* UARTx, u8 Data);
void Uart1_SendArray(HT_USART_TypeDef* UARTx, u8 *array,u8 num);
void Uart1_SendStr(HT_USART_TypeDef* UARTx, uint8_t *str);


void UART_IRQHandler(void);

#endif
