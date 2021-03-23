//__asm void my_strcpy(const char *src, char *dst){
//loop
// 	LDRB r2, [r0]
//	ADDS r0, #1 
//	STRB r2, [r1] 
//	ADDS r1, #1 
//	CMP r2, #0 
//	BNE loop 
//	BX lr 
//}

__asm void my_strcpy(const char *src, char *dst){
loop
 	LDRB r2, [r0]
	ADDS r0, #1
	CMP r2, #0
	BNE loop
	SUBS r0, #2
loop2
	LDRB r2, [r0]
	SUBS r0, #1
	STRB r2, [r1]
	ADDS r1, #1
	CMP r2, #0
	BNE loop2
	BX lr 
}

__asm void my_capitalize(char *str){
cap_loop
	LDRB r1, [r0] 
	CMP r1, #'a'-1 
	BLS cap_skip
	CMP r1, #'z'
	BHI cap_skip
	SUBS r1,#32 
	STRB r1, [r0] 
cap_skip
	ADDS r0, r0, #1 
	CMP r1, #0 
	BNE cap_loop 
	BX lr 
}


__asm void Simple_add(){
	
}


int main(void)
{
	const char a[] = "Hello world!";
	//unsigned int a = 1, b = 2, d = 3, e = 4, f = 5, h = 0xfffffff1;
	//volatile int c = 0;
	char b[20];
	my_strcpy(a, b);
	my_capitalize(b);
	while (1);
}
