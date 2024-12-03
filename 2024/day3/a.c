#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
int main() {
    FILE *input = fopen("input.txt", "r");
    if (!input) {
        perror("Error opening input");
        return 1;
    }
    char *buffer = NULL; // Pointer for the dynamically allocated array
    size_t length = 0;   // Number of characters read
    int ch;
    while ((ch = fgetc(input)) != EOF) {
        // Allocate memory for the next character
        char *temp = (char *)realloc(buffer, (length + 1) * sizeof(char));
        buffer = temp;
        buffer[length++] = (char)ch; // Store the character
    }
    char *temp = (char *)realloc(buffer, (length + 1) * sizeof(char));
    buffer = temp;
    buffer[length] = '\0';
    free(buffer);
    double mul_sum = 0;
    int num1, num2;
for (int i = 0; i < length; i++) {
    if (i + 7 >= length) break;

    if (buffer[i] == 'm' && buffer[i+1] == 'u' && buffer[i+2] == 'l' && buffer[i+3] == '(') {
        int num1 = 0, num2 = 0;
        char *point;
            if (i + 4 < length) {
                num1 = strtol(&buffer[i + 4], &point, 10);
                if (point >= &buffer[length] || *point != ',') continue;
                point++;
            }

            if (point < &buffer[length]) {
                num2 = strtol(point, &point, 10);
                if (point >= &buffer[length] || *point != ')') continue;
            }
        mul_sum += num1 * num2;
        i = point - buffer;
    }
}

    printf("%lld\n", (long long)mul_sum);
    fclose(input);

    return 0;
}