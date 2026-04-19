#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>

class ImageLoader
{
public:
    ImageLoader() = delete;
    
    ~ImageLoader() = delete;

    static GLuint loadImage(const char* filePath);
};