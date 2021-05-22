/*
    Testo dell'esercizio:
        Programmare una Macchina di Turing che, dato un nastro iniziale contenente una sequenza di A e B, con
        almeno una B, termina la sua esecuzione lasciando sul nastro la sequenza di sole B consecutive (cioè non
        separate da alcuno spazio) che si ottiene da quella iniziale eliminando tutte le A. Ad esempio:

    start state = 0
    end state = 4

    0 {'A','-',0,'>'},
    0 {'B','*',1,'>'},
    0 {'-','-',4,'-'},

    1 {'A','A',1,'>'},
    1 {'B','B',1,'>'},
    1 {'-','-',2,'>'},
    
    2 {'-','B',3,'<'},
    2 {'B','B',2,'>'},

    3 {'B','B',3,'<'},
    3 {'-','-',3,'<'},
    3 {'A','A',3,'<'},
    3 {'*','-',0,'>'},

*/


const int num_states = 5;

const int final_state = 4;
const int initial_state = 0;

char input_string[] = "-AABBABABAABBB-";

char states[][50] = {"inizio","trovato B","scrivo B","torno indietro","finito","5"};

char work_alphabet[] = {'0','1','2','3','4','5','6','7','8','9','=','-'};
char alfabet_color[][10]  = {"\x1b[35m","\x1b[32m","\x1b[37m"};
const int num_symbols = sizeof(work_alphabet)/sizeof(work_alphabet[0]);
 
//rows are states, columns are input values
//dims are, states,inputs and  are: value to print, nextstate and direction to move
int trans_func[][sizeof(work_alphabet)/sizeof(work_alphabet[0])][4]={
        {                                       //0, 0 1 2 3 4 5 6 7 8 9 = -
            {'A','-',0,'>'},
            {'B','*',1,'>'},
            {'-','-',4,'-'}},                        
        {                                       //1
            {'A','A',1,'>'},
            {'B','B',1,'>'},
            {'-','-',2,'>'}},                    
        {                                       //2
            {'-','B',3,'<'},
            {'B','B',2,'>'}},                      
        {                                       //3
            {'B','B',3,'<'},
            {'-','-',3,'<'},
            {'A','A',3,'<'},
            {'*','-',0,'>'}}       
        };
