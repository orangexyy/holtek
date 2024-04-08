#ifndef _LED_H_
#define _LED_H_

#include "ht32f5xxxx_ckcu.h"
#include "ht32f5xxxx_gpio.h"

void LED_Init(void);
void LED1_ON(void);
void LED1_OFF(void);
void LED1_Turn(void);
void LED2_ON(void);
void LED2_OFF(void);
void LED2_Turn(void);

#define LED1_PORT	HT_GPIOC
#define LED1_PIN	GPIO_PIN_14

#define LED2_PORT	HT_GPIOC
#define LED2_PIN	GPIO_PIN_15

#endif

