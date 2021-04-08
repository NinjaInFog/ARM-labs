Stack_Size					EQU		0x00000400

RCC_AHB1_BASE	EQU	0x40023800
GPIOA_BASE	EQU	0x40020000
	
RCC_AHB1_ENR_OFFSET	EQU	0x30
GPIOA_MODER_OFFSET	EQU	0X00
GPIOA_BSRR_OFFSET	EQU	0x18

RCC_AHB1_GPIOA_CLOCK_BIT EQU	0
GPIOA_LED_PIN		 EQU	5

; Area STACK (NOINIT and READ/WRITE)
               AREA    STACK, NOINIT, READWRITE, ALIGN=3
; Initialize memory equals Stack_Size
Stack_Mem       SPACE   Stack_Size
__initial_sp
; Vector Table Mapped to Address 0 at Reset
               AREA    RESET, DATA, READONLY
               EXPORT  __Vectors
               EXPORT  Reset_Handler             [WEAK]
__Vectors
               DCD     __initial_sp             ; Top of Stack
               DCD     Reset_Handler            ; Reset Handler
__Vectors_End
               AREA    |.text| , CODE, READONLY

Reset_Handler   PROC
               LDR     R0, =main
               BX      R0
               ENDP


			
		
		EXTERN main_loop
main	PROC
	
	BL main_loop
	B main
	ENDP
	ALIGN
	
	END