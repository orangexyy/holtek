#include "ht32.h"
#include "Key.h"
#include "Delay.h"


void Key_Init(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
	CKCUClock.Bit.PD    = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	GPIO_DeInit(HT_GPIOD);
	GPIO_DirectionConfig(KEY1_PORT, KEY1_PIN, GPIO_DIR_IN);
	GPIO_DirectionConfig(KEY2_PORT, KEY2_PIN, GPIO_DIR_IN);
//	GPIO_PullResistorConfig(KEY1_PORT, KEY1_PIN, GPIO_PR_UP);
//	GPIO_PullResistorConfig(KEY2_PORT, KEY2_PIN, GPIO_PR_UP);

	GPIO_ClearOutBits(KEY1_PORT, KEY1_PIN);
	GPIO_ClearOutBits(KEY2_PORT,KEY2_PIN);

}

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	if(GPIO_ReadInBit(KEY1_PORT, KEY1_PIN) == 1)
	{
		delay_ms(20);
		while(GPIO_ReadInBit(KEY1_PORT,KEY1_PIN) == 1);
		delay_ms(20);
		KeyNum = 1;
	}
	
	if(GPIO_ReadInBit(KEY2_PORT, KEY2_PIN) == 1)
	{
		delay_ms(20);
		while(GPIO_ReadInBit(KEY2_PORT, KEY2_PIN) == 1);
		delay_ms(20);
		KeyNum = 2;
	}
	
	return KeyNum;
}
