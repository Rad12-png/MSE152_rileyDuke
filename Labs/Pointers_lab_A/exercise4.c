#include <stdio.h>

int main(){
	int arr[5] = {1, 5, 3, 8, 11};
	int *ptr = arr;
	
	for(int i = 0; i < 5; i++){
		printf("The value of arr[%d] is %d", i, *(ptr + i));
		printf("\n");
	}
	// modifying the array
	for(int i = 0; i < 5; i++){
	*(ptr + i) = *(ptr + i) * 2;
	}
	
	//print the updated array
	for(int i = 0; i < 5; i++){
	printf("The NEW value of arr[%d] is %d", i, *(ptr + i));
	printf("\n");

	}
	
	return 0;
}
