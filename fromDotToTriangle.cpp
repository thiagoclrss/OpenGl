#include <GL/glew.h>
#include <GL/freeglut_std.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <cstdio>
#include "math_3d.h"

GLuint VBO;

static void RenderSceneCB(){
    glClear(GL_COLOR_BUFFER_BIT);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    glutSwapBuffers();
}

static void CreateVertexBuffer(){
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    Vector3f Vertices[3];
    Vertices[0] = Vector3f(-1.0f,-1.0f,0.0f); //bottom left
    Vertices[1] = Vector3f(0.0f,1.0f,0.0f); //top
    Vertices[2] = Vector3f(1.0f,-1.0f,0.0f); //bottom right


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
