// #define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>

#include <iostream>

int initGlfw(GLFWwindow** window) {
    if (!glfwInit()) return -1;

    *window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(*window);
    return 0;
}

void initGlew() {
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "Error !";
    }
    std::cout << "Glew Version: " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "GL Version:   " << glGetString(GL_VERSION) << std::endl;
}

int main(void) {
    GLFWwindow* window;
    initGlfw(&window);
    initGlew();

    float positions[6] = {
        -0.5f, -0.5f, 0.0f, 0.5f, 0.5f, -0.5f,  // simple triangle
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6, positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2,
                          (const char*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // Unbind Buffer

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
