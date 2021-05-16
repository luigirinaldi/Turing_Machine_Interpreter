#include <stdio.h> 

int main(){
	int return_char = 10; //ascii value of \n 
	int space_char = 32;  //ascii value of whitespace
	
	char string[1];
	int string_size = 0;
	char* string_pointer = string;

	int in_char;
	in_char = getchar(); //read first char
	while(in_char != return_char){ //stop with line break
		*(string_pointer+(string_size)) = in_char;	//assign current char to current string (char array pointed to by string_pointer)
		string_size++;
    	char new_stringArr[string_size+1];
		for (int i =0;i<string_size;i++){
			//copy values from previous array to new array
			new_stringArr[i] = string_pointer[i];
		}
		string_pointer[string_size] = 0;
		//update pointer to point to new array
		string_pointer = &new_stringArr[0];	
		in_char = getchar();
	}

	int spaces[string_size];
	int num_strings = 1;

	printf("there are %d chars in the string\n",string_size);
	//printf("%s\n",string_pointer);

	//loop to print chars and find blank spaces(ascii 32)
	for(int i = 0;i<string_size;i++){
		*(spaces+i) = 0;
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
	for(int i = 0;i<string_size;i++){
		putchar(string_pointer[i]);
	}
	putchar('\n');

/*
    char* string_array[num_strings];
	if(num_strings > 1){
		int prevpos = 0;
		char* curr_string_pointer= string; // string is char array declared earlier
		for(int i =0;i<num_strings;i++){
			char curr_string[spaces[i]-prevpos];
			curr_string_pointer = curr_string;
			for (int j =prevpos; j<spaces[i];j++){
				*(curr_string_pointer+(j-prevpos)) = &string_pointer[j];
			}			
			string_array[i] = curr_string_pointer;
			prevpos = spaces[i];
		}
	}
	printf("\n%d",string_array[0]);
	printf("\n%c",string_array[0][0]);
	printf("\n%d",string_array[0][1]);
	printf("\n%d",string_array[1]);
	printf("\n%d",string_array[1][1]);
*/
	printf("\nfinished listening to chars");
    printf("\nthere are %d strings",num_strings);
	return 0;
}