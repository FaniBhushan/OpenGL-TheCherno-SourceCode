#include "Renderer.h"
#include <iostream>

void GLClearError()
{
    while (GL_NO_ERROR != glGetError())
        ;
}

bool GLLogCall(const char *function, const char *fileName, int line)
{
    auto errorCode = glGetError();
    if (GL_NO_ERROR != errorCode)
    {
        std::cout << errorCode << " Error in function: " << function << " in file: " << fileName << " in line: " << line << std::endl;
        return false;
    }
    return true;
}