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

    char *buffer = NULL;
    size_t length = 0;
    int ch;
    while ((ch = fgetc(input)) != EOF) {
        char *temp = realloc(buffer, length + 2);
        if (!temp) {
            perror("Memory allocation failed");
            free(buffer);
            fclose(input);
            return 1;
        }
        buffer = temp;
        buffer[length++] = (char)ch;
    }
    if (buffer) buffer[length] = '\0';

    fclose(input);

    bool mul_enabled = true;
    double mul_sum = 0;
    for (size_t i = 0; i < length; i++) {
        if (i + 7 >= length) break;

        if (buffer[i] == 'm' && buffer[i+1] == 'u' && buffer[i+2] == 'l' && buffer[i+3] == '(') {
            if (mul_enabled == true) {
                int num1 = 0, num2 = 0;
                char *endPtr;

                num1 = strtol(&buffer[i + 4], &endPtr, 10);
                if (*endPtr != ',') continue;
                endPtr++;

                num2 = strtol(endPtr, &endPtr, 10);
                if (*endPtr != ')') continue;

                mul_sum += num1 * num2;

                i = endPtr - buffer;
            }
        }

        if (i + 3 < length && strncmp(&buffer[i], "do(", 3) == 0) {
            mul_enabled = true;
            i += 2;
        }

        if (i + 5 < length && strncmp(&buffer[i], "don't()", 7) == 0) {
            mul_enabled = false;
            i += 6;
        }
    }

    printf("Sum of all enabled multiplications: %.0f\n", mul_sum);

    free(buffer);
    return 0;
}
