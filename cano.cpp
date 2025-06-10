#include <GL/glut.h>

// Variáveis para os parâmetros do cano
GLdouble cano_raio = 0.5;
GLdouble cano_comprimento = 3.0;
GLint cano_fatias = 50; // Qualidade da circularidade
GLint cano_pilhas = 10;  // Qualidade ao longo do comprimento

// Variáveis para controlar a câmera
float rotX = 20.0;
float rotY = -30.0;
int lastMouseX, lastMouseY;

void display() {
    // Limpa o buffer de cor e de profundidade
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reseta as transformações
    glLoadIdentity();

    // Define a posição da câmera (olho, centro, vetor up)
    gluLookAt(0.0, 0.0, 7.0,   // Posição do olho
              0.0, 0.0, 0.0,   // Ponto para onde olha
              0.0, 1.0, 0.0);  // Vetor "para cima"

    // Aplica rotações da câmera baseadas no movimento do mouse
    glRotatef(rotX, 1.0, 0.0, 0.0);
    glRotatef(rotY, 0.0, 1.0, 0.0);

    // ---- Desenho do Cano ----
    glColor3f(0.7f, 0.7f, 0.8f); // Define a cor do cano (cinza claro)

    // Cria um novo objeto quádrico
    GLUquadric* quad = gluNewQuadric();

    // Define o estilo do desenho (preenchido)
    gluQuadricDrawStyle(quad, GLU_FILL);

    // Desenha o cilindro (cano)
    // Os dois raios são iguais para um cano reto
    gluCylinder(quad, cano_raio, cano_raio, cano_comprimento, cano_fatias, cano_pilhas);

    // Libera o objeto quádrico da memória
    gluDeleteQuadric(quad);
    // -------------------------

    // Troca os buffers para exibir o que foi desenhado
    glutSwapBuffers();
}

void reshape(int w, int h) {
    // Evita divisão por zero
    if (h == 0) h = 1;
    float ratio = 1.0 * w / h;

    // Utiliza a matriz de projeção
    glMatrixMode(GL_PROJECTION);

    // Reseta a matriz
    glLoadIdentity();

    // Define o viewport para usar a janela inteira
    glViewport(0, 0, w, h);

    // Define a perspectiva
    gluPerspective(45, ratio, 1, 100);

    // Retorna para a matriz de visualização de modelo
    glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        lastMouseX = x;
        lastMouseY = y;
    }
}

void motion(int x, int y) {
    rotY += (x - lastMouseX);
    rotX += (y - lastMouseY);
    lastMouseX = x;
    lastMouseY = y;
    glutPostRedisplay(); // Redesenha a cena
}


int main(int argc, char** argv) {
    // Inicializa o GLUT
    glutInit(&argc, argv);
    // Define o modo de display (duplo buffer, cor RGBA, profundidade)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    // Define as dimensões da janela
    glutInitWindowSize(800, 600);
    // Cria a janela com um título
    glutCreateWindow("Modelo de Cano Reto com OpenGL");

    // Registra as funções de callback
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    // Habilita o teste de profundidade para renderização 3D correta
    glEnable(GL_DEPTH_TEST);

    // Inicia o loop principal do GLUT
    glutMainLoop();

    return 0;
}
