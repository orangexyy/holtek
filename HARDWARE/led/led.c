#include "ht32.h"
#include "led.h"

void LED_Init(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
	CKCUClock.Bit.PC    = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	GPIO_DeInit(HT_GPIOC);
	GPIO_DirectionConfig(LED1_PORT, LED1_PIN, GPIO_DIR_OUT);
	GPIO_DirectionConfig(LED1_PORT, LED2_PIN, GPIO_DIR_OUT);

	GPIO_ClearOutBits       (LED1_PORT, LED1_PIN);
	GPIO_ClearOutBits       (LED1_PORT, LED2_PIN);

}

void LED1_ON(void)		//øÕÃ¸µ∆
{
	GPIO_ClearOutBits(LED1_PORT ,LED1_PIN);
}

void LED1_OFF(void)
{
	GPIO_SetOutBits(LED1_PORT ,LED1_PIN);
}

void LED1_Turn(void)
{
	if(GPIO_ReadOutBit(LED1_PORT, LED1_PIN) == 0)
	{
		GPIO_SetOutBits(LED1_PORT, LED1_PIN);
	}
	else
	{
		GPIO_ClearOutBits(LED1_PORT, LED1_PIN);
	}
}

void LED2_ON(void)		//Œ‘ “µ∆
{
	GPIO_ClearOutBits(LED2_PORT, LED2_PIN);
}


void LED2_OFF(void)
{
	GPIO_SetOutBits(LED2_PORT, LED2_PIN);
}

void LED2_Turn(void)
{
	if(GPIO_ReadOutBit(LED2_PORT, LED2_PIN) == 0)
	{
		GPIO_SetOutBits(LED2_PORT, LED2_PIN);
	}
	else
	{
		GPIO_ClearOutBits(LED2_PORT, LED2_PIN);
	}
}
