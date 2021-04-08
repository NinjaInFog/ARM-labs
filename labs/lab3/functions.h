#ifndef functions_h
#define functions_h
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


void stepper(uint32 dir);

#endif