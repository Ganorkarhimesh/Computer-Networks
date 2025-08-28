#include <stdio.h>
#include <string.h>

void calculateCRC(char data[], char divisor[]) {
    int data_len = strlen(data);
    int div_len = strlen(divisor);
    char remainder[div_len + 1];
    char padded_data[strlen(data) + div_len];
    strcpy(padded_data, data);
    for(int i=0; i<div_len-1; i++) {
        strcat(padded_data, "0");
    }
    strncpy(remainder, padded_data, div_len);
    remainder[div_len] = '\0';
    for(int i=0; i<data_len; i++) {
        if(remainder[0] == '1') {
            for(int j=1; j<div_len; j++) {
                remainder[j-1] = (remainder[j] == divisor[j]) ? '0' : '1';
            }
        }
        remainder[div_len-1] = padded_data[i+div_len];
    }
    printf("\nFinal CRC remainder: %s\n", remainder);
}

int main() {
    char data[] = "10110011";
    char divisor[] = "1101";
    printf("Data: %s\n", data);
    printf("Divisor: %s\n", divisor);
    printf("\nCRC Calculation Steps:\n");
    calculateCRC(data, divisor);
    return 0;
}
