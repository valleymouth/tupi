#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

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
class Camera;
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
class IImageView;
class IKeyboardObserver;
class Image2D;
class ImageView2D;
class IMouseObserver;
class InputAssembly;
class ISurface;
class ITexture;
class ITickObserver;
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
class Texture2D;
class VertexInput;
class ViewportState;

struct Mesh;
struct Node;
struct QueueCreateInfo;

using BufferSharedPtr = std::shared_ptr<Buffer>;
using CameraSharedPtr = std::shared_ptr<Camera>;
using CommandBufferPtr = std::unique_ptr<CommandBuffer>;
using CommandPoolSharedPtr = std::shared_ptr<CommandPool>;
using DescriptorPoolSharedPtr = std::shared_ptr<DescriptorPool>;
using DescriptorSetSharedPtr = std::shared_ptr<DescriptorSet>;
using DescriptorSetLayoutSharedPtr = std::shared_ptr<DescriptorSetLayout>;
using EngineSharedPtr = std::shared_ptr<Engine>;
using FenceSharedPtr = std::shared_ptr<Fence>;
using FramebufferSharedPtr = std::shared_ptr<Framebuffer>;
using IImageSharedPtr = std::shared_ptr<IImage>;
using IImageViewSharedPtr = std::shared_ptr<IImageView>;
using IKeyboardObserverSharedPtr = std::shared_ptr<IKeyboardObserver>;
using Image2DSharedPtr = std::shared_ptr<Image2D>;
using ImageView2DSharedPtr = std::shared_ptr<ImageView2D>;
using IMouseObserverSharedPtr = std::shared_ptr<IMouseObserver>;
using ISurfaceSharedPtr = std::shared_ptr<ISurface>;
using ITextureSharedPtr = std::shared_ptr<ITexture>;
using ITickObserverSharedPtr = std::shared_ptr<ITickObserver>;
using IWindowSharedPtr = std::shared_ptr<IWindow>;
using LogicalDeviceSharedPtr = std::shared_ptr<LogicalDevice>;
using PipelineSharedPtr = std::shared_ptr<Pipeline>;
using PipelineLayoutSharedPtr = std::shared_ptr<PipelineLayout>;
using PhysicalDeviceSharedPtr = std::shared_ptr<PhysicalDevice>;
using RenderPassSharedPtr = std::shared_ptr<RenderPass>;
using SamplerSharedPtr = std::shared_ptr<Sampler>;
using SemaphoreSharedPtr = std::shared_ptr<Semaphore>;
using ShaderSharedPtr = std::shared_ptr<Shader>;
using SwapchainSharedPtr = std::shared_ptr<Swapchain>;
using SwapchainImageSharedPtr = std::shared_ptr<SwapchainImage>;

using AttachmentDescriptionVec = std::vector<AttachmentDescription>;
using AttachmentReferenceVec = std::vector<AttachmentReference>;
using BufferSharedPtrVec = std::vector<BufferSharedPtr>;
using DescriptorPoolSizeVec = std::vector<DescriptorPoolSize>;
using DescriptorSetSharedPtrVec = std::vector<DescriptorSetSharedPtr>;
using DescriptorSetLayoutSharedPtrVec =
    std::vector<DescriptorSetLayoutSharedPtr>;
using DescriptorSetLayoutBindingVec = std::vector<DescriptorSetLayoutBinding>;
using DynamicStateVec = std::vector<DynamicState>;
using FrameVec = std::vector<Frame>;
using FramebufferSharedPtrVec = std::vector<FramebufferSharedPtr>;
using IImageSharedPtrVec = std::vector<IImageSharedPtr>;
using IKeyboardObserverSharedPtrVec = std::vector<IKeyboardObserverSharedPtr>;
using Image2DSharedPtrVec = std::vector<Image2DSharedPtr>;
using IImageViewSharedPtrVec = std::vector<IImageViewSharedPtr>;
using ImageView2DSharedPtrVec = std::vector<ImageView2DSharedPtr>;
using IMouseObserverSharedPtrVec = std::vector<IMouseObserverSharedPtr>;
using ITickObserverSharedPtrVec = std::vector<ITickObserverSharedPtr>;
using OffsetVec = std::vector<Offset>;
using PhysicalDeviceSharedPtrVec = std::vector<PhysicalDeviceSharedPtr>;
using PipelineColorBlendAttachmentStateVec =
    std::vector<PipelineColorBlendAttachmentState>;
using PipelineStageFlagsVec = std::vector<VkPipelineStageFlags>;
using PresentModeVec = std::vector<PresentMode>;
using QueueCreateInfoVec = std::vector<QueueCreateInfo>;
using QueueFamilyVec = std::vector<QueueFamily>;
using SamplerSharedPtrVec = std::vector<SamplerSharedPtr>;
using SemaphoreSharedPtrVec = std::vector<SemaphoreSharedPtr>;
using ShaderSharedPtrVec = std::vector<ShaderSharedPtr>;
using SubpassDescriptionVec = std::vector<SubpassDescription>;
using SurfaceFormatVec = std::vector<SurfaceFormat>;
using SwapchainImageSharedPtrVec = std::vector<SwapchainImageSharedPtr>;
using ITextureSharedPtrVec = std::vector<ITextureSharedPtr>;
using VertexInputAttributeDescriptionVec =
    std::vector<VertexInputAttributeDescription>;
using VertexInputBindingDescriptionVec =
    std::vector<VertexInputBindingDescription>;

namespace glfw {
class DefaultCameraInput;
class Surface;
class Window;

using DefaultCameraInputSharedPtr = std::shared_ptr<DefaultCameraInput>;
using SurfaceSharedPtr = std::shared_ptr<Surface>;
using WindowSharedPtr = std::shared_ptr<Window>;
}  // namespace glfw

namespace gltf {
struct Accessor;
struct Buffer;
struct BufferView;
struct File;
struct Image;
struct Material;
struct Mesh;
struct MeshPrimitive;
struct Node;
struct Sampler;
struct Scene;
struct Texture;
}  // namespace gltf
}  // namespace tupi
