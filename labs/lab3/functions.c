#include "functions.h"
void stepper(uint32 dir){

volatile int i = 0;
static int Steps;
	
switch(Steps){

case 0:
	*GPIOB_BSSR |= (1 << (IN1));
	*GPIOB_BSSR |= (1 << (IN2 + 16));
	*GPIOB_BSSR |= (1 << (IN3 + 16));
	*GPIOB_BSSR |= (1 << (IN4));
	for(i = 0; i < 5000; i++);
break;

case 1:
	*GPIOB_BSSR |= (1 << (IN1));
	*GPIOB_BSSR |= (1 << (IN2));
	*GPIOB_BSSR |= (1 << (IN3 + 16));
	*GPIOB_BSSR |= (1 << (IN4 + 16));				
	for(i = 0; i < 5000; i++);
break;

case 2:
	*GPIOB_BSSR |= (1 << (IN1 + 16));
	*GPIOB_BSSR |= (1 << (IN2));
	*GPIOB_BSSR |= (1 << (IN3));
	*GPIOB_BSSR |= (1 << (IN4 + 16));		
	for(i = 0; i < 5000; i++);
break;

case 3:
	*GPIOB_BSSR |= (1 << (IN1 + 16));
	*GPIOB_BSSR |= (1 << (IN2 + 16));
	*GPIOB_BSSR |= (1 << (IN3));
	*GPIOB_BSSR |= (1 << (IN4));
	for(i = 0; i < 5000; i++);
break;
	}
	if(dir)     //якщо обертання вперед то збільшуємо steps
		Steps++;
	else 		//інакше зменшуємо
		Steps--;
	
	if(Steps > 3)	//якщо перевищило кількість кроків то Обнуляємо
		Steps = 0;
	if(Steps<0)		//якщо перевищило кількість кроків у зворотню
					//сторону то встановлюємо в максимальне значення
		Steps = 3;
}