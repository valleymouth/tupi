#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace tupi {
using AttachmentDescription = VkAttachmentDescription;
using AttachmentReference = VkAttachmentReference;
using DescriptorPoolSize = VkDescriptorPoolSize;
using DescriptorSetLayoutBinding = VkDescriptorSetLayoutBinding;
using DynamicState = VkDynamicState;
using Offset = VkDeviceSize;
using PipelineColorBlendAttachmentState = VkPipelineColorBlendAttachmentState;
using PresentMode = VkPresentModeKHR;
using SurfaceFormat = VkSurfaceFormatKHR;
using VertexInputAttributeDescription = VkVertexInputAttributeDescription;
using VertexInputBindingDescription = VkVertexInputBindingDescription;

class Buffer;
class CommandBuffer;
class CommandPool;
class DescriptorPool;
class DescriptorSet;
class DescriptorSetLayout;
class Engine;
class ExtensionSet;
class Fence;
class Frame;
class Framebuffer;
class IImage;
class Image2D;
class ImageView;
class ImageView2D;
class InputAssembly;
class ISurface;
class IWindow;
class LogicalDevice;
class Pipeline;
class PipelineColorBlendState;
class PipelineDepthStencilState;
class PipelineDynamicState;
class PipelineLayout;
class PipelineMultisampleState;
class PipelineRasterizationState;
class PhysicalDevice;
class Queue;
class QueueFamily;
class RenderPass;
class Sampler;
class Semaphore;
class Shader;
class SubpassDescription;
class Swapchain;
class SwapchainImage;
class SwapchainSupportDetail;
class VertexInput;
class ViewportState;

struct Camera;
struct Mesh;
struct Node;
struct QueueCreateInfo;

using BufferSharedPtr = std::shared_ptr<Buffer>;
using CommandBufferPtr = std::unique_ptr<CommandBuffer>;
using CommandPoolSharedPtr = std::shared_ptr<CommandPool>;
using DescriptorPoolSharedPtr = std::shared_ptr<DescriptorPool>;
using DescriptorSetSharedPtr = std::shared_ptr<DescriptorSet>;
using DescriptorSetLayoutSharedPtr = std::shared_ptr<DescriptorSetLayout>;
using EngineSharedPtr = std::shared_ptr<Engine>;
using FenceSharedPtr = std::shared_ptr<Fence>;
using FramebufferPtr = std::shared_ptr<Framebuffer>;
using IImagePtr = std::shared_ptr<IImage>;
using Image2DPtr = std::shared_ptr<Image2D>;
using ImageViewPtr = std::shared_ptr<ImageView>;
using ImageViewWeakPtr = std::weak_ptr<ImageView>;
using ISurfacePtr = std::shared_ptr<ISurface>;
using IWindowPtr = std::shared_ptr<IWindow>;
using LogicalDeviceSharedPtr = std::shared_ptr<LogicalDevice>;
using PipelinePtr = std::shared_ptr<Pipeline>;
using PipelineLayoutPtr = std::shared_ptr<PipelineLayout>;
using PhysicalDeviceSharedPtr = std::shared_ptr<PhysicalDevice>;
using RenderPassPtr = std::shared_ptr<RenderPass>;
using SamplerPtr = std::shared_ptr<Sampler>;
using SemaphorePtr = std::shared_ptr<Semaphore>;
using ShaderPtr = std::shared_ptr<Shader>;
using SwapchainPtr = std::shared_ptr<Swapchain>;
using SwapchainImageSharedPtr = std::shared_ptr<SwapchainImage>;

using AttachmentDescriptionVec = std::vector<AttachmentDescription>;
using AttachmentReferenceVec = std::vector<AttachmentReference>;
using BufferPtrVec = std::vector<BufferSharedPtr>;
using DescriptorPoolSizeVec = std::vector<DescriptorPoolSize>;
using DescriptorSetSharedPtrVec = std::vector<DescriptorSetSharedPtr>;
using DescriptorSetLayoutSharedPtrVec =
    std::vector<DescriptorSetLayoutSharedPtr>;
using DescriptorSetLayoutBindingVec = std::vector<DescriptorSetLayoutBinding>;
using DynamicStateVec = std::vector<DynamicState>;
using FrameVec = std::vector<Frame>;
using FramebufferPtrVec = std::vector<FramebufferPtr>;
using ImagePtrVec = std::vector<IImagePtr>;
using Image2DPtrVec = std::vector<Image2DPtr>;
using ImageViewPtrVec = std::vector<ImageViewPtr>;
using ImageViewWeakPtrVec = std::vector<ImageViewWeakPtr>;
using OffsetVec = std::vector<Offset>;
using PhysicalDeviceSharedPtrVec = std::vector<PhysicalDeviceSharedPtr>;
using PipelineColorBlendAttachmentStateVec =
    std::vector<PipelineColorBlendAttachmentState>;
using PipelineStageFlagsVec = std::vector<VkPipelineStageFlags>;
using PresentModeVec = std::vector<PresentMode>;
using QueueCreateInfoVec = std::vector<QueueCreateInfo>;
using QueueFamilyVec = std::vector<QueueFamily>;
using SamplerPtrVec = std::vector<SamplerPtr>;
using SemaphorePtrVec = std::vector<SemaphorePtr>;
using ShaderPtrVec = std::vector<ShaderPtr>;
using SubpassDescriptionVec = std::vector<SubpassDescription>;
using SurfaceFormatVec = std::vector<SurfaceFormat>;
using SwapchainImageSharedPtrVec = std::vector<SwapchainImageSharedPtr>;
using VertexInputAttributeDescriptionVec =
    std::vector<VertexInputAttributeDescription>;
using VertexInputBindingDescriptionVec =
    std::vector<VertexInputBindingDescription>;

namespace glfw {
class Surface;
class Window;

using SurfaceSharedPtr = std::shared_ptr<Surface>;
using WindowSharedPtr = std::shared_ptr<Window>;
}  // namespace glfw

namespace gltf {
struct Accessor;
struct Buffer;
struct BufferView;
struct Reader;

using AccessorPtr = std::shared_ptr<Accessor>;
using BufferPtr = std::shared_ptr<Buffer>;
using BufferViewPtr = std::shared_ptr<BufferView>;
}  // namespace gltf

}  // namespace tupi
