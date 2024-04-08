#include "usart.h"
#include "timer.h"
#include <stdio.h>
#include <stdarg.h>

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

//����1 USART - 1
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
	
	/*USART��ʼ��///////USART0
	*�����ʣ�115200
	*�ֽڳ��ȣ�8λ
	*ֹͣλ��1λ
	*У��λ����			
	*ģʽ������ģʽ
	**/
	USART_InitStructure0.USART_BaudRate = 9600;
	USART_InitStructure0.USART_WordLength = USART_WORDLENGTH_8B;
	USART_InitStructure0.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure0.USART_Parity = USART_PARITY_NO;
	USART_InitStructure0.USART_Mode = USART_MODE_NORMAL;
	USART_Init(USART_COM0_PORT  , &USART_InitStructure0);
	
	/*USART��ʼ��///////USART1
	*�����ʣ�115200
	*�ֽڳ��ȣ�8λ
	*ֹͣλ��1λ
	*У��λ����			
	*ģʽ������ģʽ
	**/
	USART_InitStructure1.USART_BaudRate = 115200;
	USART_InitStructure1.USART_WordLength = USART_WORDLENGTH_8B;
	USART_InitStructure1.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure1.USART_Parity = USART_PARITY_NO;
	USART_InitStructure1.USART_Mode = USART_MODE_NORMAL;
	USART_Init(USART_COM1_PORT  , &USART_InitStructure1);
	
	/* ����USART�жϱ�־ */
	USART_IntConfig(USART_COM0_PORT, USART_INT_RXDR, ENABLE);
	USART_IntConfig(USART_COM1_PORT, USART_INT_RXDR, ENABLE);
	
	/* ʹ��USART���ա����� */
	USART_TxCmd(USART_COM0_PORT, ENABLE);
	USART_RxCmd(USART_COM0_PORT, ENABLE);
	
	USART_TxCmd(USART_COM1_PORT, ENABLE);
	USART_RxCmd(USART_COM1_PORT, ENABLE);
	/* ��ʼ���ж� */
	NVIC_EnableIRQ(USART_COM0_IRQn);
	NVIC_EnableIRQ(USART_COM1_IRQn);
}

void USARTx_Init()
{
	USART_CKCU_Config();
	USART_AFIO_Config();
	USARTx_Config();
}
 
// ����һ���ֽ�
void Usart0_Sendbyte(HT_USART_TypeDef* USARTx, u8 Data)
{
	USART_SendData(USART_COM0_PORT, Data);
	// �ȴ��������ݼĴ������
	while (USART_GetFlagStatus(USART_COM0_PORT, USART_FLAG_TXDE) == RESET);		
}
 
void Usart1_Sendbyte(HT_USART_TypeDef* USARTx, u8 Data)
{
	USART_SendData(USART_COM1_PORT, Data);
	// �ȴ��������ݼĴ������
	while (USART_GetFlagStatus(USART_COM1_PORT, USART_FLAG_TXDE) == RESET);		
}

//����ÿ��Ԫ�����ݳ���Ϊ8λ������
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
 
// �����ַ���
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


/////////����USART0////////
#if EN_USART0_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
u8 USART0_RX_BUF[USART0_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
u16 USART0_RX_STA = 0;     //����״̬���

void USART0_IRQHandler(void)		//�жϽ�������
{
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	u8 RX0_data;
	if( USART_GetFlagStatus(USART_COM0_PORT, USART_FLAG_RXDR ) )         //�����ж�
	{
		RX0_data = USART_ReceiveData(USART_COM0_PORT);      		//��ȡ���յ�������
		if((USART0_RX_STA & 0x8000) == 0) //����δ���
		{
			if(USART0_RX_STA < USART0_REC_LEN)	//�����Խ�������
			{
				TM_SetCounter(HT_GPTM0, 0); //���������          				//���������
				if(USART0_RX_STA == 0) 				//ʹ�ܶ�ʱ��7���ж�
				{
					TM_Cmd(HT_GPTM0, ENABLE);; //ʹ�ܶ�ʱ��
				}
				USART0_RX_BUF[USART0_RX_STA++] = RX0_data;	//��¼���յ���ֵ
			}
			else
			{
				USART0_RX_STA |= 1 << 15;				//ǿ�Ʊ�ǽ������
			}
		}		
	}

//	if( USART_GetFlagStatus(COM1_PORT, USART_FLAG_RXDR ) )         //�����ж�
//	{
//		RX_data = USART_ReceiveData(COM1_PORT);                         //��ȡ���յ�������
//		printf("data = %c\n",RX_data);                                  //���յ������ݷ��ͻص���		
//	}
	
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 
#endif	


/////////����USART1////////
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART1_RX_BUF[USART1_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART1_RX_STA=0;       //����״̬���	 

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 RX1_data;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetFlagStatus(USART_COM1_PORT, USART_FLAG_RXDR ))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		RX1_data =USART_ReceiveData(USART_COM1_PORT);	//��ȡ���յ�������
		
		if((USART1_RX_STA&0x8000)==0)//����δ���
			{
			if(USART1_RX_STA&0x4000)//���յ���0x0d
				{
				if(RX1_data!=0x0a)USART1_RX_STA=0;//���մ���,���¿�ʼ
				else USART1_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(RX1_data==0x0d)USART1_RX_STA|=0x4000;
				else
					{
					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=RX1_data ;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
		} 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 
#endif	

//void COM1_IRQHandler(void)
//{
//	u8 data;
//	
//	if( USART_GetFlagStatus(COM1_PORT, USART_FLAG_RXDR ) )         //�����ж�
//	{
//		data = USART_ReceiveData(COM1_PORT);                         //��ȡ���յ�������
//		printf("data = %c\n",data);                                  //���յ������ݷ��ͻص���		
//	}
//}


// �ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc0(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ����� */
	USART_SendData(USART_COM0_PORT, (uint8_t) ch);
		
	/* �ȴ�������� */
	while (USART_GetFlagStatus(USART_COM0_PORT, USART_FLAG_TXDE) == RESET);		
	
	return (ch);
}

int fputc1(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ����� */
	USART_SendData(USART_COM1_PORT, (uint8_t) ch);
		
	/* �ȴ�������� */
	while (USART_GetFlagStatus(USART_COM1_PORT, USART_FLAG_TXDE) == RESET);		
	
	return (ch);
}
 
// �ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc0(FILE *f)
{
	/* �ȴ������������� */
	while (USART_GetFlagStatus(USART_COM0_PORT, USART_FLAG_RXDNE) == RESET);
 
	return (int)USART_ReceiveData(USART_COM0_PORT);
}

int fgetc1(FILE *f)
{
	/* �ȴ������������� */
	while (USART_GetFlagStatus(USART_COM1_PORT, USART_FLAG_RXDNE) == RESET);
 
	return (int)USART_ReceiveData(USART_COM1_PORT);
}

