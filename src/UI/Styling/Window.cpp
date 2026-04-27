#include "Window.h"
#include "../VisibilityHandler.h"

namespace ui::styling
{

    namespace
    {
        void checkResolutionInRange(const std::string& layer, float height, float width, float minHeight, float minWidth)
        {
            static constexpr int fullWidth  = 1920;
            static constexpr int fullHeight = 1080;
            
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

        void initPosAndSize(WindowConfig& config, bool isPosResponsive)
        {
            auto& posConfig  = config.pos;
            auto& sizeConfig = config.size;

            const ImGuiViewport* viewport = ImGui::GetMainViewport();

            const ImVec2 vpPos  = viewport->WorkPos;
            const ImVec2 vpSize = viewport->WorkSize;

            if (isPosResponsive)
            {
                posConfig.rawPos   = ImVec2(vpPos.x + posConfig.posX * vpSize.x, vpPos.y + posConfig.posY * vpSize.y);
            }
            sizeConfig.rawSize = ImVec2(sizeConfig.width * vpSize.x, sizeConfig.height * vpSize.y);

            ImGui::SetNextWindowPos(posConfig.rawPos, ImGuiCond_Always);
            ImGui::SetNextWindowSize(sizeConfig.rawSize, ImGuiCond_Always);

            checkResolutionInRange(config.name, vpSize.y, vpSize.x, sizeConfig.minHeight, sizeConfig.minWidth);
        }
    } // namespace

    void Window::setPosAndSize(WindowConfig& config)
    {
        bool isPosResponsive = true;

        initPosAndSize(config, isPosResponsive);
    }

    void Window::setRelativePosAndSize(WindowConfig& config)
    {
        bool isPosResponsive = false;

        initPosAndSize(config, isPosResponsive);
    }

    void Window::init(WindowConfig& config)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, config.rounding);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, config.backgroundColor);
        ImGui::Begin(config.name.c_str(), nullptr, config.flags);

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

    void Window::addToLayout(std::function<void()> asignComponents)
    {
        ImGui::BeginGroup();

        asignComponents();
        
        ImGui::EndGroup();
    }

} // ui::styling