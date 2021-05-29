/*
    Testo dell'esercizio:
        Programmare una Macchina di Turing che, dato un nastro iniziale contenente una sequenza di A e B di
        lunghezza dispari, termina la sua esecuzione lasciando sul nastro il simbolo in posizione centrale della
        sequenza iniziale. Ad esempio:io:

    start state = 0
    end state = 6

    0
    {'A','-',1,'>'},
    {'B','-',1,'>'},
    1
    {'A','A',1,'>'},
    {'B','B',1,'>'},
    {'-','-',2,'<'},
    2
    {'A','-',3,'<'},
    {'B','-',3,'<'},
    3
    {'A','A',4,'<'},
    {'B','B',4,'<'},
    4
    {'-','-',6,'-'},
    {'A','A',5,'<'},
    {'B','B',5,'<'},
    5
    {'A','A',5,'<'},
    {'B','B',5,'<'},
    {'-','-',0,'>'},
    
*/


const int num_states = 6;

const int final_state = 6;
const int initial_state = 0;

char input_string[] = "-AABABABAAABAAAABABABA-";

char states[][50] = {"inizio","arrivo fino a -","cancello A o B","salto lettera","controllo che sia finito","loop","torno indietro","finito"};

char work_alphabet[] = {'A','B','-'};
char alfabet_color[][10]  = {"\x1b[35m","\x1b[32m","\x1b[37m"};
const int num_symbols = sizeof(work_alphabet)/sizeof(work_alphabet[0]);
 
//rows are states, columns are input values
//dims are, states,inputs and  are: value to print, nextstate and direction to move
int trans_func[][sizeof(work_alphabet)/sizeof(work_alphabet[0])][4]={
        {                                       //0, 0 1 2 3 4 5 6 7 8 9 = -
            {'A','-',1,'>'},
            {'B','-',1,'>'},                        
        },{                                       //1
            {'A','A',1,'>'},
            {'B','B',1,'>'},
            {'-','-',2,'<'},              
        },{                                       //2
            {'A','-',3,'<'},
            {'B','-',3,'<'},                
        },{                                       //3
            {'A','A',4,'<'},
            {'B','B',4,'<'},
        },{                                       //4
            {'-','-',6,'-'},
            {'A','A',5,'<'},
            {'B','B',5,'<'},
        },{                                       //5
            {'A','A',5,'<'},
            {'B','B',5,'<'},
            {'-','-',0,'>'},
        },};
