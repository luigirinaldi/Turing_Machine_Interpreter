#include <stdio.h>
int main() {
    char string[] = "String";
    char string1[] = "String 1";
    char string_array[2][10] = {"String", "String 1"};

    printf("%s",string_array);
    printf("\n");
    printf("%s",string_array+1);
    return 0;
}