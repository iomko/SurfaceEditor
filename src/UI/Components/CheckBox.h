#pragma once
#include <string>
#include "../Styling/Common.h"
#include "../Components/Common.h"

namespace ui::components
{

    class CheckBox : public IComponent
    {
    public:
        CheckBox(const std::string& name, std::shared_ptr<ui::styling::IConfig> config);

        void render() override;

        bool* isChecked();

    private:
        bool m_isChecked;
    };

} // ui::components