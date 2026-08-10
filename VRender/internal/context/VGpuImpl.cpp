#include "VGpuImpl.h"

#include <format>
#include <set>
#include <vector>

#include "Logger.h"
#include "utils/DeviceCapability.h"
#include "utils/SwapchainUtils.h"
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

namespace VRender {

VGpuImpl::VGpuImpl() = default;

VGpuImpl::~VGpuImpl() { Shutdown(); }

bool VGpuImpl::Init(const VPhyDeviceImpl& phyDeviceImpl, vk::SurfaceKHR surface,
                    vk::Extent2D windowExtent) {
  if (!surface) {
    UTILS_LOG_ERROR("VGpuImpl: Surface is null");
    return false;
  }

  _surface = surface;

  if (!createDevice(phyDeviceImpl)) {
    return false;
  }
  if (!createSwapchain(windowExtent)) {
    return false;
  }
  return true;
}

bool VGpuImpl::createDevice(const VPhyDeviceImpl& phyDeviceImpl) {
  const auto& indices = phyDeviceImpl.queueFamilyIndices();
  if (!indices.isComplete()) {
    UTILS_LOG_ERROR("VGpuImpl: Queue family indices are incomplete");
    return false;
  }

  _physicalDevice = std::make_optional<vk::raii::PhysicalDevice>(
      phyDeviceImpl.getPhysicalDevice());
  _queueIndices = indices;

  std::set<uint32_t> uniqueQueueFamilies = {
      *_queueIndices.graphicsFamily,
      *_queueIndices.presentFamily,
      *_queueIndices.transferFamily,
  };

  const float queuePriority = 1.0f;
  std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
  queueCreateInfos.reserve(uniqueQueueFamilies.size());
  for (uint32_t familyIndex : uniqueQueueFamilies) {
    queueCreateInfos.push_back(vk::DeviceQueueCreateInfo()
                                   .setQueueFamilyIndex(familyIndex)
                                   .setQueueCount(1)
                                   .setPQueuePriorities(&queuePriority));
  }

  vk::PhysicalDeviceVulkan12Features features12 =
      vk::PhysicalDeviceVulkan12Features()
          .setDescriptorIndexing(true)
          .setShaderSampledImageArrayNonUniformIndexing(true)
          .setRuntimeDescriptorArray(true)
          .setDescriptorBindingVariableDescriptorCount(true)
          .setDescriptorBindingPartiallyBound(true);

  vk::PhysicalDeviceVulkan13Features features13 =
      vk::PhysicalDeviceVulkan13Features()
          .setDynamicRendering(true)
          .setSynchronization2(true)
          .setPNext(&features12);

  vk::PhysicalDeviceFeatures2 features2 =
      vk::PhysicalDeviceFeatures2()
          .setFeatures(phyDeviceImpl.requiredFeatures())
          .setPNext(&features13);

  const auto extensionNames =
      utils::toCStringPointers(phyDeviceImpl.requiredExtensions());

  vk::DeviceCreateInfo deviceInfo =
      vk::DeviceCreateInfo()
          .setQueueCreateInfos(queueCreateInfos)
          .setPEnabledExtensionNames(extensionNames)
          .setPNext(&features2);

  try {
    _device = vk::raii::Device(phyDeviceImpl.getPhysicalDevice(), deviceInfo);
  } catch (const vk::SystemError& error) {
    UTILS_LOG_ERROR(
        std::format("VGpuImpl: Failed to create device: {}", error.what()));
    return false;
  }

  _graphicsQueue = _device.getQueue(*_queueIndices.graphicsFamily, 0);
  _presentQueue = _device.getQueue(*_queueIndices.presentFamily, 0);
  _transferQueue = _device.getQueue(*_queueIndices.transferFamily, 0);

  UTILS_LOG_INFO("VGpuImpl: Logical device and queues created");
  return true;
}

bool VGpuImpl::createSwapchain(vk::Extent2D windowExtent) {
  if (!_physicalDevice.has_value() || !*_device || !_surface) {
    UTILS_LOG_ERROR("VGpuImpl: Device/surface not ready for swapchain");
    return false;
  }

  const auto& physicalDevice = *_physicalDevice;
  const auto capabilities = physicalDevice.getSurfaceCapabilitiesKHR(_surface);
  const auto formats = physicalDevice.getSurfaceFormatsKHR(_surface);
  const auto presentModes = physicalDevice.getSurfacePresentModesKHR(_surface);

  if (formats.empty() || presentModes.empty()) {
    UTILS_LOG_ERROR("VGpuImpl: No swapchain formats/present modes available");
    return false;
  }

  const vk::SurfaceFormatKHR surfaceFormat =
      utils::chooseSwapSurfaceFormat(formats);
  const vk::PresentModeKHR presentMode =
      utils::chooseSwapPresentMode(presentModes);
  const vk::Extent2D extent =
      utils::chooseSwapExtent(capabilities, windowExtent);

  uint32_t imageCount = capabilities.minImageCount + 1;
  if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
    imageCount = capabilities.maxImageCount;
  }

  vk::SwapchainCreateInfoKHR createInfo =
      vk::SwapchainCreateInfoKHR()
          .setSurface(_surface)
          .setMinImageCount(imageCount)
          .setImageFormat(surfaceFormat.format)
          .setImageColorSpace(surfaceFormat.colorSpace)
          .setImageExtent(extent)
          .setImageArrayLayers(1)
          .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
          .setPreTransform(capabilities.currentTransform)
          .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
          .setPresentMode(presentMode)
          .setClipped(true)
          .setOldSwapchain(nullptr);

  const uint32_t queueFamilyIndices[] = {
      *_queueIndices.graphicsFamily,
      *_queueIndices.presentFamily,
  };

  if (*_queueIndices.graphicsFamily != *_queueIndices.presentFamily) {
    createInfo.setImageSharingMode(vk::SharingMode::eConcurrent)
        .setQueueFamilyIndices(queueFamilyIndices);
  } else {
    createInfo.setImageSharingMode(vk::SharingMode::eExclusive);
  }

  try {
    _swapchain = vk::raii::SwapchainKHR(_device, createInfo);
  } catch (const vk::SystemError& error) {
    UTILS_LOG_ERROR(std::format("VGpuImpl: Failed to create swapchain: {}",
                                error.what()));
    return false;
  }

  _swapchainImages = _swapchain.getImages();
  _swapchainImageFormat = surfaceFormat.format;
  _swapchainExtent = extent;

  _swapchainImageViews.clear();
  _swapchainImageViews.reserve(_swapchainImages.size());
  for (const vk::Image image : _swapchainImages) {
    vk::ImageViewCreateInfo viewInfo =
        vk::ImageViewCreateInfo()
            .setImage(image)
            .setViewType(vk::ImageViewType::e2D)
            .setFormat(_swapchainImageFormat)
            .setComponents(vk::ComponentMapping{})
            .setSubresourceRange(vk::ImageSubresourceRange(
                vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

    _swapchainImageViews.emplace_back(_device, viewInfo);
  }

  UTILS_LOG_INFO(std::format(
      "VGpuImpl: Swapchain created ({} images, {}x{})", _swapchainImages.size(),
      _swapchainExtent.width, _swapchainExtent.height));
  return true;
}

void VGpuImpl::destroySwapchain() {
  _swapchainImageViews.clear();
  _swapchainImages.clear();
  _swapchain = nullptr;
  _swapchainImageFormat = vk::Format::eUndefined;
  _swapchainExtent = vk::Extent2D{0, 0};
}

bool VGpuImpl::recreateSwapchain(vk::Extent2D windowExtent) {
  _device.waitIdle();
  destroySwapchain();
  return createSwapchain(windowExtent);
}

void VGpuImpl::Shutdown() {
  if (*_device) {
    _device.waitIdle();
  }

  destroySwapchain();

  _transferQueue = nullptr;
  _presentQueue = nullptr;
  _graphicsQueue = nullptr;
  _device = nullptr;
  _physicalDevice.reset();
  _surface = nullptr;
  _queueIndices = {};
}

}  // namespace VRender
