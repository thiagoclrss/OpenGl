// glew_freeglut_point.c
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>

void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(10.0f); // Tamanho do ponto
    glBegin(GL_POINTS);
        glColor3f(0.0f, 1.0f, 0.0f); // Cor verde
        glVertex2f(0.0f, 0.0f); // No centro da tela
    glEnd();

    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("GLEW + FreeGLUT Point Example");

    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        return -1;
    }

    glutDisplayFunc(RenderScene);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutMainLoop();

    return 0;
}
