/*
    Testo dell'esercizio:
        Programmare una Macchina di Turing che, dato un nastro iniziale contenente una sequenza di A e B,
        termina la sua esecuzione lasciando sul nastro la sequenza ottenuta rovesciando quella iniziale. Ad 
        esempio:

    start state = 0
    end state = 6

    0
    {'A','#',3,'<'},
    {'B','#',4,'<'},
    {'-','-',7,'-'},
    1 stato A 
    {'-','-',3,'<'},
    2 stato B
    {'-','-',4,'<'},
    3
    {'A','A',3,'<'},
    {'B','B',3,'<'},
    {'-','A',5,'>'},
    4
    {'-','B',5,'>'},
    {'A','A',4,'<'},
    {'B','B',4,'<'},
    5
    {'A','A',5,'>'},
    {'B','B',5,'>'},
    {'-','-',5,'>'},
    {'#','-',0,'>'},
    
*/


const int num_states = 7;

const int final_state = 7;
const int initial_state = 0;

char input_string[] = "-AABB-";

char states[][50] = {"inizio","trovato A","trovato B","scrivo A","scrivo B","torno indietro","cancello #","finito"};

char work_alphabet[] = {'A','B','-','#'};
char alfabet_color[][10]  = {"\x1b[35m","\x1b[32m","\x1b[37m"};
const int num_symbols = sizeof(work_alphabet)/sizeof(work_alphabet[0]);
 
//rows are states, columns are input values
//dims are, states,inputs and  are: value to print, nextstate and direction to move
int trans_func[][sizeof(work_alphabet)/sizeof(work_alphabet[0])][4]={
    {
        {'A','#',3,'<'},
        {'B','#',4,'<'},
        {'#','#',0,'>'},
        {'-','-',6,'<'},
    },{ //1 stato A 
        {'-','#',3,'<'},
        {'#','#',3,'<'},
    },{ //2 stato B
        {'-','#',4,'<'},
        {'#','#',4,'<'},
    },{ //3
        {'A','A',3,'<'},
        {'B','B',3,'<'},
        {'#','#',3,'<'},
        {'-','A',5,'>'},
    },{ //4
        {'-','B',5,'>'},
        {'A','A',4,'<'},
        {'#','#',4,'<'},
        {'B','B',4,'<'},
    },{ //5
        {'A','A',5,'>'},
        {'B','B',5,'>'},
        {'#','#',0,'>'},
    },{ //6 
        {'#','-',6,'<'},
        {'A','A',7,'-'},
        {'B','B',7,'-'},
    }};
