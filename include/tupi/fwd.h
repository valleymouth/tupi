#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace tupi {
using AttachmentDescription = VkAttachmentDescription;
using AttachmentReference = VkAttachmentReference;
using DynamicState = VkDynamicState;
using PipelineColorBlendAttachmentState = VkPipelineColorBlendAttachmentState;
using PresentMode = VkPresentModeKHR;
using SurfaceFormat = VkSurfaceFormatKHR;

class CommandBuffer;
class CommandPool;
class Engine;
class ExtensionSet;
class Fence;
class Frame;
class Framebuffer;
class Image;
class ImageView;
class InputAssembly;
class ISurface;
class IWindow;
class LogicalDevice;
class Pipeline;
class PipelineColorBlendState;
class PipelineDynamicState;
class PipelineLayout;
class PipelineMultisampleState;
class PipelineRasterizationState;
class PhysicalDevice;
class Queue;
class QueueFamily;
class RenderPass;
class Semaphore;
class Shader;
class SubpassDescription;
class Swapchain;
class SwapchainSupportDetail;
class VertexInput;
class ViewportState;

using CommandPoolPtr = std::shared_ptr<CommandPool>;
using EnginePtr = std::shared_ptr<Engine>;
using FencePtr = std::shared_ptr<Fence>;
using FramebufferPtr = std::shared_ptr<Framebuffer>;
using ImagePtr = std::shared_ptr<Image>;
using ImageViewPtr = std::shared_ptr<ImageView>;
using ISurfacePtr = std::shared_ptr<ISurface>;
using IWindowPtr = std::shared_ptr<IWindow>;
using LogicalDevicePtr = std::shared_ptr<LogicalDevice>;
using PipelinePtr = std::unique_ptr<Pipeline>;
using PhysicalDevicePtr = std::shared_ptr<PhysicalDevice>;
using RenderPassPtr = std::shared_ptr<RenderPass>;
using SemaphorePtr = std::shared_ptr<Semaphore>;
using ShaderPtr = std::shared_ptr<Shader>;
using SwapchainPtr = std::shared_ptr<Swapchain>;

using AttachmentDescriptionVec = std::vector<AttachmentDescription>;
using AttachmentReferenceVec = std::vector<AttachmentReference>;
using DynamicStateVec = std::vector<DynamicState>;
using FrameVec = std::vector<Frame>;
using FramebufferPtrVec = std::vector<FramebufferPtr>;
using ImagePtrVec = std::vector<ImagePtr>;
using ImageViewPtrVec = std::vector<ImageViewPtr>;
using PhysicalDevicePtrVec = std::vector<PhysicalDevicePtr>;
using PipelineColorBlendAttachmentStateVec =
    std::vector<PipelineColorBlendAttachmentState>;
using PipelineStageFlagsVec = std::vector<VkPipelineStageFlags>;
using PresentModeVec = std::vector<PresentMode>;
using QueueFamilyVec = std::vector<QueueFamily>;
using SemaphorePtrVec = std::vector<SemaphorePtr>;
using ShaderPtrVec = std::vector<ShaderPtr>;
using SubpassDescriptionVec = std::vector<SubpassDescription>;
using SurfaceFormatVec = std::vector<SurfaceFormat>;

namespace glfw {
class Surface;
class Window;

using SurfacePtr = std::shared_ptr<Surface>;
using WindowPtr = std::shared_ptr<Window>;
}  // namespace glfw
}  // namespace tupi
