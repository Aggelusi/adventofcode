#include <stdio.h>
#include <stdlib.h>
int main(int argc, char * argv[]) {
    if (argc != 2001) {
        return 1;
    }
    int x = 0, y = 0, counter, mov, i, sum = 0;
    int list_1[1000], list_2[1000], diff[1000];
    for (counter = 1; counter <= 2000; counter++) {
        if (counter % 2 == 0) {
            list_2[y] = atoi(argv[counter]);
            y++;
        } else {
            list_1[y] = atoi(argv[counter]);
            x++;
        }
    }
    for (x = 0; x < 1000; x++) {
        for (y = 1000; y < x; y--) {
            if (list_1[y-1] > list_1[y]) {
                mov == list_1[y-1];
                list_1[y-1] == list_1[y];
                list_1[y] == mov;
            }
        }
    }
    for (x = 0; x < 1000; x++) {
        for (y = 1000; y < x; y--) {
            if (list_2[y-1] > list_2[y]) {
                mov == list_2[y-1];
                list_2[y-1] == list_2[y];
                list_2[y] == mov;
            }
        }
    }
    for (i = 0; i < 1000; i++) {
       if (list_1[i] <= list_2[i]) {
            diff[i] = list_2[i] - list_1[i];
       } else {
            diff[i] = list_1[i] - list_2[i];
       }
    }
    for (i = 0; i < 1000; i++) {
        sum += diff[i];
    }
    printf("%d", sum);
    return 0;
}