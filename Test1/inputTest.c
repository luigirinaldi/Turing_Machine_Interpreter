#include <stdio.h> 
#include <stdlib.h> //contains memalloc and other stuff
#include <stdbool.h>

int main(){
/*
	what to do: 
		as values come in, add them to ever growing char array(string), 
		when whitespace or newline comes assign pointer of string to string array(char matrix)
*/

	int return_char = 10; //ascii value of \n 
	int space_char = 32;  //ascii value of whitespace
	
	int string_size = 0;
	int num_strings = 0;
	char* string_pointer = calloc(string_size+1,sizeof(char));	// "create" array of chars
	char** string_array = calloc(num_strings+1, sizeof(char));	// create array of array of chars (string array)

	bool new_string = false; 									// used to avoid mulitple spaces counting as empy values
	int in_char;
	in_char = getchar(); //read first char
	while(in_char != return_char){ //stop with line break
		if(in_char != space_char){	//if it is not a space, increment the current string array
			new_string = true;
			*(string_pointer+(string_size)) = in_char;	//assign current char to current string (char array pointed to by string_pointer)
			string_size++;
			char* new_stringArr = calloc(string_size+1,sizeof(char)); //create new array with correct size
			for(int i = 0;i < string_size;i++){
				*(new_stringArr+i) = *(string_pointer+i);
			}
			*(new_stringArr+string_size) = space_char;	//initialize spot for next value to whitespace
			free(string_pointer); 						//delete previously created array		
			string_pointer = new_stringArr;				//update pointer to point to new array
		}else if (new_string){	//if it is a space, it means the current string has finished and it must be added to the string array, which also has to be enlarged
			//create new array to remove extra final char
			char* final_string = calloc(string_size,sizeof(char));
			for(int i = 0;i < string_size;i++){
				*(final_string+i) = *(string_pointer+i);
			}
			free(string_pointer);
			string_size = 0;										//reset counter for next string
			string_pointer = calloc(string_size+1,sizeof(char));	//assign memory for new string
			printf("strings: %d, '%s' \n",num_strings,final_string);

			*(string_array+num_strings) = final_string;				//assign string pointer to string array
			num_strings++;
			char** new_string_arr = calloc(num_strings+1,sizeof(char));
			for(int i = 0;i < num_strings;i++){
				*(new_string_arr+i) = *(string_array+i);
			}
			free(string_array);
			string_array = new_string_arr;

			new_string = false;
		}			
		in_char = getchar();
		
	}
	//received return char, finish building string array
	char* final_string = calloc(string_size,sizeof(char));
	for(int i = 0;i < string_size;i++){
		*(final_string+i) = *(string_pointer+i);
	}
	free(string_pointer);
	string_size = 0;								//reset counter
	printf("strings: %d, '%s' \n",num_strings,final_string);

	*(string_array+num_strings) = final_string;		//assign string pointer to string array
	num_strings++;
	char** new_string_arr = calloc(num_strings+1,sizeof(char));
	for(int i = 0;i < num_strings;i++){
		*(new_string_arr+i) = *(string_array+i);
	}
	free(string_array);
	string_array = new_string_arr;

	for(int i = 0;i<num_strings;i++){
		printf("['");
		printf("%s",string_array[i]);
		printf("'] ");
	}
	putchar('\n');
	return 0;
}