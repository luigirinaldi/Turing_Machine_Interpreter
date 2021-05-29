#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void custom_delay(int time_in_milliseconds){
    double delay_time = (double)time_in_milliseconds/1000;
    double timediff = 0;
    clock_t curr_time;
    curr_time = clock();
    while(timediff<delay_time){
        timediff = ((double)clock()/CLOCKS_PER_SEC)-((double)curr_time/CLOCKS_PER_SEC);
    }  
}

int find_index(char *state,char **states,int num_states){
    for(int i = 0;i < num_states;i++){
        if(strcmp(state,states[i])==0){
            return i;
        }
    }
    return -1;
}

int find_char(char needle,char *haystack,int size){
    for(int i = 0;i < size;i++){
        if(needle == haystack[i]){
            return i;
        }
    }
    return -1;
}

void read_mdt(char *abs_path,char ***trans_func,int *states_size,char **states_array,char *in_vals, int *alphabet_size,int *state_ins){
    FILE *file;
    file = fopen(abs_path,"r");
    if(file == NULL){
        printf("file read failed! \n");
        return;
    }
    printf("successfully opened file!\n");
    char quintupla = 0;
    char virgola = 0;
    int q_count = 0;

    char quadruple[4] = {0,0,0,0};

    

    //variable length arrays    
    int string_length = 0;
    char *new_string = (char*) calloc(string_length+1,sizeof(char));
    int num_states = 0;
    
    int num_ins = 0;
    int curr_index = 0;

    int prev_state_size = 0;
    char **state_func = (char**) calloc(num_ins+1,sizeof(char*));

    //parameter initialization
    trans_func = (char***) calloc(num_states+1,sizeof(char**));
    states_array = (char**) calloc(num_states+1,sizeof(char*));
    in_vals = (char*) calloc(num_ins+1,sizeof(char));    
    state_ins = (int*) calloc(1,sizeof(int));  
    

    /*char trans_func[500][500][4];
    int state_ins[500] = {0};*/

    char new_char;
    new_char = fgetc(file);
    while(!feof(file)){ //temporarily i, should be !feof(file)
        if(new_char == ','){
            virgola = 1;
        } else if (new_char == '(' && quintupla == 0){
            quintupla = 1;
            //reset vals
            for(int i = 0;i<4;i++){
                quadruple[i] = 0;
            }
        } else if (new_char == ')' && quintupla == 1){      //finished reading quintuple, reset everything   
            /*
            resize state ins array
            resize transfunc 
            resize transfunc[curr_index] to state_ins[curr_index size] with realloc
            assign the new quadruple to transfunc[curr_index][state_ins[curr_index]] 
            */         
            
            //resize state ins array
            state_ins = (int*) realloc(state_ins,num_states*sizeof(int));           

            //resize trans_func
            trans_func = (char***) realloc(trans_func,num_states*sizeof(char**));

            //initialize trans_func and state_ins
            if(prev_state_size != num_states){
                for(int i=prev_state_size;i<num_states;i++){
                    state_ins[i] = 0;           //initialize new values to 0
                    trans_func[i] = (char**) calloc(1,sizeof(char*)); //initialize state array containing quadruples
                }
            }
            prev_state_size = num_states;

            trans_func[curr_index] = (char**) realloc(trans_func[curr_index],(state_ins[curr_index]+1)*sizeof(char*));           
            
            int counter = state_ins[curr_index];
            trans_func[curr_index][counter] = (char*) calloc(4,sizeof(char));
            trans_func[curr_index][counter][0] = quadruple[0];
            trans_func[curr_index][counter][1] = quadruple[1];
            trans_func[curr_index][counter][2] = quadruple[2];
            trans_func[curr_index][counter][3] = quadruple[3];
            state_ins[curr_index]++;
            
            q_count = 0;
            quintupla = 0;
        } else if (quintupla == 1 && q_count < 5){
            if(virgola == 1){
                virgola = 0;
                if(q_count == 0){
                    //this means state has stopped being read, and it can be added to state array
                    curr_index = find_index(new_string,states_array,num_states);
                    if(curr_index == -1){
                        num_states++;
                        states_array = (char**) realloc(states_array,num_states*sizeof(char*));
                        states_array[num_states-1] = new_string;
                        curr_index = num_states-1;
                    }

                    string_length = 0;
                    new_string  = (char*) calloc(string_length+1,sizeof(char));                    
                    new_string[0] = '\0';                
                } else if (q_count == 2){
                    //finished reading the outgoing state
                    int out_index = find_index(new_string,states_array,num_states);
                    if(out_index == -1){
                        num_states++;
                        states_array = (char**) realloc(states_array,num_states*sizeof(char*));
                        states_array[num_states-1] = new_string;
                        out_index = num_states-1;
                    }
                    quadruple[2] = out_index;

                    string_length = 0;
                    new_string  = (char*) calloc(string_length+1,sizeof(char));                    
                    new_string[0] = '\0';       
                }     
                q_count++;
            }
            if(q_count == 0){ 
                //contatore 0, quindi sto leggendo lo stato
                new_string[string_length] = new_char;
                string_length++;
                new_string = (char*) realloc(new_string,(string_length+1)*sizeof(char));  //resize string array to fit next value
                new_string[string_length] ='\0';      //adding terminator char to end string
            } else if (q_count ==1){
                //reading first input 
                quadruple[0] = new_char;

                int in_pos = find_char(new_char,in_vals,num_ins);
                if(in_pos == -1){
                    num_ins++;
                    in_vals = (char*) realloc(in_vals,num_ins*sizeof(char));
                    in_vals[num_ins-1] = new_char;
                    in_pos = num_ins -1;
                }
            } else if (q_count == 2){
                //output state 
                new_string[string_length] = new_char;
                string_length++;
                new_string = (char*) realloc(new_string,(string_length+1)*sizeof(char));  //resize string array to fit next value
                new_string[string_length] ='\0';      //adding terminator char to end string
            } else if (q_count == 3){
                //reading output
                quadruple[1] = new_char;

                int in_pos = find_char(new_char,in_vals,num_ins);
                if(in_pos == -1){
                    num_ins++;
                    in_vals = (char*) realloc(in_vals,num_ins*sizeof(char));
                    in_vals[num_ins-1] = new_char;
                    in_pos = num_ins -1;
                }
            } else if (q_count == 4){
                //direction char
                quadruple[3] = new_char;
            }
        }
        new_char = fgetc(file);
    }
    printf("{");
    for(int i=0;i<num_states;i++){
        int j = 0;
        printf(" { %s:\n",states_array[i]);
        while(j<state_ins[i]){
            printf("    {%c,%c,%s,%c}\n",trans_func[i][j][0],trans_func[i][j][1],states_array[trans_func[i][j][2]],trans_func[i][j][3]);
            j++;
        }        
        printf(" }");
    }
    printf("}\n");
    (*alphabet_size) = num_ins;
    (*states_size) = num_states;
}