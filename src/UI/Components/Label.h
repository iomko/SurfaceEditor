#pragma once
#include <string>
#include "../Styling/Common.h"
#include "../Components/Common.h"

namespace ui::components
{

    class Label : public IComponent
    {
    public:
        Label(const std::string& text, std::shared_ptr<ui::styling::IConfig> config);

        void render() override;
    };

} // ui::components