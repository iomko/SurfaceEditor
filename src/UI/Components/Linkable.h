#pragma once

namespace ui::components
{

    template<typename T>
    class Linkable
    {
    public:
        Linkable()
            : m_val{}
            , m_valPtr{&m_val}
        { }

        virtual ~Linkable() = default;

        void linkWith(Linkable* other)
        {
            m_valPtr = other->m_valPtr;
        }

    protected:
        T  m_val;
        T* m_valPtr;
    };

} // ui::components