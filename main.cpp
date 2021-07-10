// #define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
// #include <string.h>
#include <signal.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define ASSERT(x) \
    if (!(x)) __builtin_trap();
#ifdef _DEBUG
#define GLCall(x)   \
    GLClearError(); \
    x;              \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x
#endif

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR)
        ;
}

static bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL_Error] (" << error << "): " << function << " "
                  << file << ":" << line << std::endl;

        return false;
    };
    return true;
}

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)type] << line << '\n';
        }
    }

    return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(const std::string& source,
                                  unsigned int type) {
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();

    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader!" << std::endl;

        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader,
                                 const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs      = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs      = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));
    int result;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
    if (result == GL_FALSE) {
        std::cout << "PROGRAM ERROR!!!!!" << std::endl;
    }

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
    return program;
}

int initGlfw(GLFWwindow** window) {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    *window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(*window);
    glfwSwapInterval(1);
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

    int TRIANGLES = 2;
    int VERTICES  = 3 * TRIANGLES;  // 6
    int CORDS     = VERTICES * 2;   // 12

    float positions[] = {
        // Simple Quad
        -0.5f, -0.5f,  //  0
        0.5f,  -0.5f,  // 1
        0.5f,  0.5f,   // 2
        -0.5f, 0.5f,   // 3
    };

    unsigned int indeces[] = {
        0, 1, 2,  // Tri 1
        2, 3, 0   // Tri 2
    };

    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    // Vertex Buffer
    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions,
                        GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    // Index Buffer
    unsigned int ibuffer;
    GLCall(glGenBuffers(1, &ibuffer));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces,
                        GL_STATIC_DRAW));

    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

    unsigned int shader =
        CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));

    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);
    GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

    GLCall(glBindVertexArray(0)); // Unbind first so unbindings dont affect it
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glUseProgram(0));

    float r    = 0.0;
    float incr = 0.05f;
    while (!glfwWindowShouldClose(window)) {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        // bind stuff
        GLCall(glUseProgram(shader));
        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));  // set color
        GLCall(glBindVertexArray(vao));

        // DRAW
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r >= 1.0f)
            incr = -0.05f;
        else if (r <= 0.0f) {
            incr = 0.05f;
        }

        r += incr;

        GLCall(glfwSwapBuffers(window));

        GLCall(glfwPollEvents());
    }
    GLCall(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}
