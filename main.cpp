// #define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

int main(void) {
  GLFWwindow *window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  /* Init GLEW */
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    /* Problem: glewInit failed, something is seriously wrong. */
    std::cout << "Error !";
  }
  std::cout << "Glew Version: " << glewGetString(GLEW_VERSION) << std::endl;
  std::cout << "GL Version:   " << glGetString(GL_VERSION) << std::endl;

  float positions[6] = {
      -0.5f, -0.5f, 0.0f, 0.5f, 0.5f, -0.5f, // simple triangle
  };

  //
  // Create a buffer and give it data
  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 6, positions, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind Buffer

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDrawElements(mode, GLsizei count, GLenum type, const void *indices)
    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
