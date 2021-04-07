#include "defines.h"
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
int main(void)
{
	const char a[] = "Hello world!";
	const int arr_numb = 7;
	const int sum_numb = 3;
	uint32 summ[sum_numb] = {1, 0xfffffff1, 15};
	uint32 arr[arr_numb] = {15, 0xfffffff6, 24, 0xfffffff7, 22, 0xfffffff9, 0xfffffffA};
	volatile uint64 c = 0;
	volatile uint64 max = 0;
	char b[20];
	int n = 5;
	my_strcpy(a, b);
	c = unsigned_sum(summ, sum_numb);
	max = arr_max(arr, arr_numb);
	while (1);
}