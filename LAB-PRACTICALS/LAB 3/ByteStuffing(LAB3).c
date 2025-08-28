#include <stdio.h>
#include <string.h>

void byteStuff(char* input, char* output) {
    int j = 0;
    for(int i=0; i<strlen(input); i++) {
        if(input[i] == 'A') {
            output[j++] = 'X';
        }
        output[j++] = input[i];
    }
    output[j] = '\0';
}

int main() {
    char data[] = "ABACADA";    
    char stuffed[20];
    printf("Original: %s\n", data);
    byteStuff(data, stuffed);
    printf("Stuffed: %s\n", stuffed);
    return 0;
}
