#pragma once

#include <GL/glew.h>
#include <iostream>

#define ASSERT(x) \
    if (!x)       \
    abort()
#define GLCALL(x)   \
    GLClearError(); \
    x;              \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char *function, const char *fileName, int line);
