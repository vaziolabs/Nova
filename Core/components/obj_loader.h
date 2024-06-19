#pragma once
#include <unordered_map>
#include <vk_memory.h>
#include <filesystem>
#include <optional>
#include <vector>
#include "../../core.h"

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

std::optional<std::vector<std::shared_ptr<MeshType>>> loadGLTFMeshes(NovaCore* engine, std::filesystem::path path);