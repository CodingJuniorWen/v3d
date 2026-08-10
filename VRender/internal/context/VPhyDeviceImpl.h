#pragma once
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "VInstance.h"
#include "utils/QueueFamily.h"
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

namespace VRender {

class VPhyDeviceImpl {
 public:
  VPhyDeviceImpl();
  ~VPhyDeviceImpl();

  void setRequiredFeatures(const vk::PhysicalDeviceFeatures& features);
  void requireFeature(vk::Bool32 vk::PhysicalDeviceFeatures::* feature);
  const vk::PhysicalDeviceFeatures& requiredFeatures() const;

  void addRequiredExtension(std::string_view extension);
  void clearRequiredExtensions();
  const std::vector<std::string>& requiredExtensions() const;

  void setRequiredQueueFlags(vk::QueueFlags flags);
  vk::QueueFlags requiredQueueFlags() const;

  bool Init(const VInstance& instance, vk::SurfaceKHR surface);

  vk::raii::PhysicalDevice& getPhysicalDevice();
  const vk::raii::PhysicalDevice& getPhysicalDevice() const;

  const utils::QueueFamilyIndices& queueFamilyIndices() const;

 private:
  bool isDeviceSuitable(const vk::raii::PhysicalDevice& device) const;

 private:
  vk::PhysicalDeviceFeatures _requiredFeatures{};
  std::vector<std::string> _requiredExtensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  vk::QueueFlags _requiredQueueFlags = vk::QueueFlagBits::eGraphics;
  vk::SurfaceKHR _surface = nullptr;
  std::optional<vk::raii::PhysicalDevice> _physicalDevice;
  utils::QueueFamilyIndices _queueIndices;
};

}  // namespace VRender
