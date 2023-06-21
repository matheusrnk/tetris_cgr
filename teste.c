// gcc -o teste teste.c stb_image.h -lglut -lGL -lGLU -lm && ./teste
#include <GL/glut.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

GLuint texture;
int imageWidth, imageHeight, imageChannels;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(-0.5f, -0.5f);

    glTexCoord2f(1, 0);
    glVertex2f(0.5f, -0.5f);

    glTexCoord2f(1, 1);
    glVertex2f(0.5f, 0.5f);

    glTexCoord2f(0, 1);
    glVertex2f(-0.5f, 0.5f);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
}

void loadImageTexture()
{
    unsigned char* image = stbi_load("tetris-logo.png", &imageWidth, &imageHeight, &imageChannels, STBI_rgb_alpha);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    stbi_image_free(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("PNG Image Rendering");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    loadImageTexture();

    glutMainLoop();

    glDeleteTextures(1, &texture);

    return 0;
}

