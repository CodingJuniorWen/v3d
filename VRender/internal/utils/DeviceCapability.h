#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

namespace VRender::utils {

/// Check that every required feature bit is present in @p available.
bool checkRequiredFeatures(const vk::PhysicalDeviceFeatures& required,
                           const vk::PhysicalDeviceFeatures& available,
                           std::string_view deviceName);

/// Check that @p device exposes every extension in @p requiredExtensions.
bool checkRequiredExtensions(const vk::raii::PhysicalDevice& device,
                             const std::vector<std::string>& requiredExtensions,
                             std::string_view deviceName);

/// Convert owned strings to a C-string view list (pointers valid while @p strings live).
std::vector<const char*> toCStringPointers(
    const std::vector<std::string>& strings);

}  // namespace VRender::utils
