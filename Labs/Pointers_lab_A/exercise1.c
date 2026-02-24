#include <stdio.h>

int main(){
	int myVal = 5;
	int *myPtr = &myVal;
	printf("My value is: %d\n", myVal);
	printf("The value which the pointer is pointing to is: %d\n", *myPtr);
	printf("The address of my value is: %p\n", (void*)myPtr);
	return 0;
}
