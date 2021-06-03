#include "function.h"
// функція обертання двигуна
void stepper(uint32 dir){

static int Steps;
	
switch(Steps){

case 0:
	GPIOB->ODR |= GPIO_ODR_OD3;
	GPIOB->ODR &= ~GPIO_ODR_OD5;
	GPIOB->ODR &= ~GPIO_ODR_OD4;
	GPIOB->ODR |= GPIO_ODR_OD10;
	delay(3000);
break;

case 1:
	GPIOB->ODR |= GPIO_ODR_OD3;
	GPIOB->ODR |= GPIO_ODR_OD5;
	GPIOB->ODR &= ~GPIO_ODR_OD4;
	GPIOB->ODR &= ~GPIO_ODR_OD10;	
	delay(3000);
break;

case 2:
	GPIOB->ODR &= ~GPIO_ODR_OD3;
	GPIOB->ODR |= GPIO_ODR_OD5;
	GPIOB->ODR |= GPIO_ODR_OD4;
	GPIOB->ODR &= ~GPIO_ODR_OD10;
	delay(3000);
break;

case 3:
	GPIOB->ODR &= ~GPIO_ODR_OD3;
	GPIOB->ODR &= ~GPIO_ODR_OD5;
	GPIOB->ODR |= GPIO_ODR_OD4;
	GPIOB->ODR |= GPIO_ODR_OD10;
	delay(3000);
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

uint32 debounce(uint32 LastState) {
	uint32 state = (GPIOA->IDR & GPIO_IDR_ID4); //зчитуємо стан з GPIOA pin 4(BUTTON)
	if(LastState != state)
	{
		delay(50);								//чекаємо деякий час
		state = (GPIOA->IDR & GPIO_IDR_ID4);	//зчитуємо ще раз
	}
	return state;
}


void delay(uint32 time) //функція затримки
{
	volatile uint32 i = 0;
	for(i = 0; i < time; i++);
}

