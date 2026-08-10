#include "VInstanceImpl.h"
#include "Logger.h"
#include "vulkan/vulkan.hpp"
#include <algorithm>
#include <format>
#include <set>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_raii.hpp>

namespace VRender {

namespace {

constexpr const char *kValidationLayerName = "VK_LAYER_KHRONOS_validation";

VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              void *pUserData) {
    (void)pUserData;

    std::string type;
    switch (messageType) {
    case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
        type = "GENERAL";
        break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
        type = "VALIDATION";
        break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
        type = "PERFORMANCE";
        break;
    default:
        type = "UNKNOWN";
        break;
    }

    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        UTILS_LOG_ERROR(std::format("type: {}, validation layer: {}", type,
                                    pCallbackData->pMessage));
    }
    return VK_FALSE;
}

vk::DebugUtilsMessengerCreateInfoEXT createDebugUtilsMessenger() {
    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
    vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
        vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
    vk::DebugUtilsMessengerCreateInfoEXT createInfo(
        {}, severityFlags, messageTypeFlags, debugCallback);
    return createInfo;
}

vk::DebugUtilsMessengerCreateInfoEXT gDebugUtilsMessenger;

} // namespace

VInstanceImpl::VInstanceImpl() = default;

VInstanceImpl::~VInstanceImpl() { _instance = nullptr; }

void VInstanceImpl::enableValidation() {
    auto it = std::find(_enabledLayers.begin(), _enabledLayers.end(),
                        kValidationLayerName);
    if (it == _enabledLayers.end()) {
        _enabledLayers.emplace_back(kValidationLayerName);
    }

    gDebugUtilsMessenger = createDebugUtilsMessenger();

    _appInfo.pNext = &gDebugUtilsMessenger;
}

void VInstanceImpl::disableValidation() {
    auto it = std::find(_enabledLayers.begin(), _enabledLayers.end(),
                        kValidationLayerName);
    if (it != _enabledLayers.end()) {
        _enabledLayers.erase(it);
    }
}

void VInstanceImpl::setApplicationInfo(const VkApplicationInfo &appInfo) {
    _appInfo = appInfo;
    _createInfo.pApplicationInfo = &_appInfo;
}

void VInstanceImpl::setEnabledExtensions(
    const std::vector<std::string> &extensions) {
    _enabledExtensions = extensions;
}

void VInstanceImpl::Init() {
    _layerNames.clear();
    _layerNames.reserve(_enabledLayers.size());
    for (const auto &layer : _enabledLayers) {
        _layerNames.push_back(layer.c_str());
    }

    _extensionNames.clear();
    _extensionNames.reserve(_enabledExtensions.size());
    for (const auto &extension : _enabledExtensions) {
        _extensionNames.push_back(extension.c_str());
    }

    _createInfo.pApplicationInfo = &_appInfo;
    _createInfo.enabledLayerCount = static_cast<uint32_t>(_layerNames.size());
    _createInfo.ppEnabledLayerNames = _layerNames.data();
    _createInfo.enabledExtensionCount =
        static_cast<uint32_t>(_extensionNames.size());
    _createInfo.ppEnabledExtensionNames = _extensionNames.data();

    auto availableLayers = vk::enumerateInstanceLayerProperties();
    if (availableLayers.empty() && !_layerNames.empty()) {
        UTILS_LOG_ERROR("VInstanceImpl: No available layers");
        return;
    }

    std::set<std::string> needLayers(_enabledLayers.begin(),
                                     _enabledLayers.end());
    for (const auto &iter : availableLayers) {
        needLayers.erase(iter.layerName);
    }

    if (!needLayers.empty()) {
        UTILS_LOG_ERROR("VInstanceImpl: Required layers are not available");
        return;
    }

    _instance = vk::raii::Instance(vk::raii::Context(), _createInfo);
}

} // namespace VRender
