#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool isSafe(int* row, int rowSize) {
    int countBig = 0, countSmall = 0;
    // Check if the report is already safe (either increasing or decreasing)
    for (int j = 0; j < rowSize - 1; j++) {
        if (row[j] < row[j+1]) {
            countBig++;
        } else if (row[j] > row[j+1]) {
            countSmall++;
        }
        // Check the difference between adjacent elements
        if (abs(row[j] - row[j+1]) < 1 || abs(row[j] - row[j+1]) > 3) {
            return false;  // Invalid difference
        }
    }
    return (countSmall == rowSize - 1 || countBig == rowSize - 1);
}

bool isDampenerSafe(int* row, int rowSize) {
    // Try removing one element and check if the report becomes safe
    for (int i = 0; i < rowSize; i++) {
        int *newRow = malloc((rowSize - 1) * sizeof(int));
        int index = 0;
        // Copy the elements, skipping the i-th element
        for (int j = 0; j < rowSize; j++) {
            if (j != i) {
                newRow[index++] = row[j];
            }
        }
        bool safe = isSafe(newRow, rowSize - 1);
        free(newRow);
        if (safe) {
            return true;  // Found a way to make the report safe by removing one element
        }
    }
    return false;
}

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

    int safesum = 0;
    for (int i = 0; i < numRows; i++) {
        // Check if the report is already safe
        if (isSafe(array[i], rowSizes[i])) {
            safesum++;
        } else if (isDampenerSafe(array[i], rowSizes[i])) {
            // Check if removing one element can make it safe
            safesum++;
        }
    }
    printf("%d\n", safesum);
    fclose(input);

    // Free memory
    for (int i = 0; i < numRows; i++) {
        free(array[i]);
    }
    free(array);
    free(rowSizes);

    return 0;
}
