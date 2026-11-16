#include <stdio.h>

void valueSwap(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
	return;
}

int main(){
	int num1 = 4;
	int num2 = 6;

	printf("Before swapping: x = %d, y = %d\n", num1, num2);
	valueSwap(&num1, &num2);
	printf("After swapping: x = %d, y = %d\n", num1, num2);

	return 0;
}
