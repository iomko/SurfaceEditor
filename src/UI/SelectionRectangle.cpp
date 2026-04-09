#include "SelectionRectangle.h"
#include "../Renderer/Renderer.h"

namespace
{
    static constexpr const char* vertexShaderPath   = "../src/Renderer/Shaders/overlayShader.vert";
    static constexpr const char* fragmentShaderPath = "../src/Renderer/Shaders/overlayShader.frag"; 
} // namespace

void SelectionRectangle::create()
{
    m_shader = std::make_unique<Shader>(vertexShaderPath, fragmentShaderPath);
    m_linBuffer.create();
    m_triangleBuffer.create();
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
    clear();

    float x1 = toNDC_X(rectanglePos.startPos.x, width);
    float y1 = toNDC_Y(rectanglePos.startPos.y, height);
    float x2 = toNDC_X(rectanglePos.endPos.x, width);
    float y2 = toNDC_Y(rectanglePos.endPos.y, height);

    m_linBuffer.data.vertices.push_back({{x1, y1, 0.0f}, 0.0f});
    m_linBuffer.data.vertices.push_back({{x2, y1, 0.0f}, 0.0f});
    m_linBuffer.data.vertices.push_back({{x2, y2, 0.0f}, 0.0f});
    m_linBuffer.data.vertices.push_back({{x1, y2, 0.0f}, 0.0f});
    m_linBuffer.update();

    m_triangleBuffer.data.vertices.push_back({{x1, y1, 0.0f}});
    m_triangleBuffer.data.vertices.push_back({{x2, y1, 0.0f}});
    m_triangleBuffer.data.vertices.push_back({{x2, y2, 0.0f}});
    m_triangleBuffer.data.vertices.push_back({{x1, y1, 0.0f}});
    m_triangleBuffer.data.vertices.push_back({{x2, y2, 0.0f}});
    m_triangleBuffer.data.vertices.push_back({{x1, y2, 0.0f}});
    m_triangleBuffer.update();
}

void SelectionRectangle::clear()
{
    m_linBuffer.data.vertices.clear();
    m_triangleBuffer.data.vertices.clear();
}

void SelectionRectangle::draw()
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_shader->bind();

    // fill rectangle
    m_shader->setVec4("u_color", glm::vec4(1.0f, 0.6f, 0.0f, 0.25f));
    Renderer::drawTriangles(m_triangleBuffer.data, m_shader.get());

    // draw border
    m_shader->setVec4("u_color", glm::vec4(1.0f, 0.45f, 0.0f, 1.0f));
    Renderer::drawLineLoop(m_linBuffer.data, m_shader.get());

    m_shader->unbind();

    glEnable(GL_DEPTH_TEST);
}
