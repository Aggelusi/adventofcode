#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
int main() {
    FILE *input = fopen("input.txt", "r");
    if (!input) {
        perror("Error opening input");
        return 1;
    }
    int **array = NULL; // 2D array to hold rows
    int *rowSizes = NULL; // Array to store the size of each row
    int numRows = 0; // Total number of rows
    char line[1024]; // Buffer to store each line
    while (fgets(line, sizeof(line), input)) {
        int *row = NULL; // Dynamic array for the current row
        int rowSize = 0; // Number of elements in the current row

        char *token = strtok(line, " \n");
        while (token) {
            row = realloc(row, (rowSize + 1) * sizeof(int));
            if (!row) {
                perror("Memory allocation failed");
                fclose(input);
                return 1;
            }
            row[rowSize++] = atoi(token); // Convert string to integer
            token = strtok(NULL, " \n");
        }

        array = realloc(array, (numRows + 1) * sizeof(int *));
        rowSizes = realloc(rowSizes, (numRows + 1) * sizeof(int));
        if (!array || !rowSizes) {
            perror("Memory allocation failed");
            fclose(input);
            return 1;
        }

        array[numRows] = row;
        rowSizes[numRows++] = rowSize;
    }
    int safesum = 0, countbig, countsmall, countabs;
    bool forward = false;
    for (int i = 0; i < numRows; i++) {
        countbig = 0;
        countsmall = 0;
        countabs = 0;
        for (int j = 0; j < rowSizes[i] - 1; j++) {
            if (array[i][j] < array[i][j+1]) {
                countbig++;
            } else if (array[i][j] > array[i][j+1]) {
                countsmall++;
            }
        }
        if (countsmall == rowSizes[i] -1 || countbig == rowSizes[i] - 1) {
            for (int j = 0; j < rowSizes[i] - 1; j++) {
                if (abs(array[i][j] - array[i][j+1]) <= 3 && abs(array[i][j] - array[i][j+1]) >= 1) {
                    countabs++;
                }
            }
            if (countabs == rowSizes[i] - 1) {
                forward = true;
            }
        }
        if (forward == true) safesum++;
    }
    printf("%d\n", safesum);
    fclose(input);
    // Print the array for verification

    for (int i = 0; i < numRows; i++) {
        free(array[i]);
    }
    free(array);
    free(rowSizes);

    return 0;
}
