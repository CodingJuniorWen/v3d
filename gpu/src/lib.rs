mod device;
use vulkano::{instance::*,physical::PhysicalDevice, VulkanLibrary};
use std::sync::*;
use std::error::*;

pub struct Library{
    inner: (Arc<VulkanLibrary>, Arc<Instance>),
}

impl Library {
    fn new() -> Self {
        let inner = VulkanLibrary::new().expect("Failed to create Vulkan library");

        let instance = Instance::new(
            inner.clone(),
            InstanceCreateInfo {
                application_name: Some("My Vulkan App".into()),
                flags: InstanceCreateFlags::ENUMERATE_PORTABILITY,
                ..Default::default()
            },
        ).expect("Failed to create Vulkan instance");

        Library { inner: (inner, instance) }
    }


    fn get_library<T>(&self, fun: impl FnOnce(&VulkanLibrary) -> Result<T, Box<dyn Error>>) -> Result<T, Box<dyn Error>> {
        fun(&self.inner.0)
    }

    fn get_instance<T>(&self, fun: impl FnOnce(&Instance) -> Result<T, Box<dyn Error>>) -> Result<T, Box<dyn Error>> {
        fun(&self.inner.1)
    }
}


unsafe impl Send for Library {}
unsafe impl Sync for Library {}