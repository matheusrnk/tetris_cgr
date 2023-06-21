// gcc tetris_v2.c -lglut -lGL -lGLU -lm -o tetris_v2 && ./tetris_v2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>

GLfloat xRot = 0.0f;
GLfloat yRot = 0.0f;

int mvpeca_y = 0;
int mvpeca_x = 0;

/**
 * @brief Tamanho do tabuleiro
 * 
*/
#define BOARD_WIDTH  10
#define BOARD_HEIGHT 20

enum BLOCK_TYPE {
    B_LIGHTBLUE = 1,
    B_YELLOW    = 2,
    B_ORANGE    = 3,
    B_BLUE      = 4,
    B_GREEN     = 5,
    B_RED       = 6,
    B_PURPLE    = 7
};

/**
 * @brief Estado do jogo e afins
 * 
 */

unsigned long score_counter = 0; //MAX 10
int line_counter  = 0;
int level_counter = 0;

int board[BOARD_WIDTH][BOARD_HEIGHT];

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
    {0, 0, 0, 3},
    {0, 0, 0, 3},
    {0, 0, 3, 3},
    {0, 0, 0, 0}
};

int MTETRO_LR[MTETRO_SIZE][MTETRO_SIZE] = {
    {4, 0, 0, 0},
    {4, 0, 0, 0},
    {4, 4, 0, 0},
    {0, 0, 0, 0}
};

int MTETRO_SL[MTETRO_SIZE][MTETRO_SIZE] = {
    {0, 5, 5, 0},
    {0, 0, 5, 5},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

int MTETRO_SR[MTETRO_SIZE][MTETRO_SIZE] = {
    {0, 6, 6, 0},
    {6, 6, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

int MTETRO_T[MTETRO_SIZE][MTETRO_SIZE] = {
    {7, 7, 7, 0},
    {0, 7, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

int *M_TETROMINOS[] = {MTETRO_I, MTETRO_SQR, MTETRO_LL, 
                        MTETRO_LR, MTETRO_SL, MTETRO_SR, 
                        MTETRO_T};

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
#define BOTTOM_RIGHT_COMPONENT_DIMENSIONS d0
#define TOP_LEFT_COMPONENT_DIMENSIONS d1
#define TOP_RIGHT_COMPONENT_DIMENSIONS d2
#define SCORE_BOX_DIMENSIONS d3
#define NEXT_BOX_DIMENSIONS d4
#define LINE_BOX_DIMENSIONS d5
#define LEVEL_BOX_DIMENSIONS d5

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

/**
 * @brief Escolhe um tetromino aleatorio
 * 
 * @return int valor do tetromino
 */
static int chooseRandomTetro(){
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
 * @brief Função que desenha um tetromino
 * no formato I com a cor azul-suave:
 *       _
 *      |_|
 *      |_|
 *      |_|
 *      |_|
 * 
 * @param x Posição horizontal
 * @param y Posição vertical
 */
void drawTetrominoI(int x, int y) {
    drawBlock(x, y, LIGHT_BLUE);
    drawBlock(x, y + BLOCK_SIZE, LIGHT_BLUE);
    drawBlock(x, y + (2 * BLOCK_SIZE), LIGHT_BLUE);
    drawBlock(x, y + (3 * BLOCK_SIZE), LIGHT_BLUE);
}

/**
 * @brief Função que desenha um tetromino
 * no formato [] com a cor amarela:
 *      _ _
 *     |_|_|
 *     |_|_|
 * 
 * @param x Posição horizontal
 * @param y Posição vertical
 */
void drawTetrominoSqr(int x, int y){
    drawBlock(x, y, YELLOW);
    drawBlock(x + BLOCK_SIZE, y, YELLOW);
    drawBlock(x, y + BLOCK_SIZE, YELLOW);
    drawBlock(x + BLOCK_SIZE, y + BLOCK_SIZE, YELLOW);
}

/**
 * @brief Função que desenha um tetromino
 * no formato L virado à esquerda (Left-L)
 * com a cor azul:
 *      _
 *     |_|
 *    _|_|
 *   |_|_|
 * 
 * @param x Posição horizontal
 * @param y Posição vertical
 */
void drawTetrominoLL(int x, int y){
    drawBlock(x, y, BLUE);
    drawBlock(x + BLOCK_SIZE, y, BLUE);
    drawBlock(x + BLOCK_SIZE, y + BLOCK_SIZE, BLUE);
    drawBlock(x + BLOCK_SIZE, y + (2 * BLOCK_SIZE), BLUE);
}

/**
 * @brief Função que desenha um tetromino
 * no formato L virado à direita (Right-R)
 * com a cor laranja:
 *      _
 *     |_|
 *     |_|_
 *     |_|_|
 * 
 * @param x Posição horizontal
 * @param y Posição vertical
 */
void drawTetrominoLR(int x, int y){
    drawBlock(x, y, ORANGE);
    drawBlock(x, y + BLOCK_SIZE, ORANGE);
    drawBlock(x, y + (2 * BLOCK_SIZE), ORANGE);
    drawBlock(x + BLOCK_SIZE, y, ORANGE);
}

/**
 * @brief Função que desenha um tetromino
 * no formato S virado à esquerda (Left-L)
 * com a cor vermelha:
 *    _ _
 *   |_|_|_
 *     |_|_|
 * 
 * @param x Posição horizontal
 * @param y Posição vertical
 */
void drawTetrominoSL(int x, int y){
    drawBlock(x + BLOCK_SIZE, y, RED);
    drawBlock(x + (2 * BLOCK_SIZE), y, RED);
    drawBlock(x, y + BLOCK_SIZE, RED);
    drawBlock(x + BLOCK_SIZE, y + BLOCK_SIZE, RED);
}

/**
 * @brief Função que desenha um tetromino
 * no formato S virado à direita (Right-R)
 * com a cor verde:
 *      _ _
 *    _|_|_|
 *   |_|_|
 * 
 * @param x Posição horizontal
 * @param y Posição vertical
 */
void drawTetrominoSR(int x, int y){
    drawBlock(x, y, GREEN);
    drawBlock(x + BLOCK_SIZE, y, GREEN);
    drawBlock(x + BLOCK_SIZE, y + BLOCK_SIZE, GREEN);
    drawBlock(x + (2 * BLOCK_SIZE), y + BLOCK_SIZE, GREEN);
}

/**
 * @brief Função que desenha um tetromino
 * no formato T com a cor roxa:
 *    _ _ _
 *   |_|_|_|
 *     |_|
 * 
 * @param x Posição horizontal
 * @param y Posição vertical
 */
void drawTetrominoT(int x, int y){
    drawBlock(x + BLOCK_SIZE, y, PURPLE);
    drawBlock(x + BLOCK_SIZE, y + BLOCK_SIZE, PURPLE);
    drawBlock(x + (2 * BLOCK_SIZE), y + BLOCK_SIZE, PURPLE);
    drawBlock(x, y + BLOCK_SIZE, PURPLE);
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
void drawLogo(){
    GLuint position[] = {(BOARD_WIDTH + 2) * BLOCK_SIZE + 90, (BOARD_HEIGHT + 2) * BLOCK_SIZE + 100};
    drawText("LOGO", GLUT_BITMAP_TIMES_ROMAN_24, position, WHITE);
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
    rotateMatrix(M_TETROMINOS[currentTetromino-1]);
}

void SpecialKeys(int key, int x, int y){  

    if(key == GLUT_KEY_UP)
        doTetrominoRotation();
    
    if(key == GLUT_KEY_DOWN)
        mvpeca_y -= BLOCK_SIZE;

    if(key == GLUT_KEY_LEFT)  
        mvpeca_x -= BLOCK_SIZE;  
  
    if(key == GLUT_KEY_RIGHT)  
        mvpeca_x += BLOCK_SIZE;
                  
    xRot = (GLfloat)((const int)xRot % 360);
    yRot = (GLfloat)((const int)yRot % 360);
  
    // Refresh the Window  
    glutPostRedisplay();  
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
            drawTetrominoI((BOARD_WIDTH + 2) * BLOCK_SIZE + 115, (BOARD_HEIGHT + 2) * BLOCK_SIZE - 155);
            break;
        case TETRO_SQR:
            drawTetrominoSqr((BOARD_WIDTH + 2) * BLOCK_SIZE + 110, (BOARD_HEIGHT + 2) * BLOCK_SIZE - 135);
            break;
        case TETRO_LL:
            drawTetrominoLL((BOARD_WIDTH + 2) * BLOCK_SIZE + 110, (BOARD_HEIGHT + 2) * BLOCK_SIZE - 140);
            break;
        case TETRO_LR:
            drawTetrominoLR((BOARD_WIDTH + 2) * BLOCK_SIZE + 110, (BOARD_HEIGHT + 2) * BLOCK_SIZE - 140);
            break;
        case TETRO_SL:
            drawTetrominoSL((BOARD_WIDTH + 2) * BLOCK_SIZE + 100, (BOARD_HEIGHT + 2) * BLOCK_SIZE - 135);
            break;
        case TETRO_SR:
            drawTetrominoSR((BOARD_WIDTH + 2) * BLOCK_SIZE + 100, (BOARD_HEIGHT + 2) * BLOCK_SIZE - 135);
            break;
        case TETRO_T:
            drawTetrominoT((BOARD_WIDTH + 2) * BLOCK_SIZE + 100, (BOARD_HEIGHT + 2) * BLOCK_SIZE - 135);
            break;
        default:
            break;
    }
}

int verifiesCollison(int x, int y, int peca){
    //é possivel inserir esta peça aqui?
    //ou seja, pegamos a posições que a peça irá
    //assumir e verificamos se já há um valor naquela posição
    //Se sim, NÃO-COLISÃO NAQUELA PARTE e continua
    //Se não, COLISÃO
    //Se for sim para todos, NÃO-COLISAO
}

void drawBlocksOnBoard(){
    const GLfloat *cores[] = {LIGHT_BLUE, YELLOW, ORANGE, BLUE, GREEN, RED, PURPLE};
    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            if(board[i][j] != 0) drawBlock((j+1) * BLOCK_SIZE, (BOARD_HEIGHT - i) * BLOCK_SIZE, cores[board[i][j]-1]);
        }
    }
}

void drawTetromino(int x, int y, const int tetro[MTETRO_SIZE][MTETRO_SIZE]){
    const GLfloat *cores[] = {LIGHT_BLUE, YELLOW, ORANGE, BLUE, GREEN, RED, PURPLE};
    for(int i = 0; i < MTETRO_SIZE; i++){
        for(int j = 0; j < MTETRO_SIZE; j++){
            if(tetro[i][j] != 0) drawBlock(x + (j * BLOCK_SIZE), y - (i * BLOCK_SIZE), cores[tetro[i][j]-1]);
        }
    }
}

int verifiesTetrominoSize(int tetro[MTETRO_SIZE][MTETRO_SIZE]){
    int size = 0;
    for(int i = 0; i < MTETRO_SIZE; i++){
        for(int j = 0; j < MTETRO_SIZE; j++){
            if(tetro[i][j] != 0){ size++; continue; }
        }
    }
    return size;
}

void drawCurrentTetromino(){
    int x = (BOARD_WIDTH/2) * BLOCK_SIZE;
    int pos_x = x + mvpeca_x;
    int pos_y;
    switch (currentTetromino){
        case TETRO_I:
            pos_y = BOARD_HEIGHT * BLOCK_SIZE + mvpeca_y;
            printf("pos_x = %d\n", pos_x);
            printf("pos_y = %d\n", pos_y);

            printf("size = %d\n", verifiesTetrominoSize(MTETRO_I));

            if(pos_y >= verifiesTetrominoSize(MTETRO_I) * BLOCK_SIZE){
                if(pos_x <= -20){
                    pos_x = 0;
                    mvpeca_x += BLOCK_SIZE;
                } else if(pos_x >= BOARD_WIDTH * BLOCK_SIZE){
                    pos_x = (BOARD_WIDTH-1) * BLOCK_SIZE;
                    mvpeca_x -= BLOCK_SIZE;
                }
                //drawTetrominoI(pos_x, pos_y);
                drawTetromino(pos_x, pos_y, MTETRO_I);
            } else {
                for(int i = (pos_y/BLOCK_SIZE) + (BOARD_HEIGHT-1); i > (pos_y/BLOCK_SIZE) + (BOARD_HEIGHT-1) - 4; i--){
                    board[i][(pos_x-BLOCK_SIZE)/BLOCK_SIZE] = B_LIGHTBLUE;
                }
                //preciso resetar a matriz do objeto para o seu estado inicial tbm
                drawBlocksOnBoard();
                currentTetromino = INIT_STATE;
                mvpeca_x = 0;
                mvpeca_y = BLOCK_SIZE;
            }
            break;
        case TETRO_SQR:
            pos_y = (BOARD_HEIGHT + 2) * BLOCK_SIZE - (BLOCK_SIZE * 3) + mvpeca_y;

            if(pos_y >= BLOCK_SIZE){
                if(pos_x <= 0){
                    pos_x = BLOCK_SIZE;
                    mvpeca_x += BLOCK_SIZE;
                } else if(pos_x >= BOARD_WIDTH * BLOCK_SIZE){
                    pos_x = (BOARD_WIDTH-1) * BLOCK_SIZE;
                    mvpeca_x -= BLOCK_SIZE;
                }
                drawTetrominoSqr(pos_x, pos_y);
            } else {
                for(int i = (pos_y/BLOCK_SIZE) + (BOARD_HEIGHT-1); i > (pos_y/BLOCK_SIZE) + (BOARD_HEIGHT-1) - 2; i--){
                    for(int j = (pos_x-BLOCK_SIZE)/BLOCK_SIZE; j < ((pos_x-BLOCK_SIZE)/BLOCK_SIZE) + 2; j++){
                        board[i][j] = B_YELLOW;
                    }
                }
                drawBlocksOnBoard();
                currentTetromino = INIT_STATE;
                mvpeca_x = 0;
                mvpeca_y = BLOCK_SIZE;
            }
            break;
        case TETRO_LL:
            pos_y = (BOARD_HEIGHT + 2) * BLOCK_SIZE - (BLOCK_SIZE * 4) + mvpeca_y;
            drawTetrominoLL(pos_x, pos_y);
            break;
        case TETRO_LR:
            pos_y = (BOARD_HEIGHT + 2) * BLOCK_SIZE - (BLOCK_SIZE * 4) + mvpeca_y;
            drawTetrominoLR(pos_x, pos_y);
            break;
        case TETRO_SL:
            pos_y = (BOARD_HEIGHT + 2) * BLOCK_SIZE - (BLOCK_SIZE * 3) + mvpeca_y;
            drawTetrominoSL(pos_x, pos_y);
            break;
        case TETRO_SR:
            pos_y = (BOARD_HEIGHT + 2) * BLOCK_SIZE - (BLOCK_SIZE * 3) + mvpeca_y;
            drawTetrominoSR(pos_x, pos_y);
            break;
        case TETRO_T:
            pos_y = (BOARD_HEIGHT + 2) * BLOCK_SIZE - (BLOCK_SIZE * 3) + mvpeca_y;
            drawTetrominoT(pos_x, pos_y);
            break;
        default:
            break;
    }
}

void initGame(){

    currentTetromino = /*chooseRandomTetro()*/1;
    nextTetromino    = chooseRandomTetro();
    

    drawNextTetromino();
    imprimeTabuleiroAtual();
    drawBlocksOnBoard();
    drawCurrentTetromino();


}

// Function to display the OpenGL scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Call the function to draw the tetromino
    drawBoard();
    drawInterface();

    initGame();

    glFlush();
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

    // Register callback functions
    glutReshapeFunc(reshape);
    glutSpecialFunc(SpecialKeys);
    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}
