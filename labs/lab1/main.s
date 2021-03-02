; Initialize Stack Size
; Equ 400 hex (1024) bytes
Stack_Size      EQU     0x00000400

; Area STACK (NOINIT and READ/WRITE)
                AREA    STACK, NOINIT, READWRITE, ALIGN=4
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

                AREA    |.text| , CODE

Reset_Handler   PROC

                LDR     R0, =__main
                BX      R0
                ENDP
; X = 20h,	Y = 05h,	Z= 04h
; Define Constant Data in Code Area
X__		DCD 0x20
Y__		DCD 0x05
Z__		DCD	0x04	

__main      	PROC
; load Constant of readonly data memory
							ldr r8,  =X_
							ldr	r8,[r8]
							ldr r9,  =Y_
							ldr	r9,[r9]
							ldr r10, =Z_
							ldr	r10,[r10]
; load Constant of code memory
							ldr r4,  =X__
							ldr	r0,[r4]			; r0 = X
							ldr r4,  =Y__
							ldr	r1,[r4]         ; r1 = Y
							ldr r4, =Z__
							ldr	r2,[r4]         ; r2 = Z
; Evaluate Q = (X + Y) + (Z - Y) - (X - Y)
; Without sign calculation
							add r3, r0, r1	  	; r3 = r0+r1 = X + Y
							sub r4, r2, r1    	; r4 = r2-r1 = Z - Y
							sub r5, r0, r1    	; r5 = r0-r1 = X - Y
							add r6, r3, r4    	; r6 = (X+Y)+(Z-Y)
							sub r7, r6, r5    	; r7 = r6-(X-Y)= Q
; Calculation with the sign
							adds r3, r0, r1   	; r3 = r0+r1 = X + Y
							subs r4, r2, r1   	; r4 = r2-r1 = Z - Y
							subs r5, r0, r1   	; r5 = r0-r1 = X - Y
							adds r6, r3, r4   	; r6 = (X+Y)+(Y-Z)
							subs r8, r6, r5   	; r8 = r6-(X-Y) =Q
; Store rezult
							LDR r1, =Q
							STR r7, [r1]
							LDR r1, =Q_
							STR r8, [r1]

__mainloop
         				; place you application code here
		  B __mainloop


		  ENDP
; Define Constant Data in Area Readonly Data
							AREA    |MY CONSTANT|,DATA, READONLY
								ALIGN 4
X_		DCD 0x20
Y_		DCD 0x05
Z_		DCD	0x04
; Area Data Read/Write
                AREA    |MY DATA|,DATA
								ALIGN 4
Q			DCD	0			; Rezult 1
Q_	  DCD 0     ; Rezult 2
		  END