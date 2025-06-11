// Requisitos: GLEW + GLFW + GLM
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <cmath>

const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 MVP;
void main() {
    gl_Position = MVP * vec4(aPos, 1.0);
})";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(0.6, 0.8, 1.0, 1.0);
})";

glm::vec3 bezierPoint(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;
    return uuu * p0 + 3 * uu * t * p1 + 3 * u * tt * p2 + ttt * p3;
}

void generateTubeMesh(
    std::vector<glm::vec3>& vertices,
    std::vector<unsigned int>& indices,
    glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,
    int segments = 100,
    int radialSegments = 16,
    float radius = 0.1f)
{
    for (int i = 0; i <= segments; ++i) {
        float t = (float)i / segments;
        glm::vec3 point = bezierPoint(t, p0, p1, p2, p3);

        // Calcula a tangente de forma mais estável para o último ponto
        glm::vec3 tangent;
        if (t > 1.0f - 0.01f) {
            tangent = glm::normalize(point - bezierPoint(t - 0.01f, p0, p1, p2, p3));
        } else {
            tangent = glm::normalize(bezierPoint(t + 0.01f, p0, p1, p2, p3) - point);
        }

        glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
        if (fabs(glm::dot(tangent, normal)) > 0.99f)
            normal = glm::vec3(1.0f, 0.0f, 0.0f);

        glm::vec3 bitangent = glm::normalize(glm::cross(tangent, normal));
        normal = glm::normalize(glm::cross(bitangent, tangent));

        for (int j = 0; j < radialSegments; ++j) {
            float theta = 2.0f * glm::pi<float>() * float(j) / radialSegments;

            // CORREÇÃO: Use cosf() e sinf() para operar com floats.
            glm::vec3 circlePoint = point + radius * (cosf(theta) * normal + sinf(theta) * bitangent);
            vertices.push_back(circlePoint);
        }
    }

    for (int i = 0; i < segments; ++i) {
        for (int j = 0; j < radialSegments; ++j) {
            int nextJ = (j + 1) % radialSegments;

            int topLeft = i * radialSegments + j;
            int topRight = i * radialSegments + nextJ;
            int bottomLeft = (i + 1) * radialSegments + j;
            int bottomRight = (i + 1) * radialSegments + nextJ;

            // Primeiro triângulo
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Segundo triângulo (Lógica de índice mais clara)
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
}

GLuint createShaderProgram() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Cano Curvado Bezier", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewExperimental = true;
    glewInit();

    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    generateTubeMesh(vertices, indices,
        glm::vec3(-1, 0, 0), glm::vec3(-0.5, 1, 0), glm::vec3(0.5, -1, 0), glm::vec3(1, 0, 0));

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    GLuint shaderProgram = createShaderProgram();

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(2,2,2), glm::vec3(0,0,0), glm::vec3(0,1,0));
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 MVP = proj * view * model;

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
