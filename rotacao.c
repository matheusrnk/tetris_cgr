#include <stdio.h>
#include <stdlib.h>

#define MATRIX_SIZE 4

void rotateMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    int temp[MATRIX_SIZE][MATRIX_SIZE];

    // Copy the original matrix to a temporary matrix
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            temp[i][j] = matrix[i][j];
        }
    }

    // Perform the rotation
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix[i][j] = temp[MATRIX_SIZE - j - 1][i];
        }
    }
}

void printMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int matrix[MATRIX_SIZE][MATRIX_SIZE] = {
        {1, 0, 0, 0},
        {1, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0}
    };

    int matrix2[MATRIX_SIZE][MATRIX_SIZE] = {
        {0, 0, 0, 1},
        {0, 0, 0, 1},
        {0, 0, 1, 1},
        {0, 0, 0, 0}
    };

    int matrix3[MATRIX_SIZE][MATRIX_SIZE] = {
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };

    int matrix4[MATRIX_SIZE][MATRIX_SIZE] = {
        {0, 1, 1, 0},
        {0, 0, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };

    int matrix5[MATRIX_SIZE][MATRIX_SIZE] = {
        {1, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };

    int matrix6[MATRIX_SIZE][MATRIX_SIZE] = {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    };

    int matrix7[MATRIX_SIZE][MATRIX_SIZE] = {
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0}
    };



    printf("Original Matrix:\n");
    printMatrix(matrix7);

    rotateMatrix(matrix7);
    rotateMatrix(matrix7);
    rotateMatrix(matrix7);

    printf("\nRotated Matrix:\n");
    printMatrix(matrix7);



    return 0;
}
