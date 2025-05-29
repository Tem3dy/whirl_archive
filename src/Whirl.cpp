#include <Whirl.hpp>
#include <Logger.hpp>
#include <GuiApplication.hpp>
#include <VideoMode.hpp>

std::optional<GuiApplication> Whirl::CreateApplication(uint32_t width, uint32_t height, const std::string& title)
{
    if (title.empty())
    {
        WHIRL_FATAL("Title cannot be empty");
        return std::nullopt;
    }
    
    return GuiApplication(VideoMode {
        .width = static_cast<int>(width),
        .height = static_cast<int>(height),
        .title = title
    });
}