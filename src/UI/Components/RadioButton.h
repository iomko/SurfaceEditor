#pragma once
#include "Button.h"

namespace ui::components
{

    class RadioButton : public virtual Button
    {
    public:
        RadioButton(
            const std::string&                    name,
            std::vector<RadioButton*>&            group,
            std::shared_ptr<ui::styling::IConfig> config,
            std::function<void(Button*)>          action);

        virtual ~RadioButton() = default;

        void render() override;

        const bool& isSelected() const;

        void setIsSelected(const bool& isSelected);

        std::vector<RadioButton*>& group();

    private:
        bool                       m_isSelected;
        std::vector<RadioButton*>& m_group;
    };

} // ui::components