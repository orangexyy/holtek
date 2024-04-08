#include "Reg_RW.h"
//#include "LDChip.h"
#include "delay.h"
#include "usart.h"
#include "sys.h"
#include "ht32.h"
#include "ht32_board.h"
#include "ht32f5xxxx_01.h"

#define SDCK PBout(10)			//SPI ʱ���ź�
#define SDO  PBin(11)			//SPI �������
#define SDI  PBout(12)			//SPI ��������
#define SCS  PBout(13)			//оƬƬѡ�ź�
#define RSTB PBout(14)			//��λ�˿�
#define IRQ  PBout(15)			//�ж�����

//���ģ��SPI��ʽ��д
#define DELAY_NOP	delay_us(1)

void LD3320_Init(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.AFIO   = 1;
	CKCUClock.Bit.PC     = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	GPIO_DeInit(HT_GPIOC);												//LD3320_SDCK_PIN	���
	GPIO_DirectionConfig    (HT_GPIOC, LD3320_SDCK_PIN, GPIO_DIR_OUT); // ����������� @arg GPIO_DIR_IN  @arg GPIO_DIR_OUT
	GPIO_PullResistorConfig (HT_GPIOC, LD3320_SDCK_PIN, GPIO_PR_DISABLE); //������������ @arg GPIO_PR_UP @arg GPIO_PR_DOWN @arg GPIO_PR_DISABLE
	GPIO_DriveConfig        (HT_GPIOC, LD3320_SDCK_PIN, GPIO_DV_4MA); //���������С 4,8,12,16
	GPIO_InputConfig		(HT_GPIOC, LD3320_SDCK_PIN,ENABLE);

//	GPIO_DirectionConfig	(HT_GPIOC, LD3320_SDI_PIN,GPIO_DIR_IN);		//LD3320_SDI_PIN	����
	GPIO_PullResistorConfig (HT_GPIOC, LD3320_SDI_PIN, GPIO_PR_DISABLE);
	GPIO_DriveConfig		(HT_GPIOC, LD3320_SDI_PIN,GPIO_DV_4MA);
	GPIO_InputConfig		(HT_GPIOC, LD3320_SDI_PIN,ENABLE);
	
//	GPIO_DirectionConfig	(HT_GPIOC, LD3320_SCS_PIN,GPIO_DIR_IN);		//LD3320_SCS_PIN	����
	GPIO_PullResistorConfig (HT_GPIOC, LD3320_SCS_PIN, GPIO_PR_DISABLE);
	GPIO_DriveConfig		(HT_GPIOC, LD3320_SCS_PIN,GPIO_DV_4MA);
	GPIO_InputConfig		(HT_GPIOC, LD3320_SCS_PIN,ENABLE);
	
//	GPIO_DirectionConfig	(HT_GPIOC, LD3320_RSTB_PIN,GPIO_DIR_IN);	//LD3320_RSTB_PIN	����
	GPIO_PullResistorConfig (HT_GPIOC, LD3320_RSTB_PIN, GPIO_PR_DISABLE);
	GPIO_DriveConfig		(HT_GPIOC, LD3320_RSTB_PIN,GPIO_DV_4MA);
	GPIO_InputConfig		(HT_GPIOC, LD3320_RSTB_PIN,ENABLE);
	
	GPIO_DirectionConfig    (HT_GPIOC, LD3320_SDO_PIN, GPIO_DIR_OUT); 	//LD3320_SDO_PIN	���
	GPIO_PullResistorConfig (HT_GPIOC, LD3320_SDO_PIN, GPIO_PR_DISABLE); 
	GPIO_DriveConfig        (HT_GPIOC, LD3320_SDO_PIN, GPIO_DV_4MA);
	GPIO_InputConfig		(HT_GPIOC, LD3320_SDO_PIN,ENABLE);

	GPIO_DirectionConfig    (HT_GPIOC, LD3320_IRQ_PIN, GPIO_DIR_OUT); 	//LD3320_IRQ_PIN	���
	GPIO_PullResistorConfig (HT_GPIOC, LD3320_IRQ_PIN, GPIO_PR_DISABLE); 	
	GPIO_DriveConfig        (HT_GPIOC, LD3320_IRQ_PIN, GPIO_DV_4MA);
	GPIO_InputConfig		(HT_GPIOC, LD3320_IRQ_PIN,ENABLE);

//	GPIO_InitTypeDef  GPIO_InitStructure;
// 	
// 	RCC_APB2PeriphClockCmd(LD3320_SDCK_GPIO_CLK|LD3320_SDO_GPIO_CLK|LD3320_SDI_GPIO_CLK|
//	LD3320_SCS_GPIO_CLK|LD3320_RSTB_GPIO_CLK|LD3320_IRQ_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);	 //ʹ��PA�˿�ʱ��
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//Ҫ�ȿ�ʱ�ӣ�����ӳ�䣻�ر�jtag������swd��
	
// 	GPIO_InitStructure.GPIO_Pin = LD3320_SDCK_PIN;				//�˿�����
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  //�������
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(LD3320_SDCK_GPIO_PORT, &GPIO_InitStructure);	  //��ʼ��IO��
	
//	GPIO_InitStructure.GPIO_Pin = LD3320_SDI_PIN;				//�˿�����
// 	GPIO_Init(LD3320_SDI_GPIO_PORT, &GPIO_InitStructure);	  //��ʼ��IO��
	
//	GPIO_InitStructure.GPIO_Pin = LD3320_SCS_PIN;				//�˿�����
// 	GPIO_Init(LD3320_SCS_GPIO_PORT, &GPIO_InitStructure);	  //��ʼ��IO��
	
//	GPIO_InitStructure.GPIO_Pin = LD3320_RSTB_PIN;				//�˿�����
// 	GPIO_Init(LD3320_RSTB_GPIO_PORT, &GPIO_InitStructure);	  //��ʼ��IO��
	
//	GPIO_InitStructure.GPIO_Pin = LD3320_SDO_PIN;				//�˿�����
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		  //�������
// 	GPIO_Init(LD3320_SDO_GPIO_PORT, &GPIO_InitStructure);	  //��ʼ��IO��

//	GPIO_InitStructure.GPIO_Pin = LD3320_IRQ_PIN;				//�˿�����
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		  //�������
// 	GPIO_Init(LD3320_IRQ_GPIO_PORT, &GPIO_InitStructure);	  //��ʼ��IO��
}

void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.AFIO   = 1;
	CKCUClock.Bit.EXTI    = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
//	GPIO_EXTILineConfig(LD3320_IRQEXIT_PORTSOURCE,LD3320_IRQPINSOURCE);
	
	EXTI_IntConfig(LD3320_IRQEXITLINE,ENABLE);
	
	
	EXTI_InitStructure.EXTI_Channel=LD3320_IRQEXITLINE;
	EXTI_InitStructure.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;	
	EXTI_InitStructure.EXTI_IntType = EXTI_LOW_LEVEL;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_EnableIRQ(LD3320_IRQN);
	NVIC_SetPriority(LD3320_IRQN,2);
	
//	EXTI_InitStructure.EXTI_Line=LD3320_IRQEXITLINE;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

//	NVIC_InitStructure.NVIC_IRQChannel = LD3320_IRQN;			//ʹ�ܰ���KEY2���ڵ��ⲿ�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
//	NVIC_Init(&NVIC_InitStructure);
}


void LD_WriteReg(unsigned char address,unsigned char dataout)
{
	unsigned char i = 0;
	unsigned char command=0x04;
	SCS = 0;
	DELAY_NOP;

	//write command
	for (i=0;i < 8; i++)
	{
		if (command & 0x80) 
			SDI = 1;
		else
			SDI = 0;
		
		DELAY_NOP;
		SDCK = 0;
		command = (command << 1);  
		DELAY_NOP;
		SDCK = 1;  
	}
	//write address
	for (i=0;i < 8; i++)
	{
		if (address & 0x80) 
			SDI = 1;
		else
			SDI = 0;
		DELAY_NOP;
		SDCK = 0;
		address = (address << 1); 
		DELAY_NOP;
		SDCK = 1;  
	}
	//write data
	for (i=0;i < 8; i++)
	{
		if (dataout & 0x80) 
			SDI = 1;
		else
			SDI = 0;
		DELAY_NOP;
		SDCK = 0;
		dataout = (dataout << 1); 
		DELAY_NOP;
		SDCK = 1;  
	}
	DELAY_NOP;
	SCS = 1;
}

unsigned char LD_ReadReg(unsigned char address)
{
	unsigned char i = 0; 
	unsigned char datain =0 ;
	unsigned char temp = 0; 
	unsigned char command=0x05;
	PBout(13) = 0;
	DELAY_NOP;

	//write command
	for (i=0;i < 8; i++)
	{
		if (command & 0x80) 
			SDI = 1;
		else
			SDI = 0;
		DELAY_NOP;
		SDCK = 0;
		command = (command << 1);  
		DELAY_NOP;
		SDCK = 1;  
	}

	//write address
	for (i=0;i < 8; i++)
	{
		if (address & 0x80) 
			SDI = 1;
		else
			SDI = 0;
		DELAY_NOP;
		SDCK = 0;
		address = (address << 1); 
		DELAY_NOP;
		SDCK = 1;  
	}
	DELAY_NOP;

	//Read
	for (i=0;i < 8; i++)
	{
		datain = (datain << 1);
		temp = SDO;
		DELAY_NOP;
		SDCK = 0;  
		if (temp == 1)  
			datain |= 0x01; 
		DELAY_NOP;
		SDCK = 1;  
	}

	DELAY_NOP;
	SCS = 1;
	return datain;
}

void EXTI4_15_IRQHandler(void)
{
	if(EXTI_GetEdgeStatus(LD3320_IRQEXITLINE,EXTI_EDGE_NEGATIVE)!= RESET ) 
	{
//		ProcessInt(); 
 		printf("�����ж�\r\n");	
		EXTI_ClearEdgeFlag(LD3320_IRQEXITLINE);
//		EXTI_ClearITPendingBit(LD3320_IRQEXITLINE);//���LINE�ϵ��жϱ�־λ  
	} 
}
