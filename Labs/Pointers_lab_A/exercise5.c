#include <stdio.h>

void reverseString(char *string){
	char *first = string;
	char *last = string;
	char temp;

	//make "last" actually equal to the last char
	while (*last != '\0'){
		last++;
	}
	// make "last equal to the last letter
	if (last > first){
	last--;
	}

	// reverse it
	while (first < last){
		temp = *first;
		*first = *last;
		*last = temp;

		first++;
		last--;
	}
}


int main(){

	char myString[] = "Hello";
	char *myPointer = myString;
	while(*myPointer != '\0'){
	printf("%c ", *myPointer);
	myPointer++;
	}
	reverseString(myString);
	printf("Reverse string: %s\n", myString);

	return 0;
}
