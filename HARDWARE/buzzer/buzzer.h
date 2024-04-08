#ifndef _BUZZER_H
#define _BUZZER_H

void Buzzer_Init(void);
void Buzzer_ON(void);
void Buzzer_OFF(void);

#define Buzzer_PORT		HT_GPIOC
#define Buzzer_PIN		GPIO_PIN_12

#endif
