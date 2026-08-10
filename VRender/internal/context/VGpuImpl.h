#pragma once
#include <optional>
#include <vector>

#include "VPhyDeviceImpl.h"
#include "utils/QueueFamily.h"
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

namespace VRender {

class VGpuImpl {
 public:
  VGpuImpl();
  ~VGpuImpl();

  bool Init(const VPhyDeviceImpl& phyDeviceImpl, vk::SurfaceKHR surface,
            vk::Extent2D windowExtent);

  bool recreateSwapchain(vk::Extent2D windowExtent);

  void Shutdown();

  vk::raii::Device& getDevice() { return _device; }
  const vk::raii::Device& getDevice() const { return _device; }

  vk::raii::Queue& getGraphicsQueue() { return _graphicsQueue; }
  vk::raii::Queue& getPresentQueue() { return _presentQueue; }
  vk::raii::Queue& getTransferQueue() { return _transferQueue; }

  vk::raii::SwapchainKHR& getSwapchain() { return _swapchain; }
  const vk::raii::SwapchainKHR& getSwapchain() const { return _swapchain; }

  const std::vector<vk::Image>& swapchainImages() const {
    return _swapchainImages;
  }
  const std::vector<vk::raii::ImageView>& swapchainImageViews() const {
    return _swapchainImageViews;
  }

  vk::Format swapchainImageFormat() const { return _swapchainImageFormat; }
  vk::Extent2D swapchainExtent() const { return _swapchainExtent; }

 private:
  bool createDevice(const VPhyDeviceImpl& phyDeviceImpl);
  bool createSwapchain(vk::Extent2D windowExtent);
  void destroySwapchain();

 private:
  std::optional<vk::raii::PhysicalDevice> _physicalDevice;
  vk::SurfaceKHR _surface = nullptr;
  utils::QueueFamilyIndices _queueIndices{};

  vk::raii::Device _device = nullptr;
  vk::raii::Queue _graphicsQueue = nullptr;
  vk::raii::Queue _presentQueue = nullptr;
  vk::raii::Queue _transferQueue = nullptr;

  vk::raii::SwapchainKHR _swapchain = nullptr;
  std::vector<vk::Image> _swapchainImages;
  std::vector<vk::raii::ImageView> _swapchainImageViews;
  vk::Format _swapchainImageFormat = vk::Format::eUndefined;
  vk::Extent2D _swapchainExtent{};
};

}  // namespace VRender
