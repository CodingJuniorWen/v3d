#include "VInstance.h"

#include "context/VInstanceImpl.h"


namespace VRender {

VInstance::VInstance() : _impl(std::make_unique<VInstanceImpl>()) {}

VInstance::~VInstance() = default;

VInstance::VInstance(VInstance&&) noexcept = default;

VInstance& VInstance::operator=(VInstance&&) noexcept = default;

void VInstance::enableValidation() {
  _impl->enableValidation();
}

void VInstance::disableValidation() {
  _impl->disableValidation();
}

void VInstance::setApplicationInfo(const VkApplicationInfo& appInfo) {
  _impl->setApplicationInfo(appInfo);
}

void VInstance::setEnabledExtensions(
    const std::vector<std::string>& extensions) {
  _impl->setEnabledExtensions(extensions);
}

void VInstance::Init() {
  _impl->Init();
}

VkInstance VInstance::getHandle() const {
  return static_cast<VkInstance>(*_impl->getInstance());
}

}  // namespace VRender
