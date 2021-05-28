/*
cosa facciamo

cerco ( e comincio a leggere la quintupla
leggo i char, aggiungendoli alla stringa stato, finchè non trovo la virgola
chiamo funzione "new_state" che mi resituisce indice di stato nell'array degli stati
"new_state" restituisce posisizione se trova lo stato, altrimenti ingradisce l'array e mi restituisce la nuova pos

A questo punto ho l'indice del nuovo stato

ora devo creare quadrupla della funzione di transizione
leggo input e stato succ, output e direzione {<,>,-}
una volta creata la si assegna a all'indice dello stato 

*/

#include <stdio.h>
#include <stdlib.h>

#define MAXSTRSIZE 1000

int main(){
    FILE *file;
    char *filename = "C:\\Users\\Luigi\\Documents\\C_projects\\Test1\\mdt_mul.txt";
    char str[MAXSTRSIZE];

    file = fopen(filename,"r");
    if(file == NULL){
        printf("file read failed! \n");
        return 1;
    }
    char new_char;
    int i = 0;
    char quintupla = 0;
    char virgola = 0;
    int q_count = 0;
    new_char = fgetc(file);
    int string_length = 0;
    char *new_string = (char*) calloc(string_length+1,sizeof(char));
    while(i<10){ //temporarily i, should be !feof(file)
        if(new_char == ','){
            virgola = 1;
        }
        if(new_char == '(' && quintupla == 0){
            quintupla = 1;
        } else (quintupla == 1 && q_count < 5){
            if(virgola ==1){
                q_count++;
            }
            if(q_count == 0){ //contatore 0, quindi sto leggendo lo stato
                new_string[string_length] = new_char;
                char *bigger_string = realloc(*new_string,string_length*sizeof(char));
                memcpy(bigger_string, );
            }
        }
        new_char = fgetc(file);
        putchar('\n');
        i++;
    }
    fclose(file);
    return 0;
}