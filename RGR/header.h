#ifndef HEADER_H
#define HEADER_H
#include "stm32f4xx.h"
typedef unsigned long uint32;
typedef unsigned long long uint64;

void GPIO_Config(void);
void TIM3_Config(void);
void TIM3_IRQHandler(void);
void TIM4_Config(void);
uint32 angle(uint32 range);

extern volatile uint32 pulse_width;
extern volatile uint32 last_captured;
extern volatile uint32 signal_polarity; 
extern volatile uint32 Distance;
#endif
