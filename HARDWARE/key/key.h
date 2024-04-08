#ifndef __KEY_H
#define __KEY_H


void Key_Init(void);
uint8_t Key_GetNum(void);

#define KEY1_PORT	HT_GPIOD
#define KEY1_PIN	GPIO_PIN_1

#define KEY2_PORT	HT_GPIOD
#define KEY2_PIN	GPIO_PIN_2

#endif
