/*
    Testo dell'esercizio:
        Progettare una macchina di turing che, data in ingresso 
        una sequenza di simobli 'A', sostituisca quelli in posto
        dispari con i simboli 'B'

    ANSI escape codes 
    ESC[{line};{column}H moves cursor to line #, column #
    ESC[0K	clears from cursor to end of line
    ESC is \033

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "funcs.h"

int main(){
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
			*(new_stringArr+string_size) = '\0';	    //initialize spot for next value to null value to make proper string
			free(string_pointer); 						//delete previously created array		
			string_pointer = new_stringArr;				//update pointer to point to new array
		}else if (new_string){	//if it is a space, it means the current string has finished and it must be added to the string array, which also has to be enlarged
			
			*(string_array+num_strings) = string_pointer;				//assign string pointer to string array
            string_size = 0;
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
    if (new_string){
        *(string_array+num_strings) = string_pointer;		//assign string pointer to string array
        num_strings++;
    }

    for(int i = 0;i < num_strings;i++){
        printf("test of working string: %s\n",string_array[i]);
    }
	return 0;
}
