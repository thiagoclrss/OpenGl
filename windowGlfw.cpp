#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();

    // Configurar versão do OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Criar janela
    GLFWwindow* window = glfwCreateWindow(800, 600, "Minha Janela OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Falha ao criar janela GLFW\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Carregar funções OpenGL com GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Falha ao inicializar GLAD\n";
        return -1;
    }

    // Configurar callback para redimensionamento
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        // Cor de fundo
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // R, G, B, A
        glClear(GL_COLOR_BUFFER_BIT);        // Limpar o framebuffer

        glfwSwapBuffers(window); // Mostrar na tela
        glfwPollEvents();        // Checar entradas do teclado, mouse, etc
    }

    glfwTerminate(); // Finaliza GLFW
    return 0;
}
