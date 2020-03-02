#include <stdlib.h>

int array[64][64][64];
int randomSum;


void sumRandomPlane() {
    int ran = random()%64;
    int dice = random()%3;

    for (int i=0; i<64; i++) {
        for (int j=0; j<64; j++) {
            if (dice == 0)
                randomSum += array[ran][i][j];
            else if (dice == 1)
                randomSum += array[i][ran][j];
            else
                randomSum += array[i][j][ran];
        }
    }
}

void sumRandomLine() {
    int ran1 = random()%64;
    int ran2 = random()%64;
    int dice = random()%3;

    for (int i=0; i<64; i++) {
        if (dice == 0)
            randomSum += array[ran1][ran2][i];
        else if (dice == 1)
            randomSum += array[ran1][i][ran2];
        else
            randomSum += array[i][ran1][ran2];
    }
}

void sumRandomPoint() {
    int ran1 = random()%64;
    int ran2 = random()%64;
    int ran3 = random()%64;
    randomSum += array[ran1][ran2][ran3];
}

void sumPepeatPoint() {
    int ran1 = random()%64;
    int ran2 = random()%64;
    int ran3 = random()%64;
    for (int i=0; i < 500; i++)
        randomSum += array[ran1][ran2][ran3];
}

int main(int argc, char* argv[]) {
    // sumRandomPlane n times
    for (int i=0; i<120; i++) {
        sumRandomPlane();
    }

    // sumRandomLine n times
    for (int i=0; i<700; i++) {
        sumRandomLine();
    }

    // sumRandomPoint n time;
    for (int i=0; i<50; i++) {
        sumRandomPoint();
    }

    // sumPepeatPoint n time;
    for (int i=0; i<0; i++) {
        sumPepeatPoint();
    }
}