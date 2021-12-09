#include "Constants.h"
#include "Headers.h"
#include "Input.h"
#include "Window.h"

#include <iostream>

using uint = unsigned int;

uint shaderProgram;

void render();
void checkShaderCompiledSuccesfully(uint vertexShader);
void checkProgramLinkedSuccesfully(uint shaderProgram);

const char* vertexShaderSource = "#version 460 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 460 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(Window::WIDTH, Window::HEIGHT, "Learning OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create OpenGL Window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, Window::WIDTH, Window::HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // VERTEX SHADER START
    uint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // <- assigns just an ID

    // compilation
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // we pass shader source to the shader (with id)
    glCompileShader(vertexShader);
    checkShaderCompiledSuccesfully(vertexShader);
    // VERTEX SHADER END

    // FRAGMENT SHADER START
    uint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // compilation
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompiledSuccesfully(fragmentShader);
    // FRAGMENT SHADER END

    // SHADER PROGRAM START
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // SHADER PROGRAM END

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    uint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // STREAM_DRAW -> set only once and used at most a few times
    // STATIC_DRAW -> set only once and used many times
    // DYNAMIC_DRAW -> changed a lot and used many times
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    uint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // how to interpret data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void render()  {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void checkShaderCompiledSuccesfully(uint vertexShader) {
    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void checkProgramLinkedSuccesfully(uint shaderProgram) {
    int success;
    char infoLog[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }
}