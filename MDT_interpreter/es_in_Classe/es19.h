/*
    Testo dell'esercizio:
        Programmare una Macchina di Turing che, dato un nastro iniziale contenente una sequenza di A e B, termina
        la sua esecuzione lasciando sul nastro la sequenza che si ottiene da quella iniziale rimpiazzando due o più A
        consecutive con una sola A e due o più B consecutive con una sola B. Ad esempio:

    start state = 0
    end state = 4


    0 
        {'A','A',1,'>'},
        {'B','B',2,'>'},
        {'-','-',7,'-'},

    stato A
    1 
        {'A','*',3,'>'},
        {'B','B',0,'-'},
    
    stato B
    2 
        {'B','*',3,'>'},
        {'A','A',0,'-'},

    3 
        {'A','*',4,'<'},
        {'B','*',5,'<'},
        {'*','*',3,'>'},
        {'-','-',6,'<'},

    4 
        {'*','A',3,'>'},

    5 
        {'*','B',3,'>'},

    6
        {'*','-',6,'<'},
        {'A','A',6,'<'},
        {'B','B',6,'<'},
        {'-','-',0,'>'},
*/


const int num_states = 5;

const int final_state = 7;
const int initial_state = 0;

char input_string[] = "-AABBABABAABBB-";

char states[][50] = {"inizio","controllo A","controllo B","cancello doppio","cancello A","cancello B","torno indietro","finito"};

char work_alphabet[] = {'0','1','2','3','4','5','6','7','8','9','=','-'};
char alfabet_color[][10]  = {"\x1b[35m","\x1b[32m","\x1b[37m"};
const int num_symbols = sizeof(work_alphabet)/sizeof(work_alphabet[0]);
 
//rows are states, columns are input values
//dims are, states,inputs and  are: value to print, nextstate and direction to move
int trans_func[][sizeof(work_alphabet)/sizeof(work_alphabet[0])][4]={
        {                                       //0, 0 1 2 3 4 5 6 7 8 9 = -
            {'A','A',1,'>'},
            {'B','B',2,'>'},
            {'-','-',7,'-'}},                        
        {                                       //1
            {'A','*',3,'>'},
            {'B','B',0,'-'},
            {'-','-',7,'-'}},                
        {                                       //2
            {'B','*',3,'>'},
            {'A','A',0,'-'},
            {'-','-',7,'-'}},                   
        {                                       //3
            {'A','*',4,'<'},
            {'B','*',5,'<'},
            {'*','*',3,'>'},
            {'-','-',6,'<'}},
        {                                       //4
            {'*','A',3,'>'}},
        {                                       //5
            {'*','B',3,'>'}},
        {                                       //6
            {'*','-',6,'<'},
            {'A','A',6,'<'},
            {'B','B',6,'<'},
            {'-','-',0,'>'}}
        };
