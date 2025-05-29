#pragma once

#include <optional>

#include <GuiApplication.hpp>

namespace Whirl
{
    std::optional<GuiApplication> CreateApplication(uint32_t width, uint32_t height, const std::string& title);
};