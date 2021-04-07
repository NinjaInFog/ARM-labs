typedef unsigned long uint32;
typedef unsigned long long uint64;

#define GPIO_BSRR_OFFSET		0x18
#define GPIO_IDR_OFFSET			0x10
#define GPIO_MODER_OFFSET		0x00

#define RCC_AHB1_ENABLE_REG 	(uint32*)0x40023830

#define GPIOA_BASE_ADDR     	0x40020000
#define GPIOA_BSSR				(uint32*)(GPIOA_BASE_ADDR + GPIO_BSRR_OFFSET)
#define GPIOA_IDR				(uint32*)(GPIOA_BASE_ADDR + GPIO_IDR_OFFSET)
#define GPIOA_MODER				(uint32*)(GPIOA_BASE_ADDR + GPIO_MODER_OFFSET)

#define GPIOA_BUTTON			4
#define GPIOA_LEDR				1
#define GPIOB_LEDB				8

#define GPIOB_BASE_ADDR     	0x40020400
#define GPIOB_BSSR				(uint32*)(GPIOB_BASE_ADDR + GPIO_BSRR_OFFSET)
#define GPIOB_IDR				(uint32*)(GPIOB_BASE_ADDR + GPIO_IDR_OFFSET)
#define GPIOB_MODER				(uint32*)(GPIOB_BASE_ADDR + GPIO_MODER_OFFSET)

#define IN1						3
#define IN2						5
#define IN3						4
#define IN4						10


#define GPIOA_RCC_Init *RCC_AHB1_ENABLE_REG |= (1 << 0)
#define GPIOB_RCC_Init *RCC_AHB1_ENABLE_REG |= (1 << 1)


int Steps = 0;
int Direction = 1;


// функция вращения
void stepper(uint32 dir){

volatile int i = 0;
	
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

	
	if(dir)     //если вращение впёред то увеличиваем steps
		Steps++;
	else 		//иначе уменьшаем
		Steps--;
	
	if(Steps > 3)	//если первысило кол-во шагов то обнуляем
		Steps = 0;
	if(Steps<0)		//если первысило кол-во шагов в обратную сторону то устанавливаем в максимальное значение  
		Steps = 3;
}


void main_loop()
{
	volatile uint64 counter = 0;
	volatile uint32 state = 0;
	volatile uint32 lastState = 0;
	volatile uint32 switcher = 0;
	GPIOA_RCC_Init;
	GPIOB_RCC_Init;
	*GPIOA_MODER &= ~(1 << (GPIOA_LEDR * 2 + 1));	//
	*GPIOA_MODER |=  (1 << (GPIOA_LEDR * 2));
	
	*GPIOB_MODER &= ~(1 << (GPIOB_LEDB * 2 + 1));	//
	*GPIOB_MODER |=  (1 << (GPIOB_LEDB * 2));
	
	*GPIOA_MODER &= ~(1 << (GPIOA_BUTTON * 2));		//
	*GPIOA_MODER &= ~(1 << (GPIOA_BUTTON * 2 + 1));
	////////////////////////////////
	
	*GPIOB_MODER &= ~(1 << (IN1 * 2 + 1));	//
	*GPIOB_MODER |=  (1 << (IN1 * 2));
	
	*GPIOB_MODER &= ~(1 << (IN2 * 2 + 1));	//
	*GPIOB_MODER |=  (1 << (IN2 * 2));
	
	*GPIOB_MODER &= ~(1 << (IN3 * 2 + 1));	//
	*GPIOB_MODER |=  (1 << (IN3 * 2));
	
	*GPIOB_MODER &= ~(1 << (IN4 * 2 + 1));	//
	*GPIOB_MODER |=  (1 << (IN4 * 2));
	
	//*GPIO_BSRR |= (1 << (GPIOA_LED_PIN + 16));

	while(1){
		state = (*GPIOA_IDR & (1 << GPIOA_BUTTON));
		if((state != lastState && state)){
			switcher = !switcher;
		}
		lastState = state;
		if(switcher){
			*GPIOB_BSSR |= (1 << GPIOB_LEDB);
			*GPIOA_BSSR |= (1 << (GPIOA_LEDR + 16));
			stepper(1);
		}
		else{
			*GPIOA_BSSR |= (1 << GPIOA_LEDR);
			*GPIOB_BSSR |= (1 << (GPIOB_LEDB + 16));
			stepper(0);
		}
	}
}

