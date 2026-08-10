#pragma once

#include <memory>
#include <string>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "Export.h"

namespace VRender {

class VInstanceImpl;

class VRENDER_EXPORT VInstance {
 public:
  VInstance();
  ~VInstance();

  VInstance(const VInstance&) = delete;
  VInstance& operator=(const VInstance&) = delete;
  VInstance(VInstance&&) noexcept;
  VInstance& operator=(VInstance&&) noexcept;

  void enableValidation();
  void disableValidation();

  void setApplicationInfo(const VkApplicationInfo& appInfo);
  void setEnabledExtensions(const std::vector<std::string>& extensions);

  void Init();

  VkInstance getHandle() const;

 private:
  friend class VPhyDeviceImpl;
  std::unique_ptr<VInstanceImpl> _impl;
};

}  // namespace VRender
