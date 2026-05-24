#pragma once
#include <string>
#include <memory>
#include "../Styling/Common.h"

namespace ui::components
{
    
    class IComponent
    {
    public:
        IComponent(const std::string& name, std::shared_ptr<ui::styling::IConfig> config)
            : m_isVisible{true}
            , m_name(name)
            , m_config(std::move(config))
        { }

        virtual ~IComponent() = default;

        virtual void render() = 0;

        const std::string& name() const
        {
            return m_name;
        }

        bool isVisible() const
        {
            return m_isVisible;
        }

        ui::styling::IConfig* config()
        {
            return m_config.get();
        }

        void setIsVisible(bool isVisible)
        {
            m_isVisible = isVisible;
        }

    protected:
        bool                                  m_isVisible;
        std::string                           m_name;
        std::shared_ptr<ui::styling::IConfig> m_config;
    };

} // ui::components
