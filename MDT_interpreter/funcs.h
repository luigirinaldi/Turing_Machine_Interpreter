#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>

void pause_screen(){
    printf("Press enter to continue");
    char enter = 0;
    enter = getchar();
    enter = getchar();
    while (enter != '\r' && enter != '\n') { enter = getchar(); }
}

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

#define num_args 4 //need to use define as it is for array initializing

int read_options(char *abs_path, int opts_out[2],char **initial_string_out, char q_format_out[5]){
    /*
        reads options between curly brackets, options can be:
            1 initial_string,
            2 trailing_lambdas (y or n),
            3 delay (in ms),
            4 quintuple_format,
            and more to come (initial pos,lambda value)
        arg name is delimited by ':'
        arg value is delimited by comma hence, trailing comma is needed
    */
    FILE *file;
    file = fopen(abs_path,"r");
    if(file == NULL){
        printf("file read failed! \n");
    }
    //printf("successfully opened file!\n");

    //control variables
    int crucial_args = -3; //integer to be incremented each time a crucial arg is found, if it reaches 0 then all important args are present
    char found_opts = 0;
    char reading_q = 0;
    char q_count = 0;
    char read_state = 0; //0 is reading the arg name, other point to value type
    char *new_string = (char*) calloc(1,sizeof(char));
    int string_len = 0;
    char arg_names[num_args][50] = {
        "initial_string",
        "trailing_lambdas",
        "delay",
        "quintuple_format",
    };

    char new_char;
    new_char = fgetc(file);
    while(new_char != '}'){
        if(new_char == '{'){
            found_opts = 1;
        } else if (found_opts == 1 && new_char != ' ' && new_char != '\n'){
            if(read_state == 0){
                if(new_char == ':'){
                    //arg name finished, find out what arg it is
                    for (int i = 0;i<num_args;i++){
                        if(strcmp(new_string,arg_names[i]) == 0){
                            read_state = i+1;
                            break;
                        }
                    }
                    string_len = 0;
                    new_string = (char*) realloc(new_string, (string_len+1)*sizeof(char));
                    new_string[string_len] = '\0'; //terminating string 
                } else {
                    //reading arg name 
                    new_string[string_len] = new_char;
                    string_len++;
                    new_string = (char*) realloc(new_string, (string_len+1)*sizeof(char)); //making array larger
                    new_string[string_len] = '\0'; //terminating string 
                }
            } else {
                //argument values
                switch(read_state)
                {
                    case(1): {
                        //initial_string
                        if(new_char != ','){

                            new_string[string_len] = new_char;
                            string_len++;
                            new_string = (char*) realloc(new_string, (string_len+1)*sizeof(char)); //making array larger
                            new_string[string_len] = '\0'; //terminating string 
                        } else{                            
                            crucial_args++;
                            (*initial_string_out) = new_string;

                            read_state = 0;

                            string_len = 0;
                            new_string = (char*) calloc(string_len+1,sizeof(char));
                            new_string[string_len] = '\0'; //terminating string 
                        }
                        break;
                    }
                    case(2): {
                        //trailing lambdas
                        if(new_char == 'y'){
                            opts_out[0] = 1;
                        } else if(new_char == 'n'){
                            opts_out[0] = 0;
                        } else if(new_char == ','){
                            //wait for comma to advance read state
                            read_state = 0;
                        }
                        break;
                    }
                    case (3): {
                        //delay 
                        if(new_char == ','){
                            crucial_args++;
                            opts_out[1] = atoi(new_string);

                            read_state = 0;

                            string_len = 0;
                            new_string = (char*) realloc(new_string, (string_len+1)*sizeof(char));
                            new_string[string_len] = '\0'; //terminating string 
                        } else {
                            new_string[string_len] = new_char;
                            string_len++;
                            new_string = (char*) realloc(new_string, (string_len+1)*sizeof(char)); //making array larger
                            new_string[string_len] = '\0'; //terminating string 
                        }
                        break;
                    }
                    case(4): {
                        //quintuple format
                        if(new_char == ','){
                            crucial_args++;
                            read_state = 0;
                        } else{
                            if(new_char == '[' && reading_q == 0){
                                reading_q = 1;
                            } else if (new_char == ';'){
                                reading_q = 1;
                                q_count++;
                            } else if (reading_q == 1 && q_count < 5){
                                q_format_out[q_count] = new_char;
                                reading_q = 0;
                            }
                        }
                        break;
                    }
                }
            }
        }
        //putchar(new_char);
        new_char = fgetc(file);
    }
    fclose(file);
    //printf("finished parsing options %s\n",abs_path);

    return crucial_args;
}

void read_mdt(char q_format[5],char *abs_path,char ****trans_func_out,int *states_size,char ***states_array_out,char **in_vals_out, int *alphabet_size,int **state_ins_out){
    /*
        q format is composed of:
        s(input state),i(input char),S(output state),o(output char) and d(direction)
    */   


    FILE *file;
    errno = 0;
    file = fopen(abs_path,"r");
    if(file == NULL){
        printf("file read failed for %s \n",abs_path);
        printf("error %s \n",strerror(errno));
        return;
    }
    //printf("successfully opened file!\n");
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
    char ***trans_func = (char***) calloc(num_states+1,sizeof(char**));
    char **states_array = (char**) calloc(num_states+1,sizeof(char*));
    char *in_vals = (char*) calloc(num_ins+1,sizeof(char));    
    int *state_ins = (int*) calloc(1,sizeof(int));  
    

    /*char trans_func[500][500][4];
    int state_ins[500] = {0};*/

    char new_char;
    new_char = fgetc(file);
    while(!feof(file)){ //temporarily i, should be !feof(file)
        if(new_char == ','){
            virgola = 1;
        }
        if (new_char == '(' && quintupla == 0){
            quintupla = 1;
            virgola = 0;
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
                if(q_format[q_count] == 's'){
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
                } else if (q_format[q_count] == 'S'){
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

                q_count++; //increments q_counter, to know what the next value should be interpreted as
            }
            else {
            switch (q_format[q_count])
            {
            case ('s'):
            {
                //reading current state
                new_string[string_length] = new_char;
                string_length++;
                new_string = (char*) realloc(new_string,(string_length+1)*sizeof(char));  //resize string array to fit next value
                new_string[string_length] ='\0';      //adding terminator char to end string
                break;
            }
            case ('i'):
            {
                //reading first input 
                quadruple[0] = new_char;

                int in_pos = find_char(new_char,in_vals,num_ins);
                if(in_pos == -1){
                    num_ins++;
                    in_vals = (char*) realloc(in_vals,num_ins*sizeof(char));
                    in_vals[num_ins-1] = new_char;
                    in_pos = num_ins -1;
                }
                break;
            }
            case ('S'):
            {
                //reading outgoing state
                new_string[string_length] = new_char;
                string_length++;
                new_string = (char*) realloc(new_string,(string_length+1)*sizeof(char));  //resize string array to fit next value
                new_string[string_length] ='\0';      //adding terminator char to end string
                break;
            }
            case ('o'):
            {
                //reading outgoing char
                quadruple[1] = new_char;
                //add to alphabet 
                int in_pos = find_char(new_char,in_vals,num_ins);
                if(in_pos == -1){
                    num_ins++;
                    in_vals = (char*) realloc(in_vals,num_ins*sizeof(char));
                    in_vals[num_ins-1] = new_char;
                    in_pos = num_ins -1;
                }
                break;
            }
            case ('d'):
            {
                //reading direction
                quadruple[3] = new_char;
                break;
            }
            default:
                break;
            }
        }
        }
        new_char = fgetc(file);
    }

    
    //printing stuff
    printf("{");
    for(int i=0;i<num_states;i++){
        int j = 0;
        printf(" {\n  %s:\n",states_array[i]);
        while(j<state_ins[i]){
            printf("    {%c,%c,%s,%c}\n",trans_func[i][j][0],trans_func[i][j][1],states_array[trans_func[i][j][2]],trans_func[i][j][3]);
            j++;
        }     
        if(i == num_states-1){   
            printf(" }");
        } else{
            printf(" },");
        }
    }
    printf("}\n");

    printf("work alphabet: \n[");
    for(int i =0;i<num_ins;i++){
        printf("'%c'",in_vals[i]);
        if(i == num_ins-1){   
            printf("");
        } else{
            printf(",");
        }
    }
    printf("]\n");

    printf("states: \n[");
    for(int i =0;i<num_states;i++){
        printf("'%s'",states_array[i]);
        if(i == num_states-1){   
            printf("");
        } else{
            printf(",");
        }
    }
    printf("]\n");


    fclose(file);
    (*trans_func_out) = trans_func;
    (*states_array_out) = states_array;
    (*state_ins_out) = state_ins;
    (*in_vals_out) = in_vals;
    (*alphabet_size) = num_ins;
    (*states_size) = num_states;
}