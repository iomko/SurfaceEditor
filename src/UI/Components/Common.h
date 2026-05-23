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
            : m_name(name)
            , m_config(std::move(config))
        { }

        virtual ~IComponent() = default;

        virtual void render() = 0;

        virtual const std::string& name() const
        {
            return m_name;
        }

        ui::styling::IConfig* config()
        {
            return m_config.get();
        }

    protected:
        std::string                           m_name;
        std::shared_ptr<ui::styling::IConfig> m_config;
    };

} // ui::components
