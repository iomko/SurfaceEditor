#include "SelectionRectangle.h"

namespace
{
    static constexpr const char* vertexShaderPath   = "../src/Renderer/Shaders/overlayShader.vert";
    static constexpr const char* fragmentShaderPath = "../src/Renderer/Shaders/overlayShader.frag"; 
} // namespace


SelectionRectangle::SelectionRectangle()
    : m_active{}, m_shader(vertexShaderPath, fragmentShaderPath) {}

void SelectionRectangle::create()
{
    m_buffer.create();
}

float SelectionRectangle::toNDC_X(float x, int width)
{
    return (2.0f * x) / width - 1.0f;
}

float SelectionRectangle::toNDC_Y(float y, int height)
{
    return 1.0f - (2.0f * y) / height;
}

void SelectionRectangle::update(int width, int height, const RectanglePos& rectanglePos)
{
    float x1 = toNDC_X(rectanglePos.startPos.x, width);
    float y1 = toNDC_Y(rectanglePos.startPos.y, height);
    float x2 = toNDC_X(rectanglePos.endPos.x, width);
    float y2 = toNDC_Y(rectanglePos.endPos.y, height);

    m_buffer.data.vertices.clear();
    m_buffer.data.vertices.push_back({{x1, y1, 0.0f}, 0.0f});
    m_buffer.data.vertices.push_back({{x2, y1, 0.0f}, 0.0f});
    m_buffer.data.vertices.push_back({{x2, y2, 0.0f}, 0.0f});
    m_buffer.data.vertices.push_back({{x1, y2, 0.0f}, 0.0f});

    m_buffer.update();
}

void SelectionRectangle::clear()
{
    m_buffer.data.vertices.clear();
}
