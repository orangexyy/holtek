#include "uart.h"

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


static void UART_CKCU_Config()
{
	CKCU_PeripClockConfig_TypeDef CCLOCK;
	
	CCLOCK.Bit.AFIO = 1;
	UART_COM0_CLK(CCLOCK) = 1;
	UART_COM1_CLK(CCLOCK) = 1;
	
	CKCU_PeripClockConfig(CCLOCK, ENABLE);
}
 
static void UART_AFIO_Config()
{
	AFIO_GPxConfig(UART_COM0_TX_GPIO_ID, UART_COM0_TX_AFIO_PIN, AFIO_FUN_USART_UART);
	AFIO_GPxConfig(UART_COM0_RX_GPIO_ID, UART_COM0_RX_AFIO_PIN, AFIO_FUN_USART_UART);
	
	AFIO_GPxConfig(UART_COM1_TX_GPIO_ID, UART_COM1_TX_AFIO_PIN, AFIO_FUN_USART_UART);
	AFIO_GPxConfig(UART_COM1_RX_GPIO_ID, UART_COM1_RX_AFIO_PIN, AFIO_FUN_USART_UART);
}
 
static void UARTx_Config()
{
	USART_InitTypeDef USART_InitStructure2;
	USART_InitTypeDef USART_InitStructure3;
	
	/*USART初始化
	*波特率：115200
	*字节长度：8位
	*停止位：1位
	*校验位：无			
	*模式：正常模式
	**/
	USART_InitStructure2.USART_BaudRate = 115200;
	USART_InitStructure2.USART_WordLength = USART_WORDLENGTH_8B;
	USART_InitStructure2.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure2.USART_Parity = USART_PARITY_NO;
	USART_InitStructure2.USART_Mode = USART_MODE_NORMAL;
	USART_Init(UART_COM0_PORT  , &USART_InitStructure2);
	
	USART_InitStructure3.USART_BaudRate = 115200;
	USART_InitStructure3.USART_WordLength = USART_WORDLENGTH_8B;
	USART_InitStructure3.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure3.USART_Parity = USART_PARITY_NO;
	USART_InitStructure3.USART_Mode = USART_MODE_NORMAL;
	USART_Init(UART_COM1_PORT  , &USART_InitStructure3);
	
	/* 设置USART中断标志 */
	USART_IntConfig(UART_COM0_PORT, USART_INT_RXDR, ENABLE);
	
	USART_IntConfig(UART_COM1_PORT, USART_INT_RXDR, ENABLE);
	/* 使能USART接收、发送 */
	USART_TxCmd(UART_COM0_PORT, ENABLE);
	USART_RxCmd(UART_COM0_PORT, ENABLE);
	
	USART_TxCmd(UART_COM1_PORT, ENABLE);
	USART_RxCmd(UART_COM1_PORT, ENABLE);
	/* 初始化中断 */
	NVIC_EnableIRQ(UART_COM0_IRQn);
	NVIC_EnableIRQ(UART_COM1_IRQn);
	
}
 
void UARTx_Init()
{
	UART_CKCU_Config();
	UART_AFIO_Config();
	UARTx_Config();
}
 
// 发送一个字节
void Uart0_Sendbyte(HT_USART_TypeDef* UARTx, u8 Data)
{
	USART_SendData(UART_COM0_PORT, Data);
	// 等待发送数据寄存器清空
	while (USART_GetFlagStatus(UART_COM0_PORT, USART_FLAG_TXDE) == RESET);		
}

void Uart1_Sendbyte(HT_USART_TypeDef* UARTx, u8 Data)
{
	USART_SendData(UART_COM1_PORT, Data);
	// 等待发送数据寄存器清空
	while (USART_GetFlagStatus(UART_COM1_PORT, USART_FLAG_TXDE) == RESET);		
}
 
//发送每个元素数据长度为8位的数组
void Uart0_SendArray(HT_USART_TypeDef* USARTx, u8 *array,u8 num)
{
	u8 i;
	for( i = 0;i < num;i++)
	{
		Uart0_Sendbyte(USARTx,*array);
		array++;
	}
}

void Uart1_SendArray(HT_USART_TypeDef* USARTx, u8 *array,u8 num)
{
	u8 i;
	for( i = 0;i < num;i++)
	{
		Uart1_Sendbyte(USARTx,*array);
		array++;
	}
}
 
// 发送字符串
void Uart0_SendStr(HT_USART_TypeDef* UARTx, uint8_t *str)
{
	uint8_t i;
	for(i = 0;str[i] != '\0';i++)
	{
		Uart0_Sendbyte(UARTx,str[i]);
	}
}

void Uart1_SendStr(HT_USART_TypeDef* UARTx, uint8_t *str)
{
	uint8_t i;
	for(i = 0;str[i] != '\0';i++)
	{
		Uart1_Sendbyte(UARTx,str[i]);
	}
}
 
void UART_IRQHandler(void)
{
	u8 RX_data;
	
	if( USART_GetFlagStatus(UART_COM0_PORT, USART_FLAG_RXDR ) )         //接收中断
	{
		RX_data = USART_ReceiveData(UART_COM0_PORT);                         //读取接收到的数据
		printf("data = %c\n",RX_data);                                  //把收到的数据发送回电脑		
	}
	if( USART_GetFlagStatus(UART_COM1_PORT, USART_FLAG_RXDR ) )         //接收中断
	{
		RX_data = USART_ReceiveData(UART_COM1_PORT);                         //读取接收到的数据
		printf("data = %c\n",RX_data);                                  //把收到的数据发送回电脑		
	}
}


// 重定向c库函数printf到串口，重定向后可使用printf函数
int fputc2(int ch, FILE *f)
{
	/* 发送一个字节数据到串口 */
	USART_SendData(UART_COM0_PORT, (uint8_t) ch);
		
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(UART_COM0_PORT, USART_FLAG_TXDE) == RESET);		
	
	return (ch);
}

int fputc3(int ch, FILE *f)
{
	/* 发送一个字节数据到串口 */
	USART_SendData(UART_COM1_PORT, (uint8_t) ch);
		
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(UART_COM1_PORT, USART_FLAG_TXDE) == RESET);		
	
	return (ch);
}
 
// 重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc2(FILE *f)
{
	/* 等待串口输入数据 */
	while (USART_GetFlagStatus(UART_COM0_PORT, USART_FLAG_RXDNE) == RESET);
 
	return (int)USART_ReceiveData(UART_COM0_PORT);
}

int fgetc3(FILE *f)
{
	/* 等待串口输入数据 */
	while (USART_GetFlagStatus(UART_COM1_PORT, USART_FLAG_RXDNE) == RESET);
 
	return (int)USART_ReceiveData(UART_COM1_PORT);
}
