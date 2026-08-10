#pragma once
#include "Export.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <memory>
#include <vector>

namespace VWindow {

class VWINDOW_EXPORT VWindowImpl {
 public:
  VWindowImpl();

  ~VWindowImpl();

  void init(const unsigned int width = 800, const unsigned int height = 600);

  using WindowPtr = std::shared_ptr<GLFWwindow>;

  WindowPtr getWindow() const { return _window; }

  std::vector<const char*> getExtensions();

  // Requires a valid VkInstance. Caller owns the returned surface and must
  // destroy it with vkDestroySurfaceKHR before destroying the instance.
  VkSurfaceKHR createSurface(VkInstance instance) const;

 private:
  WindowPtr _window{nullptr};
};

}  // namespace VWindow
