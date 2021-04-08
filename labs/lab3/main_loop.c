#include "functions.h"

void main_loop()
{
	volatile uint64 counter = 0;
	volatile uint32 state = 0;
	volatile uint32 lastState = 0;
	volatile uint32 switcher = 0;
	GPIOA_RCC_Init;
	GPIOB_RCC_Init;
	//////////////GPIOA pin 1(LED RED) mod output/////////////
	*GPIOA_MODER &= ~(1 << (GPIOA_LEDR * 2 + 1));	//
	*GPIOA_MODER |=  (1 << (GPIOA_LEDR * 2));
	/////////////////////////////////////////////////
	
	//////////////GPIOB pin 8(LED BLUE) mod output//////////////
	*GPIOB_MODER &= ~(1 << (GPIOB_LEDB * 2 + 1));	//
	*GPIOB_MODER |=  (1 << (GPIOB_LEDB * 2));
	/////////////////////////////////////////////////
	
	//////////////GPIOA pin 4(BUTTON) mod INPUT//////////////
	*GPIOA_MODER &= ~(1 << (GPIOA_BUTTON * 2));		//
	*GPIOA_MODER &= ~(1 << (GPIOA_BUTTON * 2 + 1));
	/////////////////////////////////////////////////
	
	//////////////GPIOB pin 3 mod output//////////////
	*GPIOB_MODER &= ~(1 << (IN1 * 2 + 1));	//
	*GPIOB_MODER |=  (1 << (IN1 * 2));
	/////////////////////////////////////////////////
	
	//////////////GPIOB pin 5 mod output//////////////
	*GPIOB_MODER &= ~(1 << (IN2 * 2 + 1));	//
	*GPIOB_MODER |=  (1 << (IN2 * 2));
	/////////////////////////////////////////////////
	
	//////////////GPIOB pin 4 mod output//////////////
	*GPIOB_MODER &= ~(1 << (IN3 * 2 + 1));	//
	*GPIOB_MODER |=  (1 << (IN3 * 2));
	/////////////////////////////////////////////////
	
	//////////////GPIOB pin 10 mod output//////////////
	*GPIOB_MODER &= ~(1 << (IN4 * 2 + 1));	//
	*GPIOB_MODER |=  (1 << (IN4 * 2));
	/////////////////////////////////////////////////
	

	while(1){
		state = (*GPIOA_IDR & (1 << GPIOA_BUTTON)); //зчитуємо стан з GPIOA pin 4(BUTTON)
		if((state != lastState && state)){
			switcher = !switcher; 		// swithcer має два стани: 0 - обертається в одному напрямку
										//, 1 - обертається в іншому напрямку
		}
		lastState = state;
		if(switcher){						   		 // обертаємо в одному напрямку
			*GPIOB_BSSR |= (1 << GPIOB_LEDB); 		 // вмикаємо синій світлодіод
			*GPIOA_BSSR |= (1 << (GPIOA_LEDR + 16)); // вимикаємо червоний світлодіод
			stepper(1);								 // викликаємо функцію яка буде обертати в одному напрямку(1)
		}
		else{ 										// інакше в іншому напрямку
			*GPIOA_BSSR |= (1 << GPIOA_LEDR);		// вмикаємо червоний світлодіод
			*GPIOB_BSSR |= (1 << (GPIOB_LEDB + 16));// вимикаємо синій світлодіод
			stepper(0);								// викликаємо функцію яка буде обертати в іншому напрямку(0)
		}
	}
}

