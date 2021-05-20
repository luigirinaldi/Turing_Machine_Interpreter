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
    MDT Alessandro 

qin in out qout dir |qin in qout out dir 
0 {'0','0',0,'>'},  0,0,0,0,>
0 {'1','1',0,'>'},  0,1,0,1,>
0 {'2','2',0,'>'},  0,2,0,2,>
0 {'3','3',0,'>'},  0,3,0,3,>
0 {'4','4',0,'>'},  0,4,0,4,>
0 {'5','5',0,'>'},  0,5,0,5,>
0 {'6','6',0,'>'},  0,6,0,6,>
0 {'7','7',0,'>'},  0,7,0,7,>
0 {'8','8',0,'>'},  0,8,0,8,>
0 {'9','9',0,'>'},  0,9,0,9,>
0 {'-','-',1,'<'},  0,-,1,-,<
0 {'=','=',0,'>'},  0,=,0,=,>
1 {'1','0',2,'<'},  1,1,2,0,<
1 {'2','1',2,'<'},  1,2,2,1,<
1 {'3','2',2,'<'},  1,3,2,2,<
1 {'4','3',2,'<'},  1,4,2,3,<
1 {'5','4',2,'<'},  1,5,2,4,<
1 {'6','5',2,'<'},  1,6,2,5,<
1 {'7','6',2,'<'},  1,7,2,6,<
1 {'8','7',2,'<'},  1,8,2,7,<
1 {'9','8',2,'<'},  1,9,2,8,<
1 {'0','9',1,'<'},  1,0,1,9,<
1 {'=','=',4,'>'},  1,=,4,=,>
1 {'-','=',6,'<'},  1,-,6,=,<
2 {'0','0',2,'<'},  2,0,2,0,<
2 {'1','1',2,'<'},  2,1,2,1,<
2 {'2','2',2,'<'},  2,2,2,2,<
2 {'3','3',2,'<'},  2,3,2,3,<
2 {'4','4',2,'<'},  2,4,2,4,<
2 {'5','5',2,'<'},  2,5,2,5,<
2 {'6','6',2,'<'},  2,6,2,6,<
2 {'7','7',2,'<'},  2,7,2,7,<
2 {'8','8',2,'<'},  2,8,2,8,<
2 {'9','9',2,'<'},  2,9,2,9,<
2 {'-','=',3,'<'},  2,-,3,=,<
2 {'=','=',3,'<'},  2,=,3,=,<
3 {'-','1',0,'>'},  3,-,0,1,>
3 {'0','1',0,'>'},  3,0,0,1,>
3 {'1','0',3,'<'},  3,1,3,0,<
4 {'9','9',4,'>'},  4,9,4,9,>
4 {'-','-',5,'<'},  4,-,5,-,<
5 {'9','-',5,'<'},  5,9,5,-,<
5 {'=','-',7,'<'},  5,=,7,-,<
6 {'-','0',6,'>'},  6,-,6,0,>
6 {'=','=',4,'>'},  6,=,4,=,>

*/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "funcs.h"          //custom include, to not have functions bloating code

#ifdef _WIN32
    #include <windows.h> 
#endif 

#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#define RESET_STYLE "\x1b[0m"
#define DEFAULT_COLOR "\x1b[37m"                //white text
#define HIGHLIGHT_TEXT "\x1b[31m\x1b[1m"       //bold and red




int DELAY_TIME = 50; //100 ms
const int num_states = 7;

const int final_state = 7;
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

    //char states[][50] = {"stato dispari","stato pari","cancella tutto","finito :D"};
    char states[][50] = {"0","1","2","3","4","5"};
    
    //char work_alphabet[] = {'A','B','-'};
    char work_alphabet[] = {'0','1','2','3','4','5','6','7','8','9','=','-'};
    char alfabet_color[][10]  = {"\x1b[35m","\x1b[32m","\x1b[37m"};
    const int num_symbols = sizeof(work_alphabet)/sizeof(work_alphabet[0]);
    printf("num symbols: %d",num_symbols);
    //rows are states, columns are input values
    //dims are, states,inputs and  are: value to print, nextstate and direction to move
    int trans_func[][sizeof(work_alphabet)/sizeof(work_alphabet[0])][4]={
        {                                       //0, 0 1 2 3 4 5 6 7 8 9 = -
            {'0','0',0,'>'},
            {'1','1',0,'>'},
            {'2','2',0,'>'},
            {'3','3',0,'>'},
            {'4','4',0,'>'},
            {'5','5',0,'>'},
            {'6','6',0,'>'},
            {'7','7',0,'>'},
            {'8','8',0,'>'},
            {'9','9',0,'>'},
            {'-','-',1,'<'},
            {'=','=',0,'>'}},                            
        {                                       //1
            {'1','0',2,'<'},
            {'2','1',2,'<'},
            {'3','2',2,'<'},
            {'4','3',2,'<'},
            {'5','4',2,'<'},
            {'6','5',2,'<'},
            {'7','6',2,'<'},
            {'8','7',2,'<'},
            {'9','8',2,'<'},
            {'0','9',1,'<'},
            {'=','=',4,'>'},
            {'-','=',6,'<'}},                    
        {                                       //2
            {'0','0',2,'<'},
            {'1','1',2,'<'},
            {'2','2',2,'<'},
            {'3','3',2,'<'},
            {'4','4',2,'<'},
            {'5','5',2,'<'},
            {'6','6',2,'<'},
            {'7','7',2,'<'},
            {'8','8',2,'<'},
            {'9','9',2,'<'},
            {'-','=',3,'<'},
            {'=','=',3,'<'}},                      
        {                                       //3
            {'-','1',0,'>'},
            {'0','1',0,'>'},
            {'1','0',3,'<'}},         
        {                                       //4
            {'9','9',4,'>'},
            {'-','-',5,'<'}},    
        {                                       //5
            {'9','-',5,'<'},
            {'=','-',7,'<'}},
        {                                       //6
            {'-','0',6,'>'},
            {'=','=',4,'>'}},            
    };


    char input_string[] = "-420-";
    int cursor_pos = 1;                                     //start on first valid letter
    int prev_cursor_pos = cursor_pos;
    int curr_state = initial_state;                         //start with first state

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
    while(curr_state != final_state){        
        char curr_char = tape[cursor_pos];
        int curr_symbol;
        char found = 0;
        
        for(int i = 0; i < num_symbols && found != 1;i++){                //find index of current input, by comparing first value of innermost array
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
            printf("\x1b[0J");                       //clear from cursor to end of line
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
            if(cursor_pos < 1){                             //resize tape array
                tape_len++;
                char *new_tape = (char*) calloc(tape_len,sizeof(char));
                memcpy(new_tape+1,tape,tape_len-1);         //copy value from old tape to new one, starting from one posisiton ahead
                new_tape[0]='-';
                free(tape);
                tape = new_tape;
                cursor_pos++;
                prev_cursor_pos++;
                printf("\n");
                printf("\x1b[1;1f");                        //move to 1,1
                printf("%s",tape);                          //print new tape
            } else if(cursor_pos > (tape_len-2)){           //if it is greater than the last '-' then increase the array
                tape_len++;
                char *new_tape = (char*) calloc(tape_len,sizeof(char));
                memcpy(new_tape,tape,tape_len-1);         //copy value from old tape to new one, starting from one posisiton ahead
                new_tape[tape_len-1]='-';
                free(tape);
                tape = new_tape;
                printf("\n");
                printf("\x1b[1;1f");                        //move to 1,1
                printf("%s",tape);                          //print new tape
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
    printf("finished!\n");

    printf("\x1b[?25h");                    //cursor is back to being visible
    printf(RESET_STYLE);
    #ifdef _WIN32
    	SetConsoleMode(hStdout,originalMode);
    #endif
    return 0;
}


