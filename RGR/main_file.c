#include "stm32f4xx.h"
#include "header.h"

int main(void)
{
	GPIO_Config();
	TIM3_Config();
	TIM4_Config();
	while(1)
	{
		if(Distance < 81){
			TIM4->CCR3 = angle(Distance);
			
		}
	}
}
