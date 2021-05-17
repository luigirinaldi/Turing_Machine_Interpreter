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
	clock_t time;
	time = clock();
	printf("first time: %ld\n",time);
	for(int i=0;i<1000000;i++){}
	clock_t time1;
	time1 = clock();	
	printf("second time: %ld\n",time1);
	printf("time diff: %ld\n",(time1-time));
	for(int i =0;i<20;i++){	
		custom_delay(100);
		printf("time n. %d\n",i);
	}
	return 0;
}
