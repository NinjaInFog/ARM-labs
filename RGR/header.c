#include "header.h"

volatile uint32 pulse_width = 0;
volatile uint32 last_captured = 0;
volatile uint32 signal_polarity = 0;
volatile uint32 Distance = 0;


void GPIO_Config(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Enable the GPIOC clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Enable the GPIOB clock
	
	GPIOC->MODER &= ~GPIO_MODER_MODE6_0; // set PC6 as alternative mode(PC6 - Echo)
	GPIOC->MODER |= GPIO_MODER_MODE6_1;  //
	GPIOC->AFR[0] |= 0x2 << 6 * 4;		 // SET alternative function as TIM3_CH1
	
	GPIOC->MODER &= ~GPIO_MODER_MODE7_0; // set PC7 as alternative mode (PC7 - Trig)
	GPIOC->MODER |= GPIO_MODER_MODE7_1;  //
	GPIOC->AFR[0] |= 0x2 << 7 * 4;		 // SET alternative function as TIM3_CH2
	
	GPIOB->MODER &= ~GPIO_MODER_MODE8_0; // set PB8 as alternative mode (PC8 - Servo motor)
	GPIOB->MODER |= GPIO_MODER_MODE8_1;  //
	GPIOB->AFR[1] |= 0x2 << 0 * 4;		 // SET alternative function as TIM4_CH3
	
}

void TIM3_Config(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;  		  // Enable 
	TIM3->PSC = 16 - 1;							 //SET prescallar  F = 16 MHz/(15 + 1) = 1 MHz = 1 us
	TIM3->ARR = 0xffff - 1;						  //Max. value, Count_Period = (ARR + 1) / F = (0xffff) / (1 * 10 ^ (6))= 65 ms
	
	TIM3->CCMR1 &= ~TIM_CCMR1_CC1S;				  //SET chanel 1 as input capture
	TIM3->CCMR1 |= TIM_CCMR1_CC1S_0;			  //
	TIM3->CCMR1 &= ~TIM_CCMR1_IC1F; 			  //no filtering 
	TIM3->CCER |= TIM_CCER_CC1P | TIM_CCER_CC1NP; // set active both edges 
	TIM3->CCMR1 &= ~TIM_CCMR1_IC1PSC; 			  //SET no transition prescallar
	TIM3->CCER |= TIM_CCER_CC1E;				  //Enable input capture for chanel 1
	TIM3->DIER |= TIM_DIER_CC1IE;				  // Allow interrupt for chanel 1
	
	/////////PWM CONFIG for HC//////////
	
	TIM3->CCMR1 &= ~TIM_CCMR1_OC2M; //clear output compare mode bits
	TIM3->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1; // PWM mode 1 (OC2M = 110)
	TIM3->CCMR1 |= TIM_CCMR1_OC2PE; // Preload for output compare
	TIM3->CCER |= TIM_CCER_CC2E; 	//enable complementary output of CH2
	TIM3->CCER &= ~TIM_CCER_CC2P;	//set polarity
	TIM3->BDTR |= TIM_BDTR_MOE; 	//Main output enable(MOE) 
	TIM3->CCR2 = 9;					//set duration of high signal on PWM
	TIM3->CR1 |= TIM_CR1_CEN;		//ENABLE timer 
	
	NVIC_SetPriority(TIM3_IRQn, 0); //zero priority - the highest priority
	NVIC_EnableIRQ(TIM3_IRQn);     // enable interrupt for timer 3
}

void TIM4_Config(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;  				// Enable TIM4
	TIM4->PSC = 16 - 1;									//SET prescallar
	TIM4->ARR = 0x4E20 - 1;									//Max ARR value
	TIM4->CCMR2 &= ~TIM_CCMR2_OC3M; 					//clear output compare mode bits
	TIM4->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1; //PWM mode 1
	TIM4->CCMR2 |= TIM_CCMR2_OC3PE; 					//Preload for output compare(загружает первый раз ccr3 после первого переполнения)
	TIM4->CCER |= TIM_CCER_CC3E;						// complementary output
	TIM4->CCER &= ~TIM_CCER_CC3P; 						//set polarity active high
	TIM4->BDTR |= TIM_BDTR_MOE; 						//Main output enable(MOE) 
	TIM4->CCR3 = 1520;									//set duration of high signal on PWM
	TIM4->CR1 |= TIM_CR1_CEN;							//ENABLE timer 
}


void TIM3_IRQHandler()
{
	uint32 current_captured;
	if((TIM3->SR & TIM_SR_CC1IF) != 0) //interrupt caused by both edges
	{
		current_captured = TIM3->CCR1; 									  // read moment of the edge
		signal_polarity = 1 - signal_polarity; 							  // polarity of signal		
		if(signal_polarity == 0 && current_captured > last_captured)	  // 
			pulse_width = current_captured - last_captured;				  //
		else if (signal_polarity == 0 && current_captured < last_captured)//
			pulse_width = 0xffff - 1 - last_captured + current_captured;  //
		last_captured = current_captured;								  // 
		Distance = pulse_width * .034/2; 								  // (pulse time * speed of sound) 
	}
	if((TIM3->SR & TIM_SR_UIF) != 0)
		TIM3->SR &= ~TIM_SR_UIF;
}

uint32 angle(uint32 range)
{
	uint32 temp = 0;
	temp = (range - 1)*(2500 - 500)/(80) + 500;
	return temp;
}
