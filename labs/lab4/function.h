#ifndef functions_h
#define functions_h
#include "stm32f4xx.h"

typedef unsigned long uint32;

void stepper(uint32 dir);

uint32 debounce(uint32 LastState);

void delay(uint32 time);

#endif