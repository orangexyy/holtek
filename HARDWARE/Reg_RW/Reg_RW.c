#include "Reg_RW.h"
//#include "LDChip.h"
#include "delay.h"
#include "usart.h"
#include "sys.h"
#include "ht32.h"
#include "ht32_board.h"
#include "ht32f5xxxx_01.h"

#define SDCK PBout(10)			//SPI 时钟信号
#define SDO  PBin(11)			//SPI 数据输出
#define SDI  PBout(12)			//SPI 数据输入
#define SCS  PBout(13)			//芯片片选信号
#define RSTB PBout(14)			//复位端口
#define IRQ  PBout(15)			//中断引脚

//软件模拟SPI方式读写
#define DELAY_NOP	delay_us(1)

void LD3320_Init(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.AFIO   = 1;
	CKCUClock.Bit.PC     = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	GPIO_DeInit(HT_GPIOC);												//LD3320_SDCK_PIN	输出
	GPIO_DirectionConfig    (HT_GPIOC, LD3320_SDCK_PIN, GPIO_DIR_OUT); // 输入输出方向 @arg GPIO_DIR_IN  @arg GPIO_DIR_OUT
	GPIO_PullResistorConfig (HT_GPIOC, LD3320_SDCK_PIN, GPIO_PR_DISABLE); //上拉下拉电阻 @arg GPIO_PR_UP @arg GPIO_PR_DOWN @arg GPIO_PR_DISABLE
	GPIO_DriveConfig        (HT_GPIOC, LD3320_SDCK_PIN, GPIO_DV_4MA); //输出电流大小 4,8,12,16
	GPIO_InputConfig		(HT_GPIOC, LD3320_SDCK_PIN,ENABLE);

//	GPIO_DirectionConfig	(HT_GPIOC, LD3320_SDI_PIN,GPIO_DIR_IN);		//LD3320_SDI_PIN	输入
	GPIO_PullResistorConfig (HT_GPIOC, LD3320_SDI_PIN, GPIO_PR_DISABLE);
	GPIO_DriveConfig		(HT_GPIOC, LD3320_SDI_PIN,GPIO_DV_4MA);
	GPIO_InputConfig		(HT_GPIOC, LD3320_SDI_PIN,ENABLE);
	
//	GPIO_DirectionConfig	(HT_GPIOC, LD3320_SCS_PIN,GPIO_DIR_IN);		//LD3320_SCS_PIN	输入
	GPIO_PullResistorConfig (HT_GPIOC, LD3320_SCS_PIN, GPIO_PR_DISABLE);
	GPIO_DriveConfig		(HT_GPIOC, LD3320_SCS_PIN,GPIO_DV_4MA);
	GPIO_InputConfig		(HT_GPIOC, LD3320_SCS_PIN,ENABLE);
	
//	GPIO_DirectionConfig	(HT_GPIOC, LD3320_RSTB_PIN,GPIO_DIR_IN);	//LD3320_RSTB_PIN	输入
	GPIO_PullResistorConfig (HT_GPIOC, LD3320_RSTB_PIN, GPIO_PR_DISABLE);
	GPIO_DriveConfig		(HT_GPIOC, LD3320_RSTB_PIN,GPIO_DV_4MA);
	GPIO_InputConfig		(HT_GPIOC, LD3320_RSTB_PIN,ENABLE);
	
	GPIO_DirectionConfig    (HT_GPIOC, LD3320_SDO_PIN, GPIO_DIR_OUT); 	//LD3320_SDO_PIN	输出
	GPIO_PullResistorConfig (HT_GPIOC, LD3320_SDO_PIN, GPIO_PR_DISABLE); 
	GPIO_DriveConfig        (HT_GPIOC, LD3320_SDO_PIN, GPIO_DV_4MA);
	GPIO_InputConfig		(HT_GPIOC, LD3320_SDO_PIN,ENABLE);

	GPIO_DirectionConfig    (HT_GPIOC, LD3320_IRQ_PIN, GPIO_DIR_OUT); 	//LD3320_IRQ_PIN	输出
	GPIO_PullResistorConfig (HT_GPIOC, LD3320_IRQ_PIN, GPIO_PR_DISABLE); 	
	GPIO_DriveConfig        (HT_GPIOC, LD3320_IRQ_PIN, GPIO_DV_4MA);
	GPIO_InputConfig		(HT_GPIOC, LD3320_IRQ_PIN,ENABLE);

//	GPIO_InitTypeDef  GPIO_InitStructure;
// 	
// 	RCC_APB2PeriphClockCmd(LD3320_SDCK_GPIO_CLK|LD3320_SDO_GPIO_CLK|LD3320_SDI_GPIO_CLK|
//	LD3320_SCS_GPIO_CLK|LD3320_RSTB_GPIO_CLK|LD3320_IRQ_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);	 //使能PA端口时钟
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//要先开时钟，再重映射；关闭jtag，保留swd。
	
// 	GPIO_InitStructure.GPIO_Pin = LD3320_SDCK_PIN;				//端口配置
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  //推挽输出
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(LD3320_SDCK_GPIO_PORT, &GPIO_InitStructure);	  //初始化IO口
	
//	GPIO_InitStructure.GPIO_Pin = LD3320_SDI_PIN;				//端口配置
// 	GPIO_Init(LD3320_SDI_GPIO_PORT, &GPIO_InitStructure);	  //初始化IO口
	
//	GPIO_InitStructure.GPIO_Pin = LD3320_SCS_PIN;				//端口配置
// 	GPIO_Init(LD3320_SCS_GPIO_PORT, &GPIO_InitStructure);	  //初始化IO口
	
//	GPIO_InitStructure.GPIO_Pin = LD3320_RSTB_PIN;				//端口配置
// 	GPIO_Init(LD3320_RSTB_GPIO_PORT, &GPIO_InitStructure);	  //初始化IO口
	
//	GPIO_InitStructure.GPIO_Pin = LD3320_SDO_PIN;				//端口配置
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		  //推挽输出
// 	GPIO_Init(LD3320_SDO_GPIO_PORT, &GPIO_InitStructure);	  //初始化IO口

//	GPIO_InitStructure.GPIO_Pin = LD3320_IRQ_PIN;				//端口配置
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		  //推挽输出
// 	GPIO_Init(LD3320_IRQ_GPIO_PORT, &GPIO_InitStructure);	  //初始化IO口
}

void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.AFIO   = 1;
	CKCUClock.Bit.EXTI    = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
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
//	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

//	NVIC_InitStructure.NVIC_IRQChannel = LD3320_IRQN;			//使能按键KEY2所在的外部中断通道
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
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
 		printf("进入中断\r\n");	
		EXTI_ClearEdgeFlag(LD3320_IRQEXITLINE);
//		EXTI_ClearITPendingBit(LD3320_IRQEXITLINE);//清除LINE上的中断标志位  
	} 
}
