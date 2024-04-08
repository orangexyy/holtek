#include "ht32.h"
#include "Buzzer.h"

void Buzzer_Init(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
	CKCUClock.Bit.PC    = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	GPIO_DeInit(HT_GPIOC);
	GPIO_DirectionConfig(Buzzer_PORT, Buzzer_PIN, GPIO_DIR_OUT);
	GPIO_PullResistorConfig(Buzzer_PORT, Buzzer_PIN, GPIO_PR_UP);

	GPIO_ClearOutBits(Buzzer_PORT, Buzzer_PIN);

}

void Buzzer_ON(void)		
{
	GPIO_ClearOutBits(Buzzer_PORT, Buzzer_PIN);
}

void Buzzer_OFF(void)
{
	GPIO_SetOutBits(Buzzer_PORT, Buzzer_PIN);
}


