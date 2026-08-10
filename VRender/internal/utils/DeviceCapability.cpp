#include "utils/DeviceCapability.h"

#include <vulkan/vulkan_core.h>

#include <algorithm>
#include <format>
#include <iterator>
#include <string_view>

#include "Logger.h"

namespace VRender::utils {
namespace {

constexpr std::string_view kFeatureNames[] = {
    "robustBufferAccess",
    "fullDrawIndexUint32",
    "imageCubeArray",
    "independentBlend",
    "geometryShader",
    "tessellationShader",
    "sampleRateShading",
    "dualSrcBlend",
    "logicOp",
    "multiDrawIndirect",
    "drawIndirectFirstInstance",
    "depthClamp",
    "depthBiasClamp",
    "fillModeNonSolid",
    "depthBounds",
    "wideLines",
    "largePoints",
    "alphaToOne",
    "multiViewport",
    "samplerAnisotropy",
    "textureCompressionETC2",
    "textureCompressionASTC_LDR",
    "textureCompressionBC",
    "occlusionQueryPrecise",
    "pipelineStatisticsQuery",
    "vertexPipelineStoresAndAtomics",
    "fragmentStoresAndAtomics",
    "shaderTessellationAndGeometryPointSize",
    "shaderImageGatherExtended",
    "shaderStorageImageExtendedFormats",
    "shaderStorageImageMultisample",
    "shaderStorageImageReadWithoutFormat",
    "shaderStorageImageWriteWithoutFormat",
    "shaderUniformBufferArrayDynamicIndexing",
    "shaderSampledImageArrayDynamicIndexing",
    "shaderStorageBufferArrayDynamicIndexing",
    "shaderStorageImageArrayDynamicIndexing",
    "shaderClipDistance",
    "shaderCullDistance",
    "shaderFloat64",
    "shaderInt64",
    "shaderInt16",
    "shaderResourceResidency",
    "shaderResourceMinLod",
    "sparseBinding",
    "sparseResidencyBuffer",
    "sparseResidencyImage2D",
    "sparseResidencyImage3D",
    "sparseResidency2Samples",
    "sparseResidency4Samples",
    "sparseResidency8Samples",
    "sparseResidency16Samples",
    "sparseResidencyAliased",
    "variableMultisampleRate",
    "inheritedQueries",
};

static_assert(sizeof(VkPhysicalDeviceFeatures) / sizeof(VkBool32) ==
                  std::size(kFeatureNames),
              "Feature name table must match VkPhysicalDeviceFeatures");

bool isFeatureRequiredAndMissing(VkBool32 required, VkBool32 available) {
  return required == VK_TRUE && available != VK_TRUE;
}

}  // namespace

bool checkRequiredFeatures(const vk::PhysicalDeviceFeatures& required,
                           const vk::PhysicalDeviceFeatures& available,
                           std::string_view deviceName) {
  const auto& req = static_cast<VkPhysicalDeviceFeatures>(required);
  const auto& avail = static_cast<VkPhysicalDeviceFeatures>(available);

  const auto* reqFlags = reinterpret_cast<const VkBool32*>(&req);
  const auto* availFlags = reinterpret_cast<const VkBool32*>(&avail);
  constexpr size_t featureCount = std::size(kFeatureNames);

  bool supported = true;
  for (size_t i = 0; i < featureCount; ++i) {
    if (isFeatureRequiredAndMissing(reqFlags[i], availFlags[i])) {
      UTILS_LOG_ERROR(std::format(
          "DeviceCapability: Device '{}' missing required feature '{}'",
          deviceName, kFeatureNames[i]));
      supported = false;
    }
  }
  return supported;
}

bool checkRequiredExtensions(const vk::raii::PhysicalDevice& device,
                             const std::vector<std::string>& requiredExtensions,
                             std::string_view deviceName) {
  if (requiredExtensions.empty()) {
    return true;
  }

  const auto available = device.enumerateDeviceExtensionProperties();
  std::vector<std::string_view> missing;
  missing.reserve(requiredExtensions.size());

  for (const auto& required : requiredExtensions) {
    const bool found =
        std::any_of(available.begin(), available.end(),
                    [&required](const vk::ExtensionProperties& props) {
                      return std::string_view(props.extensionName) == required;
                    });
    if (!found) {
      missing.emplace_back(required);
    }
  }

  if (!missing.empty()) {
    for (const auto& extension : missing) {
      UTILS_LOG_ERROR(std::format(
          "DeviceCapability: Device '{}' missing required extension '{}'",
          deviceName, extension));
    }
    return false;
  }
  return true;
}

std::vector<const char*> toCStringPointers(
    const std::vector<std::string>& strings) {
  std::vector<const char*> pointers;
  pointers.reserve(strings.size());
  for (const auto& value : strings) {
    pointers.push_back(value.c_str());
  }
  return pointers;
}

}  // namespace VRender::utils
