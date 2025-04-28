#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cstdio>
#include <iostream>
#include "math_3d.h"

GLuint VBO;

static void RenderSceneCB()
{
    static GLclampf c = 0.0f;
    glClearColor(c,c,c,c);
    printf("%f\n",c);
    c += 1.0f/256.0f;

    if(c >= 1.0f){
        c = 0.0f;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glutPostRedisplay();
    glutSwapBuffers();
}

static void CreateVertexBuffer(){
    Vector3f Vertices[1];
    Vertices[0] = Vector3f(0.0f,0.0f,0.0f);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}



int main (int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    int width = 1920;
    int height = 1080;
    glutInitWindowSize(width, height);

    int x = 200;
    int y = 100;
    glutInitWindowPosition(x, y);
    int window = glutCreateWindow("Tutorial 02");
    printf("window id: %d\n", window);

   //Must be done after glut initialize
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s' \n", glewGetErrorString(res));
        return 1;
    }

    GLclampf Red = 0.0f, Green = 0.0f, Blue = 0.0f, Alpha = 0.0f;
    glClearColor(Red, Green, Blue, Alpha);

    CreateVertexBuffer();

    glutDisplayFunc(RenderSceneCB);

    glutMainLoop();

    return 0;

}
