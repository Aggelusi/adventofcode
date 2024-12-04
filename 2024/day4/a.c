#include <stdio.h>

int main() {
    FILE *input = fopen("input.txt", "r");
    if (!input) {
        perror("Error opening input");
        return 1;
    }
    char pinax[140][140];
    int i, j, k, word_count = 0;
    for (i = 0; i < 140; i++) {
        for (j = 0; j < 140; j++) {
            fscanf(input, "%c", &pinax[i][j]);
        }
    }
    fclose(input);
    for (i = 0; i < 140; i++) {
        for (j = 0; j < 140; j++) {            
            if (j + 3 < 140) {
                if (pinax[i][j] == 'X' && pinax[i][j+1] == 'M' && pinax[i][j+2] == 'A' && pinax[i][j+3] == 'S') {
                    word_count++;
                }
                if (pinax[i][j] == 'S' && pinax[i][j+1] == 'A' && pinax[i][j+2] == 'M' && pinax[i][j+3] == 'X') {
                    word_count++;
                }
            }
        }
    }
    for (j = 0; j < 140; j++) {
        for (i = 0; i < 140; i++) {            
            if (i + 3 < 140) {
                if (pinax[i][j] == 'X' && pinax[i+1][j] == 'M' && pinax[i+2][j] == 'A' && pinax[i+3][j] == 'S') {
                    word_count++;
                }
                if (pinax[i][j] == 'S' && pinax[i+1][j] == 'A' && pinax[i+2][j] == 'M' && pinax[i+3][j] == 'X') {
                    word_count++;
                }
            }
        }
    }
    
    return 0;
}
