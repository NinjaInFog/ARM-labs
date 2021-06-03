#include "stm32f4xx.h"

typedef unsigned long uint32;
typedef unsigned long long uint64;

void TIM5_Config(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;  //Enable the timer clock
	TIM5->PSC = 16 - 1; //choose prescallar  16 MHz / 16 = 1 MHz = 1 us
	TIM5->ARR = 0xffff; // Max arr value
	
	TIM5->CR1 |= TIM_CR1_CEN; // Enable counter
		
	while(!(TIM5->SR & TIM_SR_UIF)); //whait until UIF flag == 1 
}

void GPIOC_Config(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Enable the GPIOC clock
	
	GPIOC->MODER |= GPIO_MODER_MODE8_0; //
	GPIOC->MODER &= ~GPIO_MODER_MODE8_1;
	
	GPIOC->MODER |= GPIO_MODER_MODE9_0;
	GPIOC->MODER &= ~GPIO_MODER_MODE9_1;
	
	GPIOC->MODER |= GPIO_MODER_MODE10_0;
	GPIOC->MODER &= ~GPIO_MODER_MODE10_1;
	
	GPIOC->MODER |= GPIO_MODER_MODE5_0;
	GPIOC->MODER &= ~GPIO_MODER_MODE5_1;
	
	GPIOC->MODER &= ~GPIO_MODER_MODE6_0; // set PC6 as alternative mode
	GPIOC->MODER |= GPIO_MODER_MODE6_1;  //
	GPIOC->AFR[0] |= 0x2 << 6 * 4;		 // SET alternative function as TIM3_CH1
	
}

void TIM3_Config(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;  // Enable 
	TIM3->PSC = 16 - 1;					//SET prescallar
	TIM3->ARR = 0xFFFF;					//Max ARR value
	
	TIM3->CCMR1 &= ~TIM_CCMR1_CC1S;		//SET chanel 1 as input 
	TIM3->CCMR1 |= TIM_CCMR1_CC1S_0;
	
	TIM3->CCMR1 &= ~TIM_CCMR1_IC1F; 	//no filtering 

	TIM3->CCER |= TIM_CCER_CC1P | TIM_CCER_CC1NP; // set active both edges 
	
	TIM3->CCMR1 &= ~TIM_CCMR1_IC1PSC; 	//SET no transition prescallar
	
	TIM3->CCER |= TIM_CCER_CC1E;		//Enable input capture for chanel 1
	
	TIM3->DIER |= TIM_DIER_CC1IE;		// Allow interrupt for chanel 1
	TIM3->CR1 |= TIM_CR1_CEN;			//ENABLE timer 

}

void delay_us(uint32 us)
{	
	TIM5->CNT = 0;
	while(TIM5->CNT < us);	
}


void delay_ms(uint32 ms)
{
	uint32 i = 0;
	for(i = 0; i < ms; i++)
		delay_us(1000);
}

volatile uint32 pulse_width     = 0;
volatile uint32 last_captured   = 0;
volatile uint32 signal_polarity = 0;
volatile uint32 Distance 		= 0;
void TIM3_IRQHandler()
{
	uint32 current_captured;
	
	if((TIM3->SR & TIM_SR_CC1IF) != 0) //interrupt caused by both edges
	{
		//GPIOC->ODR |= GPIO_ODR_OD10;
		current_captured = TIM3->CCR1; // read moment of the edge
		signal_polarity = 1 - signal_polarity; //
		

			
		if(signal_polarity == 0 && current_captured >= last_captured)
		{
			pulse_width = current_captured - last_captured;
			
		//	GPIOC->ODR &= ~GPIO_ODR_OD10;
		}
		else if (signal_polarity == 0 && current_captured < last_captured)
		{
			pulse_width = 0xffff - last_captured + current_captured;
		}
		last_captured = current_captured;
		Distance = pulse_width * .034/2; // (pulse time * speed of sound) / 2
	}
	
	if((TIM3->SR & TIM_SR_UIF) != 0)
		TIM3->SR &= ~TIM_SR_UIF;
	
}

void HS_READ(void)
{
	GPIOC->ODR |= GPIO_ODR_OD8;
	delay_us(10);
	GPIOC->ODR &= ~GPIO_ODR_OD8;
}


int main(void)
{
	TIM5_Config();
	TIM3_Config();
	GPIOC_Config();
	NVIC_SetPriority(TIM3_IRQn, 0); //zero priority
	
	NVIC_EnableIRQ(TIM3_IRQn);     // enable interrupt for timer 3
	while(1)
	{
		HS_READ();
		delay_ms(200);
		if(Distance>30)
		{
			GPIOC->ODR |= GPIO_ODR_OD9;
			GPIOC->ODR |= GPIO_ODR_OD10;
			GPIOC->ODR |= GPIO_ODR_OD5;
		} else if (Distance > 15)
		{
			GPIOC->ODR |= GPIO_ODR_OD9;
			GPIOC->ODR &= ~GPIO_ODR_OD10;
			GPIOC->ODR |= GPIO_ODR_OD5;
		} else if (Distance > 8)
		{
			GPIOC->ODR &= ~GPIO_ODR_OD9;
			GPIOC->ODR &= ~GPIO_ODR_OD10;
			GPIOC->ODR |= GPIO_ODR_OD5;
		} else if (Distance > 4)
		{
			GPIOC->ODR &= ~GPIO_ODR_OD9;
			GPIOC->ODR &= ~GPIO_ODR_OD10;
			GPIOC->ODR &= ~GPIO_ODR_OD5;
		}
		
	}
}	