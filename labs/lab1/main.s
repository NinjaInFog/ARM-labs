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
; X = 04h,	Y = 35h,	Z= 08h
; Define Constant Data in Code Area
X_		DCD 0x04
Y_		DCD 0x35
Z_		DCD	0x08	

;A=BFh, B=0Fh, C=35h, D=31h
A		EQU 0xBF
B_		EQU 0x0F
C		EQU 0x35
D		EQU 0x31

__main      	PROC
; load Constant of readonly data memory
							ldr r8,  =X_
							ldr	r8,[r8]
							ldr r9,  =Y_
							ldr	r9,[r9]
							ldr r10, =Z_
							ldr	r10,[r10]
							
; Evaluate (Y + Z) + X - (Y - X) + Z = Q
							add r3, r9, r10	  	; r3 = r9+r10 = Y + Z = 35 + 8 = 3D
							sub r4, r9, r8    	; r4 = r9-r8 = Y - X = 35 - 4 = 31
							sub r5, r3, r4    	; r5 = r3-r4 = (Y + Z) - (Y - X) = C
							add r6, r5, r8    	; r6 = r5 + r8 = (Y + Z) - (Y - X) + X = 10
							add r7, r6, r10    	; r7 = r6 + r10 = (Y + Z) - (Y - X) + X + Z = 18

;Q = !(A and !B and C) + D and !C and B xor !(A and C and D) , де A=BFh, B=hh, C=35h, D=31h.
							mov r2,  #A					; 0xBF
							mov r3,  #B_				; 0x0F
							mov r4,  #C					; 0x35
							mov r5,  #D					; 0x31
							EOR r8,  r3, #0xFFFFFFFF    ; r8 = !B = FFFF FFF0
							AND r8,  r8, r2				; r8 = !B * A = FFFF FFF0 * BF = B0
							AND r8,  r8, r4				; r8 = !B * A * C = B0 * 35 = 30
							EOR r8,  r8, #0xFFFFFFFF	; r8 = !r8 = !(!B * A * C) = !30 = FFFF FFCF
							AND r9,  r2, r4				; r9 = A * C = BF * 35 = 35
							AND r9,  r9, r5				; r9 = A * C * D  = 35 * 31 = 31
							EOR r9,  r9, #0xFFFFFFFF	; r9 = !(A * C * D) = !(1) = FFFF FFCE
							EOR r10, r4, #0xFFFFFFFF	; r10 = !C = FFFF FFCA
							AND r10, r10, r5			; r10 = !C * D = FFFF FFCA * 31 = 0
							AND r10, r10, r3			; r10 = !C * D * B = 0 * F = 
							EOR r10, r10, r9			; r10 = (!C * D * B) xor !(A * C * D) = FFFF FFCE
							ORR r10, r10, r8    		; r10 = !(!B * A * C) + (!C * D * B) xor !(A * C * D) = FFFF FFCF * FFFF FFCE = FFFF FFCF
							
; Store rezult
							LDR r1, =Q
							STR r7, [r1]		; 18
							LDR r2, =Q_			
							STR r10, [r2]		; FFFF FFCF
							
							

__mainloop
         				; place you application code here
		  B __mainloop


		  ENDP
; Area Data Read/Write
                AREA    |MY DATA|,DATA, READWRITE
								ALIGN 4
Q	  DCD 0		; Rezult 1
Q_	  DCD 0     ; Rezult 2
		  END