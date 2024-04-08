#include "uart.h"

//����0 UART - 0
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

//����1 UART - 1
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
	
	/*USART��ʼ��
	*�����ʣ�115200
	*�ֽڳ��ȣ�8λ
	*ֹͣλ��1λ
	*У��λ����			
	*ģʽ������ģʽ
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
	
	/* ����USART�жϱ�־ */
	USART_IntConfig(UART_COM0_PORT, USART_INT_RXDR, ENABLE);
	
	USART_IntConfig(UART_COM1_PORT, USART_INT_RXDR, ENABLE);
	/* ʹ��USART���ա����� */
	USART_TxCmd(UART_COM0_PORT, ENABLE);
	USART_RxCmd(UART_COM0_PORT, ENABLE);
	
	USART_TxCmd(UART_COM1_PORT, ENABLE);
	USART_RxCmd(UART_COM1_PORT, ENABLE);
	/* ��ʼ���ж� */
	NVIC_EnableIRQ(UART_COM0_IRQn);
	NVIC_EnableIRQ(UART_COM1_IRQn);
	
}
 
void UARTx_Init()
{
	UART_CKCU_Config();
	UART_AFIO_Config();
	UARTx_Config();
}
 
// ����һ���ֽ�
void Uart0_Sendbyte(HT_USART_TypeDef* UARTx, u8 Data)
{
	USART_SendData(UART_COM0_PORT, Data);
	// �ȴ��������ݼĴ������
	while (USART_GetFlagStatus(UART_COM0_PORT, USART_FLAG_TXDE) == RESET);		
}

void Uart1_Sendbyte(HT_USART_TypeDef* UARTx, u8 Data)
{
	USART_SendData(UART_COM1_PORT, Data);
	// �ȴ��������ݼĴ������
	while (USART_GetFlagStatus(UART_COM1_PORT, USART_FLAG_TXDE) == RESET);		
}
 
//����ÿ��Ԫ�����ݳ���Ϊ8λ������
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
 
// �����ַ���
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
	
	if( USART_GetFlagStatus(UART_COM0_PORT, USART_FLAG_RXDR ) )         //�����ж�
	{
		RX_data = USART_ReceiveData(UART_COM0_PORT);                         //��ȡ���յ�������
		printf("data = %c\n",RX_data);                                  //���յ������ݷ��ͻص���		
	}
	if( USART_GetFlagStatus(UART_COM1_PORT, USART_FLAG_RXDR ) )         //�����ж�
	{
		RX_data = USART_ReceiveData(UART_COM1_PORT);                         //��ȡ���յ�������
		printf("data = %c\n",RX_data);                                  //���յ������ݷ��ͻص���		
	}
}


// �ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc2(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ����� */
	USART_SendData(UART_COM0_PORT, (uint8_t) ch);
		
	/* �ȴ�������� */
	while (USART_GetFlagStatus(UART_COM0_PORT, USART_FLAG_TXDE) == RESET);		
	
	return (ch);
}

int fputc3(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ����� */
	USART_SendData(UART_COM1_PORT, (uint8_t) ch);
		
	/* �ȴ�������� */
	while (USART_GetFlagStatus(UART_COM1_PORT, USART_FLAG_TXDE) == RESET);		
	
	return (ch);
}
 
// �ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc2(FILE *f)
{
	/* �ȴ������������� */
	while (USART_GetFlagStatus(UART_COM0_PORT, USART_FLAG_RXDNE) == RESET);
 
	return (int)USART_ReceiveData(UART_COM0_PORT);
}

int fgetc3(FILE *f)
{
	/* �ȴ������������� */
	while (USART_GetFlagStatus(UART_COM1_PORT, USART_FLAG_RXDNE) == RESET);
 
	return (int)USART_ReceiveData(UART_COM1_PORT);
}
