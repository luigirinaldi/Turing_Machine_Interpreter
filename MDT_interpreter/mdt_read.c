
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#include "funcs.h"          //custom include, to not have functions bloating code

#ifdef _WIN32
    #include <windows.h> 
#endif 

#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#define RESET_STYLE "\x1b[0m"
#define DEFAULT_COLOR "\x1b[37m"                //white text
#define HIGHLIGHT_TEXT "\x1b[31m\x1b[1m"       //bold and red


char input_string[] = "-AABBABBABBA-";
int DELAY_TIME = 500; //100 ms

int main(){
    #ifdef _WIN32
	    HANDLE hStdout;
    	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    	DWORD mode, originalMode;
    	GetConsoleMode(hStdout,&mode);
    	originalMode = mode;
    	mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    	SetConsoleMode(hStdout, mode);    
    #endif    

    //options stuff
    char trailing_lambdas = 0;
    printf("insert delay\n");
    scanf(" %d",&DELAY_TIME);
    printf("custom options? (y/n)\n");
    char custom_options;
    scanf(" %c",&custom_options);
    if(custom_options == 'y'){
        
        printf("remove trailing lambas? (y/n)\n");
        scanf(" %c",&trailing_lambdas);
        if(trailing_lambdas == 'y'){
            trailing_lambdas = 1;
        } else{
            trailing_lambdas = 0;
        }
        printf("custom input string? (y/n)\n");
        char custom_input;
        scanf(" %c",&custom_input);
        if(custom_input == 'y'){
            printf("insert new string:\n");
            scanf(" %s",input_string);
        }
    }
    printf("this is where the fun begins\n");
    custom_delay(DELAY_TIME);

    char *filename = "bruh.txt";

    char *buf = getcwd(NULL, 0); //automagically assigns intself correctly sized buffer for current working directory(cwd)
    if (buf == NULL) {
        printf("failed while retrieving current dir\n");
    }
    printf("Current working directory: %s\n", buf);
    char abs_path[sizeof(buf)+sizeof(filename)+2];
    sprintf(abs_path,"%s\\%s",buf,filename); //join cwd and filename to get absolute path of txt file
    
    char ***trans_func;
    int num_states;
    char **states;
    char *alphabet;
    int alphabet_size;
    int *state_in_sizes;

    read_mdt(
        abs_path, 
        &trans_func,
        &num_states,
        &states,
        &alphabet,
        &alphabet_size,
        &state_in_sizes
    );

    printf("num states: %d\n",num_states);
    int initial_state = 0;
    int final_state;
    
    for(int i =0;i<num_states;i++){
        printf("%d, %d",i,state_in_sizes[i]);
        if(state_in_sizes[i] ==0){

            final_state = i;
            printf("final state is %d\n",final_state);
        }
    }

    int cursor_pos = 1;                                     //start on first valid letter
    int prev_cursor_pos = cursor_pos;
    int curr_state = initial_state;                         //start with first state
    char reprint_tape = 0;                                  //char used as boolean to check whether or not stirng has to be reprinted


    int tape_len = strlen(input_string);
    char *tape = (char*) calloc(tape_len,sizeof(char));     //assign memory for tape 
    memcpy(tape,input_string,tape_len);

    
    
    printf("\x1b[1;1H\x1b[2J");                             //clear canvas and set cursor to 1,1 and
    printf("\x1b[?25l");                                    //make cursor invisible 
    printf(RESET_STYLE);                                    //reset all styles 
    printf(DEFAULT_COLOR);                                  //color with code 37 (white)
    printf("%s",tape);                              //print first time      
    printf("\x1b[2;1f");				                    //position cursor to print state
    char initial_string[] = "curr state: ";       
    printf("%s%s",initial_string,states[curr_state]);  
    custom_delay(DELAY_TIME);         
    while(curr_state != final_state){        
        char curr_char = tape[cursor_pos];
        int curr_symbol;
        char found = 0;
        
        for(int i = 0; i < alphabet_size && found != 1;i++){                //find index of current input, by comparing first value of innermost array
            if(trans_func[curr_state][i][0]==curr_char){
                curr_symbol = i;
                found = 1;              
            }
        }

        if (found == 1){
            char next_char = trans_func[curr_state][curr_symbol][1];
            int next_state = trans_func[curr_state][curr_symbol][2];
            char direction = trans_func[curr_state][curr_symbol][3];    

            tape[cursor_pos] = next_char;               //update input string 
            curr_state = next_state;                            //update state 
                                
            
            //reprint old value 
            printf("\n");				                    //adding so that it works on ipad
            printf("\x1b[1;%df",prev_cursor_pos+1);       	//move cursor to previous char 
            printf("%s%s",RESET_STYLE,DEFAULT_COLOR);		//reset style of previous char
            putchar(tape[prev_cursor_pos]);

            //print new value 
            printf("\x1b[1;%df",cursor_pos+1);              //move cursor to cursor pos to update string, +1 because terminal coords start from 1,1
            printf(HIGHLIGHT_TEXT);                         //highlight text property defined at the start
            putchar(next_char);
            //reset style for cells after the current one
            printf("\x1b[1;%df",cursor_pos+2);	            //move to next cell
            printf("%s%s",RESET_STYLE,DEFAULT_COLOR);       //reset style for cells on forward
                
            //update current state
            putchar('\n'); //again added so ipad works kinda
            printf("\x1b[2;%df",strlen(initial_string)+1);                    //move to update current state
            printf("\x1b[0J");                                                //clear from cursor to end of line
            printf("%s",states[curr_state]);

            printf(RESET_STYLE);
                
            prev_cursor_pos = cursor_pos;                   //save old position to reset it to normal text
            switch(direction){                              //update cursor pos based on direction        
                case '>':
                    cursor_pos++;
                    break;
                case '<':
                    cursor_pos--;
                    break;
                case '-':
                    //cursor doesn't move
                    break;
            }

            if(trailing_lambdas){
                int found_char = 1;
                int counter = 1;
                while(found_char){
                    if(tape[counter] == '-'){
                        tape_len--;
                        char *new_tape = (char*) calloc(tape_len,sizeof(char));
                        memcpy(new_tape,tape+1,tape_len);         //shift all values of tape to the left, because first one is '-'
                        free(tape);
                        tape = new_tape;
                        cursor_pos--;
                        prev_cursor_pos--;
                        reprint_tape = 1;
                    } else{
                        found_char = 0;
                    }
                }
            }

            if(cursor_pos < 1){                             //resize tape array
                tape_len++;
                char *new_tape = (char*) calloc(tape_len,sizeof(char));
                memcpy(new_tape+1,tape,tape_len-1);         //copy value from old tape to new one, starting from one posisiton ahead
                new_tape[0]='-';
                free(tape);
                tape = new_tape;
                cursor_pos++;
                prev_cursor_pos++;
                reprint_tape = 1;
            } else if(cursor_pos > (tape_len-2)){           //if it is greater than the last '-' then increase the array
                tape_len++;
                char *new_tape = (char*) calloc(tape_len+1,sizeof(char));
                memcpy(new_tape,tape,tape_len-1);         //copy value from old tape to new one, starting from one posisiton ahead
                new_tape[tape_len-1]='-';
                new_tape[tape_len] = '\0';
                free(tape);
                tape = new_tape;
                reprint_tape = 1;
            }

            if(reprint_tape){
                printf("\n");
                printf("\x1b[1;1f");                        //move to 1,1
                printf("%s%s",RESET_STYLE,DEFAULT_COLOR); 
                printf("%s",tape);                          //print new tape
                reprint_tape = 0;
            }
        } else{
            putchar('\n'); //again added so ipad works kinda
            printf("\x1b[2;%df",strlen(initial_string)+1);                    //move to update current state
            printf("\x1b[0J");                       //clear from cursor to end of line
            printf("%s","the mdt machine has failed :(");
            printf(RESET_STYLE);
            curr_state = final_state;
        }
        custom_delay(DELAY_TIME);
    }

    putchar('\n');
    printf("\x1b[3;1f");                        //move to 3,1
    printf("finished!\n");

    printf("\x1b[?25h");                    //cursor is back to being visible
    printf(RESET_STYLE);
    #ifdef _WIN32
    	SetConsoleMode(hStdout,originalMode);
    #endif
    return 0;
}


