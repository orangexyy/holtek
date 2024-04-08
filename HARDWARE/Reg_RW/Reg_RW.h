#ifndef REG_RW_H
#define REG_RW_H
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


///LD3320引脚相关定义
#define LD3320_SDCK_GPIO_CLK			CKCUClock
#define LD3320_SDCK_GPIO_PORT			HT_GPIOC
#define LD3320_SDCK_PIN					  	GPIO_PIN_10

#define LD3320_SDO_GPIO_CLK				CKCUClock
#define LD3320_SDO_GPIO_PORT			HT_GPIOC
#define LD3320_SDO_PIN						GPIO_PIN_11
	
#define LD3320_SDI_GPIO_CLK				CKCUClock
#define LD3320_SDI_GPIO_PORT			HT_GPIOC
#define LD3320_SDI_PIN						GPIO_PIN_12

#define LD3320_SCS_GPIO_CLK				CKCUClock
#define LD3320_SCS_GPIO_PORT			HT_GPIOC
#define LD3320_SCS_PIN						GPIO_PIN_13		

#define LD3320_RSTB_GPIO_CLK		  	CKCUClock
#define LD3320_RSTB_GPIO_PORT			HT_GPIOC
#define LD3320_RSTB_PIN						GPIO_PIN_14		

#define LD3320_IRQ_GPIO_CLK				CKCUClock
#define LD3320_IRQ_GPIO_PORT			HT_GPIOC
#define LD3320_IRQ_PIN						GPIO_PIN_15

#define LD3320_IRQEXIT_PORTSOURCE		GPIO_PC
#define LD3320_IRQPINSOURCE					GPIO_PIN_15
#define LD3320_IRQEXITLINE					EXTI_CHANNEL_15
#define LD3320_IRQN								EXTI4_15_IRQn

#define READ_SDO()   GPIO_ReadInBit(LD3320_SDO_GPIO_PORT, LD3320_SDO_PIN)



//函数声明
void LD3320_Init(void);
void EXTIX_Init(void);
void LD_WriteReg( unsigned char address, unsigned char dataout );
unsigned char LD_ReadReg( unsigned char address );

void EXTI4_15_IRQHandler(void);


#endif
