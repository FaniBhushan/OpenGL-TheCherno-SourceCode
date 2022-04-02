#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define ASSERT(x) \
    if (!x)       \
    abort()
#define GLCALL(x)   \
    GLClearError(); \
    x;              \
    ASSERT(GLPrintError(#x, __FILE__, __LINE__))

static void GLClearError()
{
    while (GL_NO_ERROR != glGetError())
        ;
}

static bool GLPrintError(const char *function, const char *fileName, int line)
{
    auto errorCode = glGetError();
    if (GL_NO_ERROR != errorCode)
    {
        std::cout << errorCode << " Error in function: " << function << " in file: " << fileName << "in line: " << line << std::endl;
        return false;
    }
    return true;
}

struct ShaderSource
{
    std::string fragmentShader;
    std::string vertexShader;
};

static ShaderSource ParseShader(const std::string filePath)
{
    std::ifstream file(filePath);

    enum class ShaderType
    {
        NONE = -1,
        FRAGMENT = 0,
        VERTEX = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type;

    while (getline(file, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
            else
            {
                type = ShaderType::VERTEX;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string &source)
{
    // create shader object  and returns the id of the same
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();

    // it replaces the source code in the shader object
    GLCALL(glShaderSource(id, 1, &src, nullptr));

    // compile the shader cod)
    GLCALL(glCompileShader(id));

    // Todo: error handling like syntax error in the shader source code.
    // glgetshaderiv, alloca

    int result;

    // get the compilation status of the shader. If false, enter error handling
    GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;

        // Get the length of the error message
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        // using alloca, create a buffer for storing the error message by dynamically allocatinf memory on stack.
        char *buff = (char *)alloca(length * sizeof(char));

        // Get the error message in the buff and print.
        glGetShaderInfoLog(id, length, &length, buff);

        std::cout << "failed to compile the" << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
                  << " shader" << std::endl;
        std::cout << buff << std::endl;

        // since there was an error in compiling the shader, we can delete the shader
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    // create a program object to which a shader can be attached later.
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // attach the shaders to the program object.
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    /*This will link the program object. Since there are fragment and vertex shaders attached to the program object.
    This will create executables which can run on vertex and fragment processors*/
    glLinkProgram(program);
    glValidateProgram(program);

    // since the shaders are already attached and linked and executables are created, they can be deleted from the memory.
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // use specific version of Opengl lib i.e. 3.3 here
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // setting the core profile instead of compatibility profile. This will result in no default vertex array object creation as 0.
    // Therefore a vertex array object needs to be defined i.e VAO below
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    std::cout << "I'm apple machine" << std::endl;
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    unsigned int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    unsigned int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    std::cout << "oepngl shader version: " << major << "." << minor << std::endl;

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // set the interval for delaying the swapping of the buffer when glfwSwapBmjuffer is called
    glfwSwapInterval(1);

    if (GLEW_OK == glewInit())
        std::cout << "OK" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    // buffer data store
    float positions[] = {
        -0.5f, -0.5f, // 0
        0.5f, -0.5f,  // 1
        0.5f, 0.5f,   // 2
        -0.5f, 0.5f}; // 3

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0};

    // buffer
    unsigned int buffer, VAO, ibo;

    // creates/generate 1 buffer object name (since the first parameter is 1)
    glGenBuffers(1, &buffer);

    // creates/generate 1 Array object name (since the first parameter is 1)
    glGenVertexArrays(1, &VAO);

    // bind the vertex array object to the given name
    glBindVertexArray(VAO);

    // bind the buffer object name against the target "GL_ARRAY_BUFFER"
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // creates and initializes the buffer data store
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW);

    // create,bind assigne data to indices buffer:
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // define the array of vertex attribute data for "position" attribute. Other attribute examples: position : 0 (index), texture coordinate: 1, normal : 2
    // Also defines the layout. at index 0 vertex buffer "buffer" is bound
    GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    ShaderSource shaderSource = ParseShader("./Res/basic.shader");

    // create the shaders using the function defined above
    unsigned int shader = CreateShader(shaderSource.vertexShader, shaderSource.fragmentShader);

    // Bind the program returned, which shall be when this main is executed.
    glUseProgram(shader);

    GLCALL(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(!(location == -1));
    GLCALL(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

    // enable the vertex attribute array at index 0:
    glEnableVertexAttribArray(0);

    // Unbind the vertex array, shader, buffer and index buffer
    GLCALL(glBindVertexArray(0));
    GLCALL(glUseProgram(0));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    float r = 0.5f;
    float increment = 0.05f;

    /* Render Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        // clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Following will draw the triangle using the shaders defined above:

        // binding the shader program
        GLCALL(glUseProgram(shader));
        GLCALL(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

        // binding the vertex array object and index buffer since unbound above.
        GLCALL(glBindVertexArray(VAO));
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

        // Draw the square by drawing two triangles using the indices buffer:
        GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f)
        {
            increment = -0.05f;
        }
        else if (r < 0.0f)
        {
            increment = 0.05f;
        }

        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &buffer);
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
