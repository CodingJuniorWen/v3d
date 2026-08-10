#pragma once

#include <optional>
#include <string_view>

#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

namespace VRender::utils {

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;
  std::optional<uint32_t> transferFamily;

  bool isComplete() const {
    return graphicsFamily.has_value() && presentFamily.has_value() &&
           transferFamily.has_value();
  }
};

/// Find graphics / present / transfer queue families for @p device.
/// Returns incomplete indices and logs on failure.
QueueFamilyIndices findQueueFamilies(const vk::raii::PhysicalDevice& device,
                                     vk::SurfaceKHR surface,
                                     vk::QueueFlags requiredQueueFlags,
                                     std::string_view deviceName);

}  // namespace VRender::utils
