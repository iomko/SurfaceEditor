#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "../Renderer/BufferStorages.h"
#include "../Renderer/Shader.h"

struct RectanglePos
{
    glm::vec2 startPos;
    glm::vec2 endPos;
};

class SelectionRectangle
{
public:
    void create();

    void update(int width, int height, const RectanglePos& rectanglePos);

    void clear();

    void draw();

private:
    float toNDC_X(float x, int width);

    float toNDC_Y(float y, int height);

private:
    LineBufferStorage       m_linBuffer;
    TriangleBufferStorage   m_triangleBuffer;
    std::unique_ptr<Shader> m_shader;
};
