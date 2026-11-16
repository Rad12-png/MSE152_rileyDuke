#include <stdio.h>

int main(){
	int *ptr = NULL;
	if(ptr == NULL){
	printf("Pointer is NULL, cannot derefference.\n");
	} else {
	printf("Pointer value: %d \n", *ptr);
	}
	return 0;
}
