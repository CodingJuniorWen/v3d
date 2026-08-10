#include "utils/QueueFamily.h"

#include <format>

#include "Logger.h"

namespace VRender::utils {

QueueFamilyIndices findQueueFamilies(const vk::raii::PhysicalDevice& device,
                                     vk::SurfaceKHR surface,
                                     vk::QueueFlags requiredQueueFlags,
                                     std::string_view deviceName) {
  QueueFamilyIndices indices{};

  const auto queueFamilies = device.getQueueFamilyProperties();
  std::optional<uint32_t> graphicsOnly;
  std::optional<uint32_t> presentOnly;
  std::optional<uint32_t> dedicatedTransfer;
  std::optional<uint32_t> anyTransfer;

  for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilies.size()); ++i) {
    const vk::QueueFlags supported = queueFamilies[i].queueFlags;
    const bool graphics = (supported & requiredQueueFlags) == requiredQueueFlags;
    const bool present = device.getSurfaceSupportKHR(i, surface);
    const bool transfer =
        static_cast<bool>(supported & vk::QueueFlagBits::eTransfer) ||
        static_cast<bool>(supported & vk::QueueFlagBits::eGraphics) ||
        static_cast<bool>(supported & vk::QueueFlagBits::eCompute);
    const bool dedicated =
        static_cast<bool>(supported & vk::QueueFlagBits::eTransfer) &&
        !static_cast<bool>(supported & vk::QueueFlagBits::eGraphics) &&
        !static_cast<bool>(supported & vk::QueueFlagBits::eCompute);

    if (graphics && present && !indices.graphicsFamily.has_value()) {
      indices.graphicsFamily = i;
      indices.presentFamily = i;
    }
    if (graphics && !graphicsOnly.has_value()) {
      graphicsOnly = i;
    }
    if (present && !presentOnly.has_value()) {
      presentOnly = i;
    }
    if (dedicated && !dedicatedTransfer.has_value()) {
      dedicatedTransfer = i;
    }
    if (transfer && !anyTransfer.has_value()) {
      anyTransfer = i;
    }
  }

  if (!indices.graphicsFamily.has_value() && graphicsOnly.has_value() &&
      presentOnly.has_value()) {
    indices.graphicsFamily = graphicsOnly;
    indices.presentFamily = presentOnly;
  }

  if (!indices.graphicsFamily.has_value() || !indices.presentFamily.has_value()) {
    UTILS_LOG_ERROR(std::format(
        "QueueFamily: Device '{}' has no suitable graphics/present queue family",
        deviceName));
    return indices;
  }

  if (dedicatedTransfer.has_value()) {
    indices.transferFamily = dedicatedTransfer;
  } else if (anyTransfer.has_value()) {
    indices.transferFamily = anyTransfer;
  } else {
    // Graphics/compute queues implicitly support transfer.
    indices.transferFamily = indices.graphicsFamily;
  }

  return indices;
}

}  // namespace VRender::utils
