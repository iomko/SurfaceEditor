#pragma once
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
    SelectionRectangle();

    ~SelectionRectangle() = default;

    void create();

    void update(int width, int height, const RectanglePos& rectanglePos);

    void clear();

    inline LineBufferStorage& buffer() { return m_buffer; }

    inline Shader& shader() { return m_shader; }

    inline const bool& active() const { return m_active; }

    inline void setActive(const bool& active) { m_active = active; }

private:
    float toNDC_X(float x, int width);

    float toNDC_Y(float y, int height);

private:
    LineBufferStorage m_buffer;
    Shader            m_shader;
    bool              m_active;
};
