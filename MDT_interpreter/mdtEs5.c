/*
    Testo dell'esercizio:
        Progettare una macchina di turing che, data in ingresso 
        una sequenza di simobli 'A', sostituisca quelli in posto
        dispari con i simboli 'B'

*/

#include <stdio.h>
#include <string.h>
#include <time.h>

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
    char input_string[100] = "test";
    printf("\e[1;1H\e[2J"); 
    print_string(input_string);
    for(int i = 0; i < 10;i++){
        input_string[i+4] = i+'0';
        custom_delay(100);
        printf("\033[%d;%dH",i+1,i+1);
        print_string(input_string);
    }

/*
    custom_delay(1000);
    printf("\e[1;1H\e[2J");     //regex clear terminal
    printf("second message");
*/
    return 0;
}

