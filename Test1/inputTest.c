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
		*(new_stringArr+string_size) = 0;	//initialize spot for next value (probably redundant)
		free(string_pointer); 				//delete previously created array		
		string_pointer = new_stringArr;		//update pointer to point to new array
		in_char = getchar();
	}

	int* spaces = calloc(string_size,sizeof(int));
	int num_strings = 1;

	printf("there are %d chars in the string\n",string_size);
	//printf("%s\n",string_pointer);

	//loop to print chars and find blank spaces(ascii 32)
	for(int i = 0;i<string_size;i++){
		//putchar(i+'0');
		if(*(string_pointer+i) == space_char){
			//found whitespace print new line char
			putchar(return_char); 
    		*(spaces+(num_strings-1)) = i;
			num_strings++;
		}else{
			//character is not a space so print it 
			putchar(*(string_pointer+i));
		}
	}
	putchar('\n');

	//CREATE STRING ARRAY
    char** string_array = calloc(num_strings, sizeof(char));
	if(num_strings > 1){
		int prevpos = 0;
		for(int i =0;i<num_strings;i++){
			char* curr_string = calloc(spaces[i]-prevpos,sizeof(char));		//assign memory space to string 
			for (int j = prevpos; j<spaces[i];j++){
				*(curr_string+(j-prevpos)) = *(string_pointer+j);
			}			
			*(string_array+i) = curr_string;	//assign current string to ith position of char matrix/string array
			prevpos = spaces[i];
		}
	}
	printf("%s\n",string_array[0]);

	printf("finished listening to chars \n");
    printf("there are %d strings \n",num_strings);
	return 0;
}