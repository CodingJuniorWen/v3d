#pragma once

#include <vector>

#include "vulkan/vulkan.hpp"

namespace VRender::utils {

vk::SurfaceFormatKHR chooseSwapSurfaceFormat(
    const std::vector<vk::SurfaceFormatKHR>& availableFormats);

vk::PresentModeKHR chooseSwapPresentMode(
    const std::vector<vk::PresentModeKHR>& availablePresentModes);

vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities,
                              vk::Extent2D windowExtent);

}  // namespace VRender::utils
