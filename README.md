# OpenGL-TheCherno-SourceCode

This source code is from the Youtube tutorial for OpenGL from "The Cherno"
https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2

Platform
I am using Macbook pro (Big Sur v11.6) to compile this.

Prerequisites
- glfw
- glew

Details of installation is in the Youtube video.

command to compile Application.cpp with glfw and glew libraries:
 g++ $(pkg-config --cflags glfw3 glew) -o Application Application.cpp $(pkg-config --static --libs glfw3 glew) -framework OpenGL