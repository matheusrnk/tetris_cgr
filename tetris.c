// gcc tetris.c -lglut -lGL -lGLU -lm -o tetris && ./tetris

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <GL/glut.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

GLuint texture;
int imageWidth, imageHeight, imageChannels;

#define TRUE 1
#define FALSE 0

#define NOT_PRESSED 2
#define PRESSED 1

/**
 * @brief Tamanho do tabuleiro
 * 
*/
#define BOARD_WIDTH  10
#define BOARD_HEIGHT 20

/**
 * @brief Estado do jogo e afins
 * 
 */

unsigned long score_counter = 0; //MAX 10
int line_counter  = 0;
int level_counter = 0;

int board[BOARD_HEIGHT][BOARD_WIDTH];

/**
 * 
 * 
 * 
 * 
 * 
 * 
 */

#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 640

#define MTETRO_SIZE 4
/**
 * @brief Desenho das peças em forma matricial
 * para inserí-las no tabuleiro
*/
int MTETRO_I[MTETRO_SIZE][MTETRO_SIZE] = {
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0}
};

int MTETRO_SQR[MTETRO_SIZE][MTETRO_SIZE] = {
    {0, 0, 0, 0},
    {0, 2, 2, 0},
    {0, 2, 2, 0},
    {0, 0, 0, 0}
};

int MTETRO_LL[MTETRO_SIZE][MTETRO_SIZE] = {
    {0, 0, 3, 0},
    {0, 0, 3, 0},
    {0, 3, 3, 0},
    {0, 0, 0, 0}
};

int MTETRO_LR[MTETRO_SIZE][MTETRO_SIZE] = {
    {0, 4, 0, 0},
    {0, 4, 0, 0},
    {0, 4, 4, 0},
    {0, 0, 0, 0}
};

int MTETRO_SL[MTETRO_SIZE][MTETRO_SIZE] = {
    {0, 0, 0, 0},
    {0, 5, 5, 0},
    {0, 0, 5, 5},
    {0, 0, 0, 0}
};

int MTETRO_SR[MTETRO_SIZE][MTETRO_SIZE] = {
    {0, 0, 0, 0},
    {0, 6, 6, 0},
    {6, 6, 0, 0},
    {0, 0, 0, 0}
};

int MTETRO_T[MTETRO_SIZE][MTETRO_SIZE] = {
    {0, 0, 0, 0},
    {7, 7, 7, 0},
    {0, 7, 0, 0},
    {0, 0, 0, 0}
};

/**
 * @brief SAFE COPIES
 * 
 */
const int SC_MTETRO_I[MTETRO_SIZE][MTETRO_SIZE] = {
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0}
};

const int SC_MTETRO_SQR[MTETRO_SIZE][MTETRO_SIZE] = {
    {0, 0, 0, 0},
    {0, 2, 2, 0},
    {0, 2, 2, 0},
    {0, 0, 0, 0}
};

const int SC_MTETRO_LL[MTETRO_SIZE][MTETRO_SIZE] = {
    {0, 0, 3, 0},
    {0, 0, 3, 0},
    {0, 3, 3, 0},
    {0, 0, 0, 0}
};

const int SC_MTETRO_LR[MTETRO_SIZE][MTETRO_SIZE] = {
    {0, 4, 0, 0},
    {0, 4, 0, 0},
    {0, 4, 4, 0},
    {0, 0, 0, 0}
};

const int SC_MTETRO_SL[MTETRO_SIZE][MTETRO_SIZE] = {
    {0, 0, 0, 0},
    {0, 5, 5, 0},
    {0, 0, 5, 5},
    {0, 0, 0, 0}
};

const int SC_MTETRO_SR[MTETRO_SIZE][MTETRO_SIZE] = {
    {0, 0, 0, 0},
    {0, 6, 6, 0},
    {6, 6, 0, 0},
    {0, 0, 0, 0}
};

const int SC_MTETRO_T[MTETRO_SIZE][MTETRO_SIZE] = {
    {0, 0, 0, 0},
    {7, 7, 7, 0},
    {0, 7, 0, 0},
    {0, 0, 0, 0}
};

/**
 * @brief Possíveis estados dos tetrominos
 * sendo utilizados
 * 
 */
enum Tetrominos {
    INIT_STATE = -1,
    TETRO_I    = 1,
    TETRO_SQR  = 2,
    TETRO_LL   = 3,
    TETRO_LR   = 4,
    TETRO_SL   = 5,
    TETRO_SR   = 6,
    TETRO_T    = 7
};

/**
 * @brief Guardam o estado dos tetrominos
 * 
 */
enum Tetrominos nextTetromino    = INIT_STATE;
enum Tetrominos currentTetromino = INIT_STATE;

/**
 * @brief Dimensões da caixas
 * da interface
 * 
 */
const int d0[] = {260, 440};
const int d1[] = {240, 200};
const int d2[] = {260, 200};
const int d3[] = {200, 120};
const int d4[] = {200, 160};
const int d5[] = {200, 100};
const int d6[] = {260, 140};
#define BOTTOM_RIGHT_COMPONENT_DIMENSIONS d0
#define TOP_LEFT_COMPONENT_DIMENSIONS d1
#define TOP_RIGHT_COMPONENT_DIMENSIONS d2
#define SCORE_BOX_DIMENSIONS d3
#define NEXT_BOX_DIMENSIONS d4
#define LINE_BOX_DIMENSIONS d5
#define LEVEL_BOX_DIMENSIONS d5
#define GAMEOVER_BOX_DIMENSIONS d6

/**
 * @brief Espaçamento do texto
 * desenhado na tela
 * 
*/
#define TEXT_SPACING 22

/**
 * @brief Definição de cores para serem usadas
 * como macros.
 * 
*/
static const GLfloat c0[] = {0.0f, 1.0f, 1.0f};
static const GLfloat c1[] = {1.0f, 1.0f, 0.0f};
static const GLfloat c2[] = {0.0f, 0.0f, 1.0f};
static const GLfloat c3[] = {1.0f, 0.5f, 0.0f};
static const GLfloat c4[] = {1.0f, 0.0f, 0.0f};
static const GLfloat c5[] = {0.0f, 1.0f, 0.0f};
static const GLfloat c6[] = {0.35f, 0.0f, 0.35f};
static const GLfloat c7[] = {0.5f, 0.5f, 0.5f};
static const GLfloat c8[] = {0.0f, 0.0f, 0.0f};
static const GLfloat c9[] = {1.0f, 1.0f, 1.0f};
#define LIGHT_BLUE c0
#define YELLOW c1
#define BLUE c2
#define ORANGE c3
#define RED c4
#define GREEN c5
#define PURPLE c6
#define GRAY c7
#define BLACK c8
#define WHITE c9

/**
 * @brief Tamanho de uma unidade de bloco
 */
#define BLOCK_SIZE 20

int timeBased = TRUE;

/**
 * @brief Escolhe um tetromino aleatorio
 * 
 * @return int valor do tetromino
 */
static int chooseRandomTetro(){
    time_t t;
    if(timeBased) { 
        srand((unsigned) time(&t)); timeBased = FALSE; 
    } else timeBased = TRUE;
    return (rand() % (TETRO_T - TETRO_I + 1)) + TETRO_I;
}

/**
 * @brief Transforma um 'unsigned long' em um
 * vetor de char (string).
 * 
 * @param str ponteiro para o vetor que receberá a string convertida
 * @param value valor que será convertido para string
 */
void ulitoa(char *str, unsigned long value){
    int i, rem, len = 0; 
    unsigned long n;

    if(value == 0){
        str[0] = '0';
        str[1] = '\0';
        return;
    }
 
    n = value;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = value % 10;
        value = value / 10;
        str[len - (i + 1)] = rem + '0';
    }

    str[len] = '\0';
}

void imprimeTabuleiroAtual(){
    for(int i = 0; i < BOARD_HEIGHT; i++){
        printf("[");
        for(int j = 0; j < BOARD_WIDTH; j++){
            if(j == BOARD_WIDTH - 1) printf("%d", board[i][j]);
            else printf("%d,", board[i][j]);
        }
        printf("]\n");
    }
    printf("\n");
}

void rotateMatrix(int matrix[MTETRO_SIZE][MTETRO_SIZE]) {
    int temp[MTETRO_SIZE][MTETRO_SIZE];

    // Copy the original matrix to a temporary matrix
    for (int i = 0; i < MTETRO_SIZE; i++) {
        for (int j = 0; j < MTETRO_SIZE; j++) {
            temp[i][j] = matrix[i][j];
        }
    }

    // Perform the rotation
    for (int i = 0; i < MTETRO_SIZE; i++) {
        for (int j = 0; j < MTETRO_SIZE; j++) {
            matrix[i][j] = temp[MTETRO_SIZE - j - 1][i];
        }
    }
}

/**
 * @brief Função base para os tetraminos,
 * onde é desenhado apenas uma unidade de bloco.
 *      _
 *     |_|
 * 
 * @param x Posição horizontal do bloco
 * @param y Posição vertical do bloco
 * @param color Cor que o bloco será colorido
 */
void drawBlock(int x, int y, const GLfloat *color) {
    glColor3fv(color);
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + BLOCK_SIZE, y);
    glVertex2i(x + BLOCK_SIZE, y + BLOCK_SIZE);
    glVertex2i(x, y + BLOCK_SIZE);
    glEnd();

    glColor3f(color[0]/2, color[1]/2, color[2]/2);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x, y);
    glVertex2i(x + BLOCK_SIZE, y);
    glVertex2i(x + BLOCK_SIZE, y + BLOCK_SIZE);
    glVertex2i(x, y + BLOCK_SIZE);
    glEnd();
}

/**
 * @brief Funcao que desenha o tabuleiro 
 * na tela
 * 
 */
void drawBoard(){
    // chao
    for(int i = 0; i < BOARD_WIDTH + 2; i++){
        drawBlock(i * BLOCK_SIZE, 0, GRAY);
    }

    //lateral esquerda
    for(int i = 0; i < BOARD_HEIGHT + 1; i++){
        drawBlock(0, (i+1) * BLOCK_SIZE, GRAY);
    }

    //lateral direita
    for(int i = 0; i < BOARD_HEIGHT + 1; i++){
        drawBlock((BOARD_WIDTH + 1) * BLOCK_SIZE, (i+1) * BLOCK_SIZE, GRAY);
    }

    //teto
    for(int i = 0; i < BOARD_WIDTH; i++){
        drawBlock((i+1) * BLOCK_SIZE, (BOARD_HEIGHT + 1) * BLOCK_SIZE, GRAY);
    }
}

/**
 * @brief Desenha qualquer texto na tela
 * 
 * @param text O texto a ser desenhado
 * @param font A fonte do texto (inclui tamanho)
 * @param position Posição onde o texto ficará na tela; {x, y}
 * @param color A cor a ser tomada pelo texto
 * 
 */
void drawText(char *text, void *font, GLuint *position, const GLfloat *color){

    glColor3fv(color);

    size_t text_len = strlen(text);
    size_t j = 0;
    for(size_t i = position[0]; j < text_len; i = i + TEXT_SPACING){
        glRasterPos2i(i, position[1]);
        glutBitmapCharacter(font, text[j]);
        j++;
    }
}

/**
 * @brief Função que desenha a logo do jogo
 * 
 * @attention Esta função ainda está em construção.
 * Ou seja, isto é apenas um placeholder
 */
/*void drawLogo(){
    GLuint position[] = {(BOARD_WIDTH + 2) * BLOCK_SIZE + 90, (BOARD_HEIGHT + 2) * BLOCK_SIZE + 100};
    drawText("LOGO", GLUT_BITMAP_TIMES_ROMAN_24, position, WHITE);
}*/

void drawLogo(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    int posX = BLOCK_SIZE * 4;
    int posY = BLOCK_SIZE * 4;

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(-posX + (BLOCK_SIZE * (BOARD_WIDTH + 8.5)), -posY + (BLOCK_SIZE * (BOARD_HEIGHT + 6.5)));

    glTexCoord2f(1, 0);
    glVertex2f(posX + (BLOCK_SIZE * (BOARD_WIDTH + 8.5)), -posY + (BLOCK_SIZE * (BOARD_HEIGHT + 6.5)));

    glTexCoord2f(1, 1);
    glVertex2f(posX  + (BLOCK_SIZE * (BOARD_WIDTH + 8.5) ) , posY + (BLOCK_SIZE * (BOARD_HEIGHT + 6.5)));

    glTexCoord2f(0, 1);
    glVertex2f(-posX + (BLOCK_SIZE * (BOARD_WIDTH + 8.5)), posY + (BLOCK_SIZE * (BOARD_HEIGHT + 6.5)));
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

/**
 * @brief Função que desenha um retângulo qualquer
 * na tela
 * 
 * @param x Poisção horizontal do retângulo
 * @param y Posição vertical do retângulo
 * @param dimension Dimensões do retângulo; {width, height}
 * @param color Cor que o retângulo possuirá
 */
void drawRectangle(int x, int y, const int *dimension, const GLfloat *color) {
    glColor3fv(color);
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + dimension[0], y);
    glVertex2i(x + dimension[0], y + dimension[1]);
    glVertex2i(x, y + dimension[1]);
    glEnd();
}

void drawScoreBox(){
    GLuint position[] = {BLOCK_SIZE + 45, (BOARD_HEIGHT + 2) * BLOCK_SIZE + 130};
    drawRectangle(BLOCK_SIZE, (BOARD_HEIGHT + 2) * BLOCK_SIZE + 40, SCORE_BOX_DIMENSIONS, BLACK);
    drawText("SCORE", GLUT_BITMAP_TIMES_ROMAN_24, position, WHITE);
}

void drawNextBox(){
    GLuint position[] = {(BOARD_WIDTH + 2) * BLOCK_SIZE + 85, (BOARD_HEIGHT + 2) * BLOCK_SIZE - 50};
    drawRectangle((BOARD_WIDTH + 2) * BLOCK_SIZE + 30, (BOARD_HEIGHT + 2) * BLOCK_SIZE - 180, NEXT_BOX_DIMENSIONS, BLACK);
    drawText("NEXT", GLUT_BITMAP_TIMES_ROMAN_24, position, WHITE);
}

void drawLineBox(){
    GLuint position[] = {(BOARD_WIDTH + 2) * BLOCK_SIZE + 90, (BOARD_HEIGHT + 2) * BLOCK_SIZE - 230};
    drawRectangle((BOARD_WIDTH + 2) * BLOCK_SIZE + 30, (BOARD_HEIGHT + 2) * BLOCK_SIZE - 300, LINE_BOX_DIMENSIONS, BLACK);
    drawText("LINE", GLUT_BITMAP_TIMES_ROMAN_24, position, WHITE);
}

void drawLevelBox(){
    GLuint position[] = {(BOARD_WIDTH + 2) * BLOCK_SIZE + 80, (BOARD_HEIGHT + 2) * BLOCK_SIZE - 350};
    drawRectangle((BOARD_WIDTH + 2) * BLOCK_SIZE + 30, (BOARD_HEIGHT + 2) * BLOCK_SIZE - 420, LEVEL_BOX_DIMENSIONS, BLACK);
    drawText("LEVEL", GLUT_BITMAP_TIMES_ROMAN_24, position, WHITE);
}

void drawScoreCounterOnBox(){
    char *score_to_string = (char*) malloc(10 * sizeof(char));
    ulitoa(score_to_string, score_counter);
    int sl = strlen(score_to_string);
    GLuint position[] = {BLOCK_SIZE + 92 - ((sl-1) * 10), (BOARD_HEIGHT + 2) * BLOCK_SIZE + 85};
    drawText(score_to_string, GLUT_BITMAP_TIMES_ROMAN_24, position, WHITE);
    free(score_to_string);
}

void drawLineCounterOnBox(){
    char *line_to_string = (char*) malloc(10 * sizeof(char));
    ulitoa(line_to_string, line_counter);
    int ll = strlen(line_to_string);
    GLuint position[] = {(BOARD_WIDTH + 2) * BLOCK_SIZE + 122 - ((ll-1) * 10), (BOARD_HEIGHT + 2) * BLOCK_SIZE - 270};
    drawText(line_to_string, GLUT_BITMAP_TIMES_ROMAN_24, position, WHITE);
    free(line_to_string);
}

void drawLevelCounterOnBox(){
    char *level_to_string = (char*) malloc(10 * sizeof(char));
    ulitoa(level_to_string, level_counter);
    int lvl = strlen(level_to_string);
    GLuint position[] = {(BOARD_WIDTH + 2) * BLOCK_SIZE + 122 - ((lvl-1) * 10), (BOARD_HEIGHT + 2) * BLOCK_SIZE - 390};
    drawText(level_to_string, GLUT_BITMAP_TIMES_ROMAN_24, position, WHITE);
    free(level_to_string);
}

/**
 * @brief Função que desenha a interface
 * que engloba o tetris
 * 
 * @attention Essa função ainda está em
 * construção
 */
void drawInterface(){
    drawRectangle((BOARD_WIDTH + 2) * BLOCK_SIZE, 0, BOTTOM_RIGHT_COMPONENT_DIMENSIONS, PURPLE);
    drawRectangle(0, (BOARD_HEIGHT + 2) * BLOCK_SIZE, TOP_LEFT_COMPONENT_DIMENSIONS, PURPLE);
    drawRectangle((BOARD_WIDTH + 2) * BLOCK_SIZE, (BOARD_HEIGHT + 2) * BLOCK_SIZE, 
                    TOP_RIGHT_COMPONENT_DIMENSIONS, PURPLE);

    drawScoreBox();
    drawNextBox();
    drawLineBox();
    drawLevelBox();

    drawLogo();

    drawScoreCounterOnBox();
    drawLineCounterOnBox();
    drawLevelCounterOnBox();
}

void doTetrominoRotation(){
    switch (currentTetromino){
        case TETRO_I:   rotateMatrix(MTETRO_I);   break;
        case TETRO_SQR: rotateMatrix(MTETRO_SQR); break;
        case TETRO_LL:  rotateMatrix(MTETRO_LL);  break;
        case TETRO_LR:  rotateMatrix(MTETRO_LR);  break;
        case TETRO_SL:  rotateMatrix(MTETRO_SL);  break;
        case TETRO_SR:  rotateMatrix(MTETRO_SR);  break;
        case TETRO_T:   rotateMatrix(MTETRO_T);   break;
        default: exit(0); break;
    }
}

/**
 * @brief Posicao atual do tetromino
 * 
 */
int pot_current_tetro_line = 1; //Nao pode iniciar em zero pq senao SEGFAULTS
int pot_current_tetro_col  = 3;
int current_tetro_line = 1;
int current_tetro_col = 3;

void drawTetromino(int x, int y, const int tetro[MTETRO_SIZE][MTETRO_SIZE]){
    const GLfloat *cores[] = {LIGHT_BLUE, YELLOW, ORANGE, BLUE, GREEN, RED, PURPLE};
    for(int i = 0; i < MTETRO_SIZE; i++){
        for(int j = 0; j < MTETRO_SIZE; j++){
            if(tetro[i][j] != 0) drawBlock(x + (j * BLOCK_SIZE), y - (i * BLOCK_SIZE), cores[tetro[i][j]-1]);
        }
    }
}

/**
 * @brief Desenha o proximo tetromino
 * no box NEXT
 * 
 * @attention Esta função ainda esta
 * em construção e muito provavelmente
 * será modificada, pois a ideia é passar
 * um tetromino e ela desenhar, mas não
 * ela propria escolher e desenhar. Ou
 * seja, foi feita para fins de teste.
 * 
 */
void drawNextTetromino(){
    switch (nextTetromino){
        case TETRO_I:
            drawTetromino((BOARD_WIDTH + 2) * BLOCK_SIZE + 100, (BOARD_HEIGHT + 2) * BLOCK_SIZE - 90, SC_MTETRO_I);
            break;
        case TETRO_SQR:
            drawTetromino((BOARD_WIDTH + 2) * BLOCK_SIZE + 90, (BOARD_HEIGHT + 2) * BLOCK_SIZE - 85, SC_MTETRO_SQR);
            break;
        case TETRO_LL:
            drawTetromino((BOARD_WIDTH + 2) * BLOCK_SIZE + 70, (BOARD_HEIGHT + 2) * BLOCK_SIZE - 100, SC_MTETRO_LL);
            break;
        case TETRO_LR:
            drawTetromino((BOARD_WIDTH + 2) * BLOCK_SIZE + 105, (BOARD_HEIGHT + 2) * BLOCK_SIZE - 100, SC_MTETRO_LR);
            break;
        case TETRO_SL:
            drawTetromino((BOARD_WIDTH + 2) * BLOCK_SIZE + 80, (BOARD_HEIGHT + 2) * BLOCK_SIZE - 110, SC_MTETRO_SL);
            break;
        case TETRO_SR:
            drawTetromino((BOARD_WIDTH + 2) * BLOCK_SIZE + 100, (BOARD_HEIGHT + 2) * BLOCK_SIZE - 110, SC_MTETRO_SR);
            break;
        case TETRO_T:
            drawTetromino((BOARD_WIDTH + 2) * BLOCK_SIZE + 100, (BOARD_HEIGHT + 2) * BLOCK_SIZE - 110, SC_MTETRO_T);
            break;
        default:
            break;
    }
}

void landTetrominoOnBoard(int tetro[MTETRO_SIZE][MTETRO_SIZE]){
    for(int i = 0; i < MTETRO_SIZE; i++){
        for(int j = 0; j < MTETRO_SIZE; j++){
            if(tetro[i][j] != 0){
                board[i+(current_tetro_line-1)][j+current_tetro_col] = tetro[i][j];
            }
        }
    }
}

void resetPointer(){
    pot_current_tetro_line = 1;
    pot_current_tetro_col  = 3;
}

void updateCurrentPointer(){
    current_tetro_line = pot_current_tetro_line;
    current_tetro_col  = pot_current_tetro_col;
}

void updatePotentialPointer(){
    pot_current_tetro_line = current_tetro_line;
    pot_current_tetro_col  = current_tetro_col;
}

void drawBlocksOnBoard(){
    const GLfloat *cores[] = {LIGHT_BLUE, YELLOW, ORANGE, BLUE, GREEN, RED, PURPLE};
    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            if(board[i][j] != 0) drawBlock((j+1) * BLOCK_SIZE, (BOARD_HEIGHT - i) * BLOCK_SIZE, cores[board[i][j]-1]);
        }
    }
}

void drawCurrentTetromino(){
    int x = (((BOARD_WIDTH)/2) * BLOCK_SIZE) - (4 * BLOCK_SIZE);
    int mvpeca_x = current_tetro_col * BLOCK_SIZE;
    int mvpeca_y = current_tetro_line * BLOCK_SIZE;
    //printf("mvpeca_x = %d\n", mvpeca_x);
    //printf("mvpeca_y = %d\n", mvpeca_y);
    //printf("current_line = %d\n", current_tetro_line);
    switch (currentTetromino){
        case TETRO_I:
            drawTetromino(x + mvpeca_x, (BOARD_HEIGHT + 2) * BLOCK_SIZE - BLOCK_SIZE - mvpeca_y, MTETRO_I);
            break;
        case TETRO_SQR:
            drawTetromino(x  + mvpeca_x, (BOARD_HEIGHT + 2) * BLOCK_SIZE - mvpeca_y, MTETRO_SQR);
            break;
        case TETRO_LL:
            drawTetromino(x  + mvpeca_x, (BOARD_HEIGHT + 2) * BLOCK_SIZE - BLOCK_SIZE - mvpeca_y, MTETRO_LL);
            break;
        case TETRO_LR:
            drawTetromino(x  + mvpeca_x, (BOARD_HEIGHT + 2) * BLOCK_SIZE - BLOCK_SIZE - mvpeca_y, MTETRO_LR);
            break;
        case TETRO_SL:
            drawTetromino(x  + mvpeca_x, (BOARD_HEIGHT + 2) * BLOCK_SIZE - mvpeca_y, MTETRO_SL);
            break;
        case TETRO_SR:
            drawTetromino(x  + mvpeca_x, (BOARD_HEIGHT + 2) * BLOCK_SIZE - mvpeca_y, MTETRO_SR);
            break;
        case TETRO_T:
            drawTetromino(x  + mvpeca_x, (BOARD_HEIGHT + 2) * BLOCK_SIZE - mvpeca_y, MTETRO_T);
            break;
        default:
            break;
    }
}

void updateTetroWithSafeCopy_(int tetro[MTETRO_SIZE][MTETRO_SIZE], const int t_sc[MTETRO_SIZE][MTETRO_SIZE]){
    for(int i = 0; i < MTETRO_SIZE; i++){
        for(int j = 0; j < MTETRO_SIZE; j++){
            tetro[i][j] = t_sc[i][j];
        }
    }
}

void updateTetroWithSafeCopy(int tetro){
    switch (tetro){
        case TETRO_I:   updateTetroWithSafeCopy_(MTETRO_I, SC_MTETRO_I);     break;
        case TETRO_SQR: updateTetroWithSafeCopy_(MTETRO_SQR, SC_MTETRO_SQR); break;
        case TETRO_LL:  updateTetroWithSafeCopy_(MTETRO_LL, SC_MTETRO_LL);   break;
        case TETRO_LR:  updateTetroWithSafeCopy_(MTETRO_LR, SC_MTETRO_LR);   break;
        case TETRO_SL:  updateTetroWithSafeCopy_(MTETRO_SL, SC_MTETRO_SL);   break;
        case TETRO_SR:  updateTetroWithSafeCopy_(MTETRO_SR, SC_MTETRO_SR);   break;
        case TETRO_T:   updateTetroWithSafeCopy_(MTETRO_T, SC_MTETRO_T);     break;
        default: exit(0); break;
    }
}

void drawGameOverBox(){
    GLuint position[] = {(BOARD_WIDTH - 5) * BLOCK_SIZE + 30, (BOARD_HEIGHT - 5) * BLOCK_SIZE - 90};
    drawRectangle((BOARD_WIDTH - 5) * BLOCK_SIZE, (BOARD_HEIGHT - 5) * BLOCK_SIZE - 150, GAMEOVER_BOX_DIMENSIONS, RED);
    drawText("GAME OVER!", GLUT_BITMAP_TIMES_ROMAN_24, position, WHITE);
}

void verifyGameOver(){
    int countBlock = 0;
    for(int row = 0; row < BOARD_HEIGHT; row++){
        for(int col = 0; col < BOARD_WIDTH; col++){
            if( board[row][col] != 0 ){
                countBlock++;
                break;
            }
        }
    }
    if(countBlock == BOARD_HEIGHT){
        drawBlocksOnBoard();
        drawGameOverBox();
        glFlush();
        sleep(3);
        glutDestroyWindow(glutGetWindow());
    }
}

void updateScore(int linesBroke){
    int base_score = 0;
    switch (linesBroke){
        case 0:  base_score = 0;    break;
        case 1:  base_score = 40;   break;
        case 2:  base_score = 100;  break;
        case 3:  base_score = 300;  break;
        default: base_score = 1200; break;
    }
    score_counter += base_score * (level_counter + 1) * (line_counter + 1);
}

void updateLineCounter(int linesBroke){
    int base_line = 0;
    switch (linesBroke){
        case 0:  base_line = 0; break;
        case 1:  base_line = 1; break;
        case 2:  base_line = 2; break;
        default: base_line = 3; break;
    }
    if(base_line == 0) line_counter = 0;
    else if(base_line + line_counter >= 10) line_counter = 10;
    else line_counter += base_line;
}

void updateLevelCounter(){
    if(line_counter >= 5){
        level_counter++;
    }
}

void lineBreak(){
    int linesBroke = 0;
    int col,row;
    for(row = 0; row < BOARD_HEIGHT; row++){
        for(col = 0; col < BOARD_WIDTH; col++){
            if( board[row][col] == 0 ){
                break;
            }
        }
        if(col==BOARD_WIDTH){//percorreu a linha toda com algum elemento diferente de 0
            for(int i=row;i>0;i--){
                for(int j=0;j< BOARD_WIDTH;j++){
                    if(i==0){
                        board[i][j] = 0;
                    }
                    board[i][j] = board[i-1][j];
                }
            }
            linesBroke++;
        }
    }
    updateLineCounter(linesBroke);
    updateLevelCounter();
    updateScore(linesBroke);
}

void firstCheck(int tetro[MTETRO_SIZE][MTETRO_SIZE]){
    for(int row = 0; row < MTETRO_SIZE; row++){
        for(int col = 0; col < MTETRO_SIZE; col++){
            if(tetro[row][col] != 0){
                if(row + (pot_current_tetro_line-1) >= BOARD_HEIGHT){
                    //LAND PIECE
                    landTetrominoOnBoard(tetro);
                    resetPointer();

                    lineBreak();

                    updateTetroWithSafeCopy(currentTetromino);
                    currentTetromino = nextTetromino;
                    nextTetromino = chooseRandomTetro();
                } else if(board[row+pot_current_tetro_line-1][col+pot_current_tetro_col] != 0){
                    //LAND PIECE
                    landTetrominoOnBoard(tetro);
                    resetPointer();

                    lineBreak();

                    updateTetroWithSafeCopy(currentTetromino);
                    currentTetromino = nextTetromino;
                    nextTetromino = chooseRandomTetro();
                }
            }
        }
    }
    updateCurrentPointer();
    drawCurrentTetromino();
    //imprimeTabuleiroAtual();
    verifyGameOver();
}

void secondCheck(int tetro[MTETRO_SIZE][MTETRO_SIZE]){
    for(int row = 0; row < MTETRO_SIZE; row++){
        for(int col = 0; col < MTETRO_SIZE; col++){
            if(tetro[row][col] != 0){
                if(col + pot_current_tetro_col < 0){
                    updatePotentialPointer();
                    drawCurrentTetromino();
                    return;
                }
                if(col + pot_current_tetro_col >= BOARD_WIDTH){
                    updatePotentialPointer();
                    drawCurrentTetromino();
                    return;
                }
                if(board[row+pot_current_tetro_line-1][col+pot_current_tetro_col] != 0){
                    /*landTetrominoOnBoard(tetro);
                    resetPointer();*/
                    updatePotentialPointer();
                    drawCurrentTetromino();
                    return;
                }
            }
        }
    }
    updateCurrentPointer();
    drawCurrentTetromino();
    //imprimeTabuleiroAtual();
}

void thirdCheck(int tetro[MTETRO_SIZE][MTETRO_SIZE]){
    //CHECKS FOR THE BUTTON ROTATION
    int potential_tetro[MTETRO_SIZE][MTETRO_SIZE];

    //--guarda estado da matriz do tetromino atual (aux)
    for(int i = 0; i < MTETRO_SIZE; i++){
        for(int j = 0; j < MTETRO_SIZE; j++){
            potential_tetro[i][j] = tetro[i][j];
        }
    }

    //faz a rotação nessa matriz criada (aux)
    rotateMatrix(potential_tetro);

    //e compara com o board
    for(int row = 0; row < MTETRO_SIZE; row++){
        for(int col = 0; col < MTETRO_SIZE; col++){
            if(potential_tetro[row][col] != 0){
                if(col + current_tetro_col < 0){
                    updatePotentialPointer();
                    drawCurrentTetromino();
                    return;
                }
                if(col + current_tetro_col + 1 >= BOARD_WIDTH){
                    updatePotentialPointer();
                    drawCurrentTetromino();
                    return;
                }
                if(row + (current_tetro_line-1) + 1 >= BOARD_HEIGHT){
                    /*landTetrominoOnBoard(tetro);
                    resetPointer();*/
                    return;
                }
                if(board[row+current_tetro_line-1][col+current_tetro_col] != 0){
                    /*landTetrominoOnBoard(tetro);
                    resetPointer();*/
                    updatePotentialPointer();
                    drawCurrentTetromino();
                    return;
                }
            }
        }
    }

    rotateMatrix(tetro);
    updateCurrentPointer();
    drawCurrentTetromino();
    //imprimeTabuleiroAtual();

    //se ok, rotaciona a matriz original
    //se nao, so mantem o estado original da matriz
}

void runsFirstCheck(int tetro){
    switch (currentTetromino){
        case TETRO_I:   firstCheck(MTETRO_I);   break;
        case TETRO_SQR: firstCheck(MTETRO_SQR); break;
        case TETRO_LL:  firstCheck(MTETRO_LL);  break;
        case TETRO_LR:  firstCheck(MTETRO_LR);  break;
        case TETRO_SL:  firstCheck(MTETRO_SL);  break;
        case TETRO_SR:  firstCheck(MTETRO_SR);  break;
        case TETRO_T:   firstCheck(MTETRO_T);   break;
        default: exit(0); break;
    }
    //pot_current_tetro_line++;
    //glutTimerFunc(1000, runsFirstCheck, 0);
}

void runsSecondCheck(int tetro){
    switch (currentTetromino){
        case TETRO_I:   secondCheck(MTETRO_I);   break;
        case TETRO_SQR: secondCheck(MTETRO_SQR); break;
        case TETRO_LL:  secondCheck(MTETRO_LL);  break;
        case TETRO_LR:  secondCheck(MTETRO_LR);  break;
        case TETRO_SL:  secondCheck(MTETRO_SL);  break;
        case TETRO_SR:  secondCheck(MTETRO_SR);  break;
        case TETRO_T:   secondCheck(MTETRO_T);   break;
        default: exit(0); break;
    }
}

void runsThirdCheck(int tetro){
    switch (currentTetromino){
        case TETRO_I:   thirdCheck(MTETRO_I);   break;
        case TETRO_SQR: thirdCheck(MTETRO_SQR); break;
        case TETRO_LL:  thirdCheck(MTETRO_LL);  break;
        case TETRO_LR:  thirdCheck(MTETRO_LR);  break;
        case TETRO_SL:  thirdCheck(MTETRO_SL);  break;
        case TETRO_SR:  thirdCheck(MTETRO_SR);  break;
        case TETRO_T:   thirdCheck(MTETRO_T);   break;
        default: exit(0); break;
    }
}

int isKeyPressed = NOT_PRESSED; //TRUE - 1, FALSE - 2;

void SpecialKeys(int key, int x, int y){  

    if(key == GLUT_KEY_UP){
        runsThirdCheck(currentTetromino);
    }
    
    if(key == GLUT_KEY_DOWN){
        pot_current_tetro_line++;
        runsFirstCheck(currentTetromino);
    }

    if(key == GLUT_KEY_LEFT){
        pot_current_tetro_col--;
        runsSecondCheck(currentTetromino);
    } 
  
    if(key == GLUT_KEY_RIGHT){
        pot_current_tetro_col++;
        runsSecondCheck(currentTetromino);
    }

    isKeyPressed = PRESSED;

    // Refresh the Window
    glutPostRedisplay();  
}

void initGame(){
    

    drawNextTetromino();
    runsFirstCheck(currentTetromino);
    drawBlocksOnBoard();

}

// Function to display the OpenGL scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Call the function to draw the tetromino
    drawBoard();
    drawInterface();
    drawLogo();

    initGame();

    if(isKeyPressed == NOT_PRESSED){
        pot_current_tetro_line++;
        glutTimerFunc(250, display, 0);
    } else {
        /*pot_current_tetro_line = current_tetro_line;*/
        updatePotentialPointer();
    }

    isKeyPressed = NOT_PRESSED;

    glFlush();
}

void loadImageTexture()
{
    unsigned char* image = stbi_load("logo_tetris.png", &imageWidth, &imageHeight, &imageChannels, STBI_rgb_alpha);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    stbi_image_free(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

// Function to handle window resizing
void reshape(int width, int height) {
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-WINDOW_WIDTH)/2, 
                            (glutGet(GLUT_SCREEN_HEIGHT)-WINDOW_HEIGHT)/2);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Tetris");

    // Set the background color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    currentTetromino = chooseRandomTetro();
    nextTetromino = chooseRandomTetro();

    loadImageTexture();

    // Register callback functions
    glutReshapeFunc(reshape);
    glutSpecialFunc(SpecialKeys);
    glutDisplayFunc(display);

    glutMainLoop();

    glDeleteTextures(1, &texture);

    return 0;
}
