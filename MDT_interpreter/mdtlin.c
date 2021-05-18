/*
    Testo dell'esercizio:
        Progettare una macchina di turing che, data in ingresso 
        una sequenza di simobli 'A', sostituisca quelli in posto
        dispari con i simboli 'B'

    ANSI escape codes 
    ESC[{line};{column}H moves cursor to line #, column #
    ESC[0K	clears from cursor to end of line
    ESC[0m reset color and style
    ESC[1m bold
    ESC is
        Octal: \033
        Unicode: \u001b
        Hexadecimal: \x1b
        Decimal: 27

*/
#include <stdio.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
    #include <windows.h> 
#endif 

#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#define RESET_STYLE "\x1b[0m"
#define DEFAULT_COLOR "\x1b[37m"        //white text
#define HIGHLIGHT_TEXT "\x1b[31m\x1b[1m"       //bold and red

void custom_delay(int time_in_milliseconds){
    double delay_time = (double)time_in_milliseconds/1000;
    double timediff = 0;
    clock_t curr_time;
    curr_time = clock();
    while(timediff<delay_time){
        timediff = ((double)clock()/CLOCKS_PER_SEC)-((double)curr_time/CLOCKS_PER_SEC);
    }  
}

void print_tape(char* tape,char* alphabet, char* *alphabet_color){

}

int DELAY_TIME = 500; //100 ms
const int num_states = 4;
const int num_symbols = 3;
const int final_state = 3;
const int initial_state = 0;

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

    char states[][50] = {"stato dispari","stato pari","cancella tutto","finito :D"};
    
    char work_alphabet[] = {'A','B','-'};
    char alfabet_color[][10]  = {"\x1b[35m","\x1b[32m","\x1b[37m");

    //rows are states, columns are input values
    //dims are, states,inputs and 3 are: value to print, nextstate and direction to move
    int trans_func[][sizeof(work_alphabet)/sizeof(work_alphabet[0])][3]={
        {{'A',1,'>'},{'-','-','-'},{'-',2,'<'}},            //q0, A B -
        {{'B',0,'>'},{'-','-','-'},{'-',2,'<'}},            //q1
        {{'-',2,'<'},{'-',2,'<'},{'-',3,'-'}},              //q2
        {{'-','-','-'},{'-','-','-'},{'-','-','-'}},        //q3
    };


    char input_string[100] = "-AAAAAAAAAAAAAAAAAAAAAAAA-";
    int cursor_pos = 1;                                     //start on first valid letter
    int prev_cursor_pos = cursor_pos;
    int curr_state = initial_state;                         //start with first state
    
    printf("\x1b[1;1H\x1b[2J");                             //clear canvas and set cursor to 1,1 and
    printf("\x1b[?25l");                                    //make cursor invisible 
    printf(RESET_STYLE);                                    //reset all styles 
    printf(DEFAULT_COLOR);                                  //color with code 37 (white)
    printf("%s",input_string);                              //print first time      
    printf("\x1b[2;1f");				    //position cursor to print state
    char initial_string[] = "curr state: ";       
    printf("%s%s",initial_string,states[curr_state]);                  
    while(curr_state != final_state){        
        char curr_char = input_string[cursor_pos];
        int curr_symbol;
        for(int i = 0; i < num_symbols;i++){                //find index of current input
            if(work_alphabet[i]==curr_char){
                curr_symbol = i;
                break;
            }
        }
        char next_char = trans_func[curr_state][curr_symbol][0];
        int next_state = trans_func[curr_state][curr_symbol][1];
        char direction = trans_func[curr_state][curr_symbol][2];    

        input_string[cursor_pos] = next_char;               //update input string 
        curr_state = next_state;                            //update state 
	                           
        
        //reprint old value 
        printf("\n");				                    //adding so that it works on ipad
        printf("\x1b[1;%df",prev_cursor_pos+1);       	//move cursor to previous char 
        printf("%s%s",RESET_STYLE,DEFAULT_COLOR);		//reset style of previous char
        putchar(input_string[prev_cursor_pos]);

        //print new value 
        printf("\x1b[1;%df",cursor_pos+1);              //move cursor to cursor pos to update string, +1 because terminal coords start from 1,1
        printf(HIGHLIGHT_TEXT);      //highlight text property defined at the start
        putchar(next_char);
        //reset style for cells after the current one
        printf("\x1b[1;%df",cursor_pos+2);	            //move to next cell
        printf("%s%s",RESET_STYLE,DEFAULT_COLOR);       //reset style for cells on forward
            
        //update current state
        putchar('\n'); //again added so ipad works kinda
        printf("\x1b[2;%df",strlen(initial_string)+1);                    //move to update current state
        printf("\x1b[0J");                       //clear from cursor to end of line
        printf("%s",states[curr_state]);
        //printf("\x1b[2;%df",strlen(initial_string)+1+strlen(states[curr_state]));     //move to after newly printed state
        //printf("\x1b[0J");                       //clear from cursor to end of line
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

        custom_delay(DELAY_TIME);
    }

    putchar('\n');
    printf("finished!\n");

    printf("\x1b[?25h");                    //cursor is back to being visible
    printf(RESET_STYLE);
    #ifdef _WIN32
    	SetConsoleMode(hStdout,originalMode);
    #endif
    return 0;
}


