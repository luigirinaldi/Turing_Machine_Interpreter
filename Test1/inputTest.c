#include <stdio.h> 
#include <stdlib.h> //contains memalloc and other stuff

int main(){
	int return_char = 10; //ascii value of \n 
	int space_char = 32;  //ascii value of whitespace
	
	int string_size = 0;
	char* string_pointer;
	string_pointer = calloc(string_size+1,sizeof(char)); // "create" array of chars

	int in_char;
	in_char = getchar(); //read first char
	while(in_char != return_char){ //stop with line break
		*(string_pointer+(string_size)) = in_char;	//assign current char to current string (char array pointed to by string_pointer)
		string_size++;
    	char* new_stringArr;
		new_stringArr = calloc(string_size+1,sizeof(char)); //create new array with correct size
		for (int i =0;i<string_size;i++){			
			*(new_stringArr+i) = *(string_pointer+i);		//copy values from previous array to new array
		}
		*(new_stringArr+string_size) = space_char;	//initialize spot for next value to whitespace
		free(string_pointer); 						//delete previously created array		
		string_pointer = new_stringArr;				//update pointer to point to new array
		in_char = getchar();
	}

	int* spaces = calloc(string_size,sizeof(int));
	int num_strings = 0;

	printf("there are %d chars in the string\n",string_size);

	//loop to print chars and find blank spaces(ascii 32)
	for(int i = 0;i<string_size;i++){
		//putchar(i+'0');
		if(*(string_pointer+i) == space_char){
			//found whitespace 
    		*(spaces+(num_strings)) = i;
			num_strings++;
		}
	}	
    printf("there are %d strings \n",num_strings);

	//CREATE STRING ARRAY
    char** string_array = calloc(num_strings, sizeof(char));
	int prevpos = 0;
	for(int i = 0;i<num_strings;i++){
		char* curr_string = calloc(spaces[i]-prevpos,sizeof(char));		//assign memory space to string 
		for (int j = prevpos; j<spaces[i];j++){
			*(curr_string+(j-prevpos)) = *(string_pointer+j);
		}			
		*(string_array+i) = curr_string;	//assign current string to ith position of char matrix/string array
		prevpos = spaces[i]+1;
	}

	for(int i = 0;i<num_strings;i++){
		printf("['");
		printf("%s",string_array[i]);
		printf("'] ");
	}
	putchar('\n');
	return 0;
}