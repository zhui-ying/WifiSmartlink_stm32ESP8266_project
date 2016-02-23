#ifndef __INTERFACE_H_
#define __INTERFACE_H_

#include "stm32f10x.h"

//user LED PG15
#define LED_PIN           GPIO_Pin_15
#define LED_GPIO        GPIOG
#define LED_SET         GPIO_SetBits(LED_GPIO , LED_PIN)
#define LED_RESET       GPIO_ResetBits(LED_GPIO , LED_PIN)

void delay_init(void);
void Delayms(u32 Nms);
void Delay_us(u32 Nus);
void LEDToggle(uint16_t Led);

void TIM2_Init(void);
void UserLEDInit(void);

#endif

