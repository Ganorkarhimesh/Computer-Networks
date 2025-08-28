#include <stdio.h>

void bitStuff(char* input, char* output) {
    int count = 0;
    int j = 0;
    for(int i=0; input[i]!='\0'; i++) {
        output[j++] = input[i];
        if(input[i] == '1') {
            count++;
            if(count == 5) {
                output[j++] = '0';
                count = 0;
            }
        } else {
            count = 0;
        }
    }
    output[j] = '\0';
}

int main() {
    char input[] = "111110111111";
    char stuffed[20];
    printf("Original: %s\n", input);
    bitStuff(input, stuffed);
    printf("After stuffing: %s\n", stuffed);
    return 0;
}