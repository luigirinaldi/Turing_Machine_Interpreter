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
#include <string.h>
#include <time.h>
#include <windows.h>

void custom_delay(int time_in_milliseconds){
    double delay_time = (double)time_in_milliseconds/1000;
    double timediff = 0;
    clock_t curr_time;
    curr_time = clock();
    while(timediff<delay_time){
        timediff = ((double)clock()/CLOCKS_PER_SEC)-((double)curr_time/CLOCKS_PER_SEC);
    }  
}

void print_string(char input_string[]){
    putchar('|');
    for(int i = 0;i<strlen(input_string);i++){
        putchar(input_string[i]);
        putchar('|');
    }
    //putchar('\n');
}

int main(){
    //stuff for windows terminal
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;                                 //getting original console configuration
    GetConsoleMode(hConsole, &mode);                //used at the end to reset screen config
    const DWORD originalMode = mode;                //save original mode to variable 
    //mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;   //suggested by microsoft docs, doesn't work
    //SetConsoleMode(hConsole,mode);
    PCWSTR sequence = L"\x1b[2J";                   
    WriteConsoleW(hConsole, sequence, ARRAYSIZE(sequence), NULL, NULL); //clear terminal initially

    
    
    
    
    char states[3][40] = {"pari","dispari","FINITO!"};                   //make array one bigger to allow for \0 of strings
    int num_states = 3;
    char work_alphabet[3] = {'A','B','-'};
    int num_symbols = 3;
    //rows are states, columns are input values
    //dims are, states,inputs and 3 are: value to print, nextstate and direction to move
    int trans_func[3][3][3]={
        {{'A',1,'>'},{'-','-','-'},{'-',2,'-'}},            //q0, A B -
        {{'B',0,'>'},{'-','-','-'},{'-',2,'-'}},            //q1
        {{'-','-','-'},{'-','-','-'},{'-','-','-'}},        //q2
    };


    char input_string[100] = "-AAAAAAAAAA-";
    int cursor_pos = 1;                         //start on first valid letter
    int curr_state = 0;                         //start with first state
    int final_state = 2;
    
    
    //printf("%s",input_string);                  //print first time   
    COORD initial_pos = {0,0};
    SetConsoleCursorPosition(hConsole,initial_pos);
    WriteConsole(hConsole,input_string,ARRAYSIZE(input_string),NULL,NULL);

    //printf("\ncurr state: %s",states[curr_state]);//print state
    char state_lable[] = "curr state: ";
    int state_string_size = strlen(state_lable)+strlen(states[curr_state]);
    char* state_string = calloc(state_string_size,sizeof(char));// create array to store message to send
    sprintf(state_string,"%s %s",state_lable,*(states+curr_state));
    COORD state_pos = {0,1};
    SetConsoleCursorPosition(hConsole,state_pos);
    WriteConsole(hConsole,state_string,state_string_size,NULL,NULL);        
    
    while(curr_state != final_state){        
        char curr_char = input_string[cursor_pos];
        int curr_symbol;
        for(int i = 0; i < num_symbols;i++){                    //find index of current input
            if(work_alphabet[i]==curr_char){
                curr_symbol = i;
                break;
            }
        }
        char next_char = trans_func[curr_state][curr_symbol][0];
        int next_state = trans_func[curr_state][curr_symbol][1];
        char direction = trans_func[curr_state][curr_symbol][2];
                                       //update state 
        input_string[cursor_pos] = next_char;                   //update input string 

        //printf("\033[1;%dH",cursor_pos+1);                    //move cursor to cursor pos to update string
        //putchar(next_char);                                   //print new value
        COORD cursor_position = {cursor_pos,0};
        SetConsoleCursorPosition(hConsole,cursor_position);
        WriteConsole(hConsole,&next_char,1,NULL,NULL);    

        switch(direction){
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
        /*
        printf("\033[2;13H");                                   //move to update current state
        printf("\033[0J");                                      //clear from cursor to end of line
        printf("%s",states[curr_state]);
        printf("\033[2;%dH",13+strlen(states[curr_state]));     //move to after newly printed state
        printf("\033[0J");                                      //clear from cursor to end of line
        */
        int new_state_string_size = strlen(states[next_state]);
        char* new_state_string = calloc(state_string_size,sizeof(char));// create array to store message to send
        int clear_buff_size = strlen(states[curr_state]);
        char* clear_buff = calloc(state_string_size,sizeof(char));//buffer to clear prev state
        for(int i=0;i<clear_buff_size;i++){                       //populate clear buff array with whitespace
            *(clear_buff+i) = ' ';
        }
        sprintf(new_state_string,"%s",*(states+next_state));
        COORD new_state_pos = {strlen(state_lable),1};
        SetConsoleCursorPosition(hConsole,new_state_pos);                         
        WriteConsole(hConsole,clear_buff,clear_buff_size,NULL,NULL);
        WriteConsole(hConsole,new_state_string,new_state_string_size,NULL,NULL);    


        curr_state = next_state; 
        custom_delay(1000);
    }
    //printf("\nfinished!");
    
    custom_delay(1000);
    SetConsoleMode(hConsole, originalMode);                     //final thing suggested by windows docs
    return 0;
}

