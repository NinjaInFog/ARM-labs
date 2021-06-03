#include "stm32f4xx.h"
#include "function.h"

int main(void)
{
	volatile uint32 state = 0;
	volatile uint32 lastState = 0;
	volatile uint32 switcher = 0;
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	
	GPIOA->MODER |= GPIO_MODER_MODE1_0;
	GPIOA->MODER &= ~GPIO_MODER_MODE1_1;
	
	GPIOA->MODER &= ~GPIO_MODER_MODE4_0;
	GPIOA->MODER &= ~GPIO_MODER_MODE4_1;
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	
	GPIOB->MODER |= GPIO_MODER_MODE8_0;
	GPIOB->MODER &= ~GPIO_MODER_MODE8_1;
	
	
	////////////////////////////////////
	GPIOB->MODER |= GPIO_MODER_MODE3_0;
	GPIOB->MODER &= ~GPIO_MODER_MODE3_1;
	
	GPIOB->MODER |= GPIO_MODER_MODE4_0;
	GPIOB->MODER &= ~GPIO_MODER_MODE4_1;
	
	GPIOB->MODER |= GPIO_MODER_MODE5_0;
	GPIOB->MODER &= ~GPIO_MODER_MODE5_1;
	
	GPIOB->MODER |= GPIO_MODER_MODE10_0;
	GPIOB->MODER &= ~GPIO_MODER_MODE10_1;
	////////////////////////////////////
	
	while(1)
	{
		state = debounce(lastState);    // функція для дребезгу
		if((state != lastState && state)){
			switcher = !switcher; 		// swithcer має два стани: 0 - обертається в одному напрямку
										//, 1 - обертається в іншому напрямку
		}
		lastState = state;
		if(switcher){						   		 // обертаємо в одному напрямку
			GPIOB->ODR |= GPIO_ODR_OD8;				 // вмикаємо синій світлодіод
			GPIOA->ODR &= ~GPIO_ODR_OD1;			 // вимикаємо червоний світлодіод
			stepper(1);								 // викликаємо функцію яка буде обертати в одному напрямку(1)
		}
		else{ 										// інакше в іншому напрямку
			GPIOB->ODR &= ~GPIO_ODR_OD8;            //вимикаємо синій світлодіод
			GPIOA->ODR |= GPIO_ODR_OD1;             // вмикаємо червоний світлодіод
			stepper(0);								// викликаємо функцію яка буде обертати в іншому напрямку(0)
		}
	}
}	