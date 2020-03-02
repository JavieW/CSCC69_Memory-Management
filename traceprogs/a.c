#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int array[100][100];


void generate() {
    for (int i=0;i<100;i++) {
        for (int j=0;j<100;j++) {
            array[i][j] = random();
        }
    }
}

int main(int argc, char* argv[]) {
    generate();
}