#include "VWindow.h"

#include <vector>

namespace VWindow {

VWindowImpl::VWindowImpl() = default;

VWindowImpl::~VWindowImpl() = default;

void VWindowImpl::init(const unsigned int width, const unsigned int height) {
  if (_window) {
    return;
  }

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow* win =
      glfwCreateWindow(width, height, "VWindow", nullptr, nullptr);
  _window = WindowPtr(win, [](GLFWwindow* ptr) {
    glfwDestroyWindow(ptr);
    glfwTerminate();
  });
}

std::vector<const char*> VWindowImpl::getExtensions() {
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions =
      glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  return std::vector<const char*>(glfwExtensions,
                                  glfwExtensions + glfwExtensionCount);
}

VkSurfaceKHR VWindowImpl::createSurface(VkInstance instance) const {
  if (!_window || !instance) {
    return VK_NULL_HANDLE;
  }

  VkSurfaceKHR surface = VK_NULL_HANDLE;
  if (glfwCreateWindowSurface(instance, _window.get(), nullptr, &surface) !=
      VK_SUCCESS) {
    return VK_NULL_HANDLE;
  }
  return surface;
}

}  // namespace VWindow
