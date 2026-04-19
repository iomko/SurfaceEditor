#include "Window.h"
#include "../VisibilityHandler.h"

namespace ui
{

    namespace
    {
        static constexpr ImVec4 DEFAULT_BACKGROUND(0.15f, 0.15f, 0.17f, 0.85f);
        static constexpr ImVec4 TRANSPARENT_BACKGROUND(0.15f, 0.15f, 0.17f, 0.70f);

        void checkResolutionInRange(const std::string& layer, float height, float width, float minHeight, float minWidth)
        {
            static constexpr const int fullWidth  = 1920;
            static constexpr const int fullHeight = 1080;
            const int minResWidth  = fullWidth * minWidth;
            const int minResHeight = fullHeight * minHeight;

            if (width < minResWidth || height < minResHeight)
            {
                VisibilityHandler::setOutOfRange(layer);
            }
            else
            {
                VisibilityHandler::setInRange(layer);
            }
        }
    } // namespace

    void Window::setPosAndSize(WindowPosConfig& config)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        const ImVec2 vpPos  = viewport->WorkPos;
        const ImVec2 vpSize = viewport->WorkSize;

        config.rawPos  = ImVec2(vpPos.x + config.posX * vpSize.x, vpPos.y + config.posY * vpSize.y);
        config.rawSize = ImVec2(config.width * vpSize.x, config.height * vpSize.y);

        ImGui::SetNextWindowPos(config.rawPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(config.rawSize, ImGuiCond_Always);

        checkResolutionInRange(config.layer, vpSize.y, vpSize.x, config.minHeight, config.minWidth);
    }

    void Window::init(WindowConfig& config)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, config.rounding);
        if (!config.transparent)
        {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, DEFAULT_BACKGROUND);
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, TRANSPARENT_BACKGROUND);
        }
        ImGui::Begin(config.name, nullptr, config.flags);

        ++config.styles.appliedColorStyles;
        ++config.styles.appliedVarStyles;
    }

    void Window::destroy(WindowConfig& config)
    {
        ImGui::End();
        ImGui::PopStyleColor(config.styles.appliedColorStyles);
        ImGui::PopStyleVar(config.styles.appliedVarStyles);

        config.styles.appliedColorStyles = 0;
        config.styles.appliedVarStyles = 0;
    }

    void Window::addToLayout(float margin, std::function<void()> asignComponents)
    {
        //TODO - set margin

        ImGui::BeginGroup();

        asignComponents();
        
        ImGui::EndGroup();
    }

    void Window::nextItem(const Orientation& orientation)
    {
        //TODO
    }

    void Window::drawSeparator(const Orientation& orientation, float length)
    {
        //TODO
    }

} // ui