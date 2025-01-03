#pragma once
#include "../../components/lexicon.h"
#include "../../components/vertex.h"
#include "../../components/vk_memory.h"

#include <optional>
#include <vector>
#include <deque>
#include <functional>
#include <glm/glm.hpp>


    //////////////////
    // Object Types //
    //////////////////

// These could possibly be moved to ObjectLoader
struct Vertex_T {
    glm::vec3 position; // TODO: figure out how to include vec4 for physics
    glm::vec3 normal;
    glm::vec2 uv_coord;
    glm::vec4 color;
};

struct VertexRange {
    size_t start;
    size_t end;
};

struct Buffer_T {
    VkBuffer buffer;
    VmaAllocation allocation;
    VmaAllocationInfo info;
};

struct MeshBuffer {
    Buffer_T idx_buffer;
    Buffer_T vtx_buffer;
    VkDeviceAddress buffer_address;
};

struct MeshType {
    std::string name;
    std::vector<VertexRange> meshes;
    MeshBuffer buffer;
};

typedef void (*fnManifest)();

    /////////////////////
    // LOCAL VARIABLES //
    /////////////////////

const bool USE_VALIDATION_LAYERS = true;
constexpr unsigned int MAX_FRAMES_IN_FLIGHT = 2;
const std::vector<const char*> VALIDATION_LAYERS = { "VK_LAYER_KHRONOS_validation" };
const uint32_t VALIDATION_LAYER_COUNT = static_cast<uint32_t>(VALIDATION_LAYERS.size());
const std::vector<const char*> DEVICE_EXTENSIONS = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, };


    ////////////////////////
    // STRUCT DEFINITIONS //
    ////////////////////////

struct DeletionQueue 
    {
        std::deque<std::function<void()>> deletors;

        void push_fn(std::function<void()> fn) { deletors.push_back(fn); }
        void flush() { for (auto it = deletors.rbegin(); it != deletors.rend(); it++) { (*it)(); } deletors.clear(); }
    };


struct CommandContext
    {
        VkCommandPool pool;
        VkCommandBuffer buffer;
    };


struct FrameData 
    {
        VkSemaphore image_available;
        VkSemaphore render_finished;
        VkSemaphore transfer_finished;
        VkSemaphore compute_finished;
        VkFence in_flight;
        DeletionQueue deletion_queue;
        CommandContext cmd;
    };

struct QueueFamilyIndices 
    {
        std::optional<unsigned int> graphics_family = -1;
        std::optional<unsigned int> present_family = -1;
        std::optional<unsigned int> transfer_family = -1;
        std::optional<unsigned int> compute_family = -1;

        bool isComplete() {
            return graphics_family >= 0 && present_family >= 0 && transfer_family >= 0 && compute_family >= 0;
        }
    };

struct Immediate
    {
        VkFence fence;
        VkCommandBuffer cmd;
    };

// This could be a class that constructs queues and families dynamically
struct Queues 
    {
        VkQueue graphics;
        VkQueue present;
        VkQueue transfer;
        VkQueue compute;

        DeletionQueue deletion;

        CommandContext xfr;
        CommandContext cmp;

        std::vector<VkQueueFamilyProperties> families;
        QueueFamilyIndices indices;
        std::vector<std::vector<float>> priorities;

        Immediate immediate;
    };

struct SwapChainSupportDetails 
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

struct SwapChainDetails 
    {
        VkSurfaceFormatKHR  surface_format;
        VkPresentModeKHR    present_mode;
        VkExtent2D          extent; // TODO: Make 3D
    };

struct SwapChainContext 
    {
        VkSwapchainKHR instance;
        std::vector<VkImage> images;
        std::vector<VkImageView> image_views;
        std::vector<VkFramebuffer> framebuffers;
        VkFormat format;
        VkExtent2D extent;
        SwapChainSupportDetails support;
        SwapChainDetails details;
    };

struct QueuePresentContext 
    {
        VkSubmitInfo submit_info;
        VkPresentInfoKHR present_info;
    };

struct BufferContext 
    {
        VkBuffer buffer;
        VkDeviceMemory memory;
    };


struct MVP 
    {
        glm::mat4 model; // The model matrix is the one that will be used to transform the vertices of the model
        glm::mat4 view;  // The view matrix is the one that will be used to transform the vertices of the world
        glm::mat4 proj;  // The projection matrix is the one that will be used to transform the vertices of the camera
    };

struct DescriptorContext
    {
        VkDescriptorSetLayout layout;
        VkDescriptorPool pool;
        std::vector<VkDescriptorSet> sets;
    };

    ////////////////////////
    // DEBUGGER & LOGGING //
    ////////////////////////

void createDebugMessenger(VkInstance *instance, VkDebugUtilsMessengerEXT *_debug_messenger);
void destroyDebugUtilsMessengerEXT(VkInstance, VkDebugUtilsMessengerEXT, const VkAllocationCallbacks*);
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT, 
                                                    VkDebugUtilsMessageTypeFlagsEXT, 
                                                    const VkDebugUtilsMessengerCallbackDataEXT*, 
                                                    void*);
