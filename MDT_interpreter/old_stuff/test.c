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
#include <stdlib.h>		//malloc and calloc
#include <string.h>		//memcpy and strlen
#include <time.h>
#include <stdbool.h>

#include "funcs.h"

int main(){
	int return_char = 10; //ascii value of \n 
	int space_char = 32;  //ascii value of whitespace
	int comma_char = 44; 
	
	int string_size = 0;
	int num_strings = 0;
	char* string_pointer = calloc(string_size+1,sizeof(char));	// "create" array of chars
	char** string_array = calloc(num_strings+1, sizeof(char));	// create array of array of chars (string array)
	

	char new_string = 0; 									// used to avoid mulitple spaces counting as empy values
	int in_char;
	char not_finished = 1;
	
	do{	
		in_char = getchar(); //read first char
		if(in_char == return_char){
			not_finished = 0;
		}
		if (not_finished){ 
			if(in_char != space_char && in_char != comma_char){	//if it is not a space, increment the current string array
				
				*(string_pointer+(string_size)) = in_char;	//assign current char to current string (char array pointed to by string_pointer)
				string_size++;

				string_pointer = (char*) realloc(string_pointer,(string_size+1) * sizeof(char));  //create new array with correct size
				*(string_pointer+string_size) = '\0';	    //initialize spot for next value to null value to make proper string
				new_string = 1;
			}else if (new_string == 1){	//if it is a space, it means the current string has finished and it must be added to the string array, which also has to be enlarged
				new_string = 0;

				char *new_string = (char*) calloc(string_size+1,sizeof(char));					//create new memory to store new string
				memcpy(new_string,string_pointer,string_size+1);								//copy values from string_pointer to new_string
				
				*(string_array+num_strings) = new_string;									//assign string pointer to string array
				string_size = 0;
				string_pointer = (char*) realloc(string_pointer,(string_size+1) * sizeof(char));
				string_pointer[0] = '\0';															//initialize with null value 

				num_strings++;
				string_array = (char**) realloc(string_array,(num_strings+1) * sizeof(char*));		//resize array of strings

			}	 	
		}
	} while(not_finished); //stop with line break
    
	if (new_string){
        *(string_array+num_strings) = string_pointer;		//assign string pointer to string array
        num_strings++;
    }

    for(int i = 0;i < num_strings;i++){
        printf("string number %d: [%s]\n",i,string_array[i]);
    }
	return 0;
}
