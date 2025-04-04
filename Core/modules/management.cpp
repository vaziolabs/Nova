#include "../core.h"
#define VMA_IMPLEMENTATION
#include "../components/vk_memory.h"

    ///////////////////////////
    // PIPELINE CONSTRUCTION //
    ///////////////////////////

void NovaCore::destroyPipeline(Pipeline* pipeline)
    {
        report(LOGGER::DEBUG, "Management - Destroying Pipeline ..");
        vkDestroyPipeline(logical_device, pipeline->instance, nullptr);
        vkDestroyPipelineLayout(logical_device, pipeline->layout, nullptr);
        delete pipeline;
        return;
    }

void NovaCore::constructGraphicsPipeline()
    { 
        report(LOGGER::DEBUG, "Management - Constructing Graphics Pipeline .."); 
        
        graphics_pipeline = new Pipeline();

        graphics_pipeline->shaders(&logical_device)
                .vertexInput()
                .inputAssembly()
                .viewportState()
                .rasterizer()
                .multisampling()
                .colorBlending()
                .dynamicState()
                .createLayout(&logical_device, &descriptor.layout)
                .pipe(&render_pass)
                .create(&logical_device);

        return; 
    }
    
void NovaCore::constructComputePipeline()
    { 
        report(LOGGER::DEBUG, "Management - Constructing Compute Pipeline .."); 
        
        return; 
    }

    //////////////////////////
    // RENDER PASS CREATION //
    //////////////////////////

VkAttachmentDescription NovaCore::colorAttachment()
    {
        report(LOGGER::VLINE, "\t\t .. Creating Color Attachment ..");

        return {
            .flags = 0,
            .format = swapchain.format,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        };
    }

static VkAttachmentReference colorAttachmentRef()
    {
        report(LOGGER::VLINE, "\t\t .. Creating Color Attachment Reference ..");

        return {
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };
    }

static VkSubpassDescription subpassDescription(VkAttachmentReference* color_attachment_ref)
    {
        report(LOGGER::VLINE, "\t\t .. Creating Subpass Description");

        return {
            .flags = 0,
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .colorAttachmentCount = 1,
            .pColorAttachments = color_attachment_ref
        };
    }

static VkRenderPassCreateInfo renderPassInfo(VkAttachmentDescription* color_attachment, VkSubpassDescription* subpass_description)
    {
        report(LOGGER::VLINE, "\t\t .. Creating Render Pass Info ..");


        return {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .attachmentCount = 1,
            .pAttachments = color_attachment,
            .subpassCount = 1,
            .pSubpasses = subpass_description
        };
    }

void NovaCore::createRenderPass()
    {
        report(LOGGER::VLINE, "\t .. Creating Render Pass ..");

        //log();
        VkAttachmentDescription _color_attachment = colorAttachment();
        VkAttachmentReference _color_attachment_ref = colorAttachmentRef();
        VkSubpassDescription _subpass_description = subpassDescription(&_color_attachment_ref);
        VkRenderPassCreateInfo render_pass_info = renderPassInfo(&_color_attachment, &_subpass_description);
        
        VK_TRY(vkCreateRenderPass(logical_device, &render_pass_info, nullptr, &render_pass));

        return;
    }


VkRenderPassBeginInfo NovaCore::getRenderPassInfo(size_t i)
    {
        return {
                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                .pNext = nullptr,
                .renderPass = render_pass,
                .framebuffer = swapchain.framebuffers[i],
                .renderArea = {
                    .offset = {0, 0},
                    .extent = swapchain.extent
                },
                .clearValueCount = 1,
                .pClearValues = &CLEAR_COLOR
            };
    }

    ///////////////////////////
    // DESCRIPTOR SET LAYOUT //
    ///////////////////////////

static inline VkDescriptorSetLayoutBinding _getLayoutBinding()
    {
        return {
                binding: 0,
                descriptorType: VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                descriptorCount: 1,
                stageFlags: VK_SHADER_STAGE_VERTEX_BIT,
                pImmutableSamplers: nullptr
            };
    }

static inline VkDescriptorSetLayoutCreateInfo _getLayoutInfo(VkDescriptorSetLayoutBinding* bindings)
    {
        return {
                sType: VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
                bindingCount: 1,
                pBindings: bindings
            };
    }

void NovaCore::createDescriptorSetLayout() 
    {
        report(LOGGER::VLINE, "\t .. Creating Descriptor Set Layout ..");

        VkDescriptorSetLayoutBinding _layout_binding = _getLayoutBinding();
        VkDescriptorSetLayoutCreateInfo _layout_info = _getLayoutInfo(&_layout_binding);

        VK_TRY(vkCreateDescriptorSetLayout(logical_device, &_layout_info, nullptr, &descriptor.layout));

        return;
    }

static inline VkDescriptorPoolSize getPoolSize(uint32_t ct)
    {
        report(LOGGER::VLINE, "\t\t .. Creating Descriptor Pool Size of %d ..", ct);

        return {
            .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = ct
        };
    }

static inline VkDescriptorPoolCreateInfo getPoolInfo(uint32_t ct, VkDescriptorPoolSize* size)
    {
        report(LOGGER::VLINE, "\t\t .. Creating Descriptor Pool Info with size %d ..", size->descriptorCount);

        return {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .maxSets = ct,
            .poolSizeCount = 1,
            .pPoolSizes = size
        };
    }

void NovaCore::constructDescriptorPool() 
    {
        report(LOGGER::VLINE, "\t .. Constructing Descriptor Pool ..");

        VkDescriptorPoolSize _pool_size = getPoolSize(MAX_FRAMES_IN_FLIGHT);
        VkDescriptorPoolCreateInfo _pool_info = getPoolInfo(MAX_FRAMES_IN_FLIGHT, &_pool_size);

        VK_TRY(vkCreateDescriptorPool(logical_device, &_pool_info, nullptr, &descriptor.pool));

        return;
    }

static inline VkDescriptorSetAllocateInfo getDescriptorSetAllocateInfo(uint32_t ct, VkDescriptorPool* pool, std::vector<VkDescriptorSetLayout>& layouts)
    {
        report(LOGGER::VLINE, "\t\t .. Creating Descriptor Set Allocate Info ..");

        return {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext = nullptr,
            .descriptorPool = *pool,
            .descriptorSetCount = ct,
            .pSetLayouts = layouts.data()
        };
    }

static inline VkDescriptorBufferInfo getDescriptorBufferInfo(VkBuffer* buffer, VkDeviceSize size)
    {
        report(LOGGER::VLINE, "\t\t .. Creating Descriptor Buffer Info ..");

        return {
            .buffer = *buffer,
            .offset = 0,
            .range = size
        };
    }

static inline VkWriteDescriptorSet getDescriptorWrite(VkDescriptorSet* set, VkDescriptorBufferInfo* buffer_info)
    {
        report(LOGGER::VLINE, "\t\t .. Creating Descriptor Write ..");

        return {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = nullptr,
            .dstSet = *set,
            .dstBinding = 0,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .pImageInfo = nullptr,
            .pBufferInfo = buffer_info,
            .pTexelBufferView = nullptr
        };
    }

void NovaCore::createDescriptorSets() 
    {
        report(LOGGER::VLINE, "\t .. Creating Descriptor Sets ..");

        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptor.layout);
        VkDescriptorSetAllocateInfo _alloc_info = getDescriptorSetAllocateInfo(static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT), &descriptor.pool, layouts);

        descriptor.sets.resize(MAX_FRAMES_IN_FLIGHT);

        VK_TRY(vkAllocateDescriptorSets(logical_device, &_alloc_info, descriptor.sets.data()));

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
            {
                VkDescriptorBufferInfo _buffer_info = getDescriptorBufferInfo(&uniform[i].buffer, sizeof(MVP));
                VkWriteDescriptorSet _write_descriptor = getDescriptorWrite(&descriptor.sets[i], &_buffer_info);

                vkUpdateDescriptorSets(logical_device, 1, &_write_descriptor, 0, nullptr);
            }
    }

    /////////////////////
    // COMMAND BUFFERS //
    /////////////////////

static inline VkCommandPoolCreateInfo createCommandPoolInfo(unsigned int queue_family_index, char* name)
    {
        report(LOGGER::VLINE, "\t\t .. Creating %s Command Pool Info ..", name);
        return {
                .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                .pNext = nullptr,
                .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                .queueFamilyIndex = queue_family_index
            };
    }
    
void NovaCore::createCommandPool() 
    {
        report(LOGGER::VLINE, "\t .. Creating Command Pool ..");

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            char name[32];
            sprintf(name, "Graphics %d", i);
            VkCommandPoolCreateInfo _gfx_cmd_pool_create_info = createCommandPoolInfo(queues.indices.graphics_family.value(), name);
            VK_TRY(vkCreateCommandPool(logical_device, &_gfx_cmd_pool_create_info, nullptr, &frames[i].cmd.pool));
        }

        {
            char name[] = "Transfer";
            VkCommandPoolCreateInfo _xfr_cmd_pool_create_info = createCommandPoolInfo(queues.indices.transfer_family.value(), name);
            VK_TRY(vkCreateCommandPool(logical_device, &_xfr_cmd_pool_create_info, nullptr, &queues.xfr.pool));
        }

        {
            char name[] = "Compute";
            VkCommandPoolCreateInfo _cmp_cmd_pool_create_info = createCommandPoolInfo(queues.indices.compute_family.value(), name);
            VK_TRY(vkCreateCommandPool(logical_device, &_cmp_cmd_pool_create_info, nullptr, &queues.cmp.pool));
        }

        return;
    }

inline VkCommandBufferAllocateInfo NovaCore::createCommandBuffersInfo(VkCommandPool& cmd_pool, char* name)
    {
        report(LOGGER::VLINE, "\t\t .. Creating %s Command Buffer Info  ..", name);

        return {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                .pNext = nullptr,
                .commandPool = cmd_pool,
                .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                .commandBufferCount = 1
            };
    }

void NovaCore::createCommandBuffers() 
    {
        report(LOGGER::VLINE, "\t .. Creating Command Buffers ..");

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            char name[32];
            sprintf(name, "Graphics %d", i);
            VkCommandBufferAllocateInfo _gfx_cmd_buf_alloc_info = createCommandBuffersInfo(frames[i].cmd.pool, name);
            VK_TRY(vkAllocateCommandBuffers(logical_device, &_gfx_cmd_buf_alloc_info, &frames[i].cmd.buffer));
        }

        {
            char name[] = "Transfer";
            VkCommandBufferAllocateInfo _xfr_cmd_buf_alloc_info = createCommandBuffersInfo(queues.xfr.pool, name);
            VK_TRY(vkAllocateCommandBuffers(logical_device, &_xfr_cmd_buf_alloc_info, &queues.xfr.buffer));
        }

        {
            char name[] = "Compute";
            VkCommandBufferAllocateInfo _cmp_cmd_buf_alloc_info = createCommandBuffersInfo(queues.cmp.pool, name);
            VK_TRY(vkAllocateCommandBuffers(logical_device, &_cmp_cmd_buf_alloc_info, &queues.cmp.buffer));
        }

        return;
    }


VkCommandBuffer NovaCore::createEphemeralCommand(VkCommandPool& pool) 
    {
        report(LOGGER::VLINE, "\t .. Creating Ephemeral Command Buffer ..");

        VkCommandBuffer _buffer;

        char name[] = "Ephemeral";
        VkCommandBufferAllocateInfo _tmp_alloc_info = createCommandBuffersInfo(pool, name);

        VK_TRY(vkAllocateCommandBuffers(logical_device, &_tmp_alloc_info, &_buffer));

        VkCommandBufferBeginInfo _begin_info = createBeginInfo();
        VK_TRY(vkBeginCommandBuffer(_buffer, &_begin_info));

        return _buffer;
    }

static inline VkSubmitInfo createSubmitInfo(VkCommandBuffer* cmd)
    {
        report(LOGGER::VLINE, "\t .. Creating Submit Info ..");

        return {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .pWaitDstStageMask = nullptr,
            .commandBufferCount = 1,
            .pCommandBuffers = cmd,
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = nullptr
        };
    }

void NovaCore::flushCommandBuffer(VkCommandBuffer& buf, char* name) 
    {
        report(LOGGER::VLINE, "\t .. Ending %s Command Buffer ..", name);

        VK_TRY(vkEndCommandBuffer(buf));

        // Submit the command buffer
        VkSubmitInfo _submit_info = createSubmitInfo(&buf);
        VK_TRY(vkQueueSubmit(queues.transfer, 1, &_submit_info, VK_NULL_HANDLE));
        VK_TRY(vkQueueWaitIdle(queues.transfer));

        // Free the command buffer
        vkFreeCommandBuffers(logical_device, queues.xfr.pool, 1, &buf);

        return;
    }

void NovaCore::destroyCommandContext()
    {
        report(LOGGER::VERBOSE, "Management - Destroying Semaphores, Fences and Command Pools ..");
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
            {
                vkDestroySemaphore(logical_device, frames[i].image_available, nullptr);
                vkDestroySemaphore(logical_device, frames[i].render_finished, nullptr);
                //vkDestroySemaphore(logical_device, frames[i].transfer_finished, nullptr);
                //vkDestroySemaphore(logical_device, frames[i].compute_finished, nullptr);
                vkDestroyFence(logical_device, frames[i].in_flight, nullptr);
                vkDestroyCommandPool(logical_device, frames[i].cmd.pool, nullptr);
            }

        vkDestroyCommandPool(logical_device, queues.xfr.pool, nullptr);
        vkDestroyCommandPool(logical_device, queues.cmp.pool, nullptr);
    }

    /////////////////////
    // SYNC STRUCTURES //
    /////////////////////

static VkSemaphoreCreateInfo createSemaphoreInfo()
    {
        report(LOGGER::VLINE, "\t .. Creating Semaphore Info ..");

        return {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0
        };
    }

static VkFenceCreateInfo createFenceInfo()
    {
        report(LOGGER::VLINE, "\t .. Creating Fence Info ..");

        return {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_FENCE_CREATE_SIGNALED_BIT
        };
    }

void NovaCore::createSyncObjects() 
    {
        report(LOGGER::VLINE, "\t .. Creating Sync Objects ..");

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            VkSemaphoreCreateInfo semaphore_info = createSemaphoreInfo();
            VkFenceCreateInfo fence_info = createFenceInfo();

            VK_TRY(vkCreateSemaphore(logical_device, &semaphore_info, nullptr, &frames[i].image_available));
            VK_TRY(vkCreateSemaphore(logical_device, &semaphore_info, nullptr, &frames[i].render_finished));
            VK_TRY(vkCreateFence(logical_device, &fence_info, nullptr, &frames[i].in_flight));
        }

        return;
    }

static inline VmaAllocationCreateInfo getVMAAllocationInfo(VmaMemoryUsage mem_usage)
    {
        report(LOGGER::VLINE, "\t .. Creating VMA Allocation Info ..");

        return {
            .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT,
            .usage = mem_usage,
        };
    }

Buffer_T NovaCore::createEphemeralBuffer(size_t size, VkBufferUsageFlags flags, VmaMemoryUsage mem_usage)
    {
        report(LOGGER::VLINE, "\t .. Creating Ephemeral Buffer ..");

        Buffer_T buffer;
    
        VkBufferCreateInfo _buffer_info = getBufferInfo(size, flags);
        VmaAllocationCreateInfo _alloc_info = getVMAAllocationInfo(mem_usage);

        VK_TRY(vmaCreateBuffer(allocator, &_buffer_info, &_alloc_info, &buffer.buffer, &buffer.allocation, nullptr));

        return buffer;
    }

static inline VkCommandBufferSubmitInfo getBufferSubmitInfo(VkCommandBuffer cmd)
    {
        report(LOGGER::VLINE, "\t .. Creating Buffer Submit Info ..");

        return {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
            .pNext = nullptr,
            .commandBuffer = cmd,
            .deviceMask = 0,
        };
    }

static inline VkSubmitInfo2 getSubmitInfo2(VkCommandBufferSubmitInfo* cmd)
    {
        report(LOGGER::VLINE, "\t .. Creating Submit Info 2 ..");

        return {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
            .pNext = nullptr,
            .waitSemaphoreInfoCount = 0,
            .pWaitSemaphoreInfos = nullptr,
            .commandBufferInfoCount = 1,
            .pCommandBufferInfos = cmd,
            .signalSemaphoreInfoCount = 0,
            .pSignalSemaphoreInfos = nullptr
        };
    }

void NovaCore::immediateSubmit(std::function<void(VkCommandBuffer)>&& func)
    {
        VK_TRY(vkResetFences(logical_device, 1, &queues.immediate.fence));
        VK_TRY(vkResetCommandBuffer(queues.immediate.cmd, 0));

        VkCommandBuffer cmd = queues.immediate.cmd;
        VkCommandBufferBeginInfo _begin_info = createBeginInfo();
        _begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VK_TRY(vkBeginCommandBuffer(cmd, &_begin_info));
        func(cmd);
        VK_TRY(vkEndCommandBuffer(cmd));

        VkCommandBufferSubmitInfo _cmd_submit_info = getBufferSubmitInfo(cmd);
        VkSubmitInfo2 _submit_info = getSubmitInfo2(&_cmd_submit_info);

        VK_TRY(vkQueueSubmit2(queues.transfer, 1, &_submit_info, queues.immediate.fence));
        VK_TRY(vkWaitForFences(logical_device, 1, &queues.immediate.fence, VK_TRUE, UINT64_MAX));
    }

MeshBuffer NovaCore::createMeshBuffer(std::span<uint32_t> idx, std::span<Vertex_T> vtx)
    {
        const size_t VERTEX_BUFFER_SIZE = vtx.size() * sizeof(Vertex_T);
        const size_t INDEX_BUFFER_SIZE = idx.size() * sizeof(uint32_t);

        MeshBuffer buffer;

        buffer.vtx_buffer = createEphemeralBuffer(VERTEX_BUFFER_SIZE, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT, VMA_MEMORY_USAGE_GPU_ONLY);
        VkBufferDeviceAddressInfo _buffer_info = { 
            .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
            .pNext = nullptr,
            .buffer = buffer.vtx_buffer.buffer
        };
        buffer.buffer_address = vkGetBufferDeviceAddress(logical_device, &_buffer_info);

        buffer.idx_buffer = createEphemeralBuffer(INDEX_BUFFER_SIZE,  VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY);

        Buffer_T staging_buffer = createEphemeralBuffer(VERTEX_BUFFER_SIZE + INDEX_BUFFER_SIZE, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY);
        void *data = staging_buffer.allocation->GetMappedData();

        memcpy(data, vtx.data(), VERTEX_BUFFER_SIZE);
        memcpy((char*)data + VERTEX_BUFFER_SIZE, idx.data(), INDEX_BUFFER_SIZE);

        immediateSubmit([&](VkCommandBuffer cmd) {
            VkBufferCopy _vertex_copy = {
                .srcOffset = 0,
                .dstOffset = 0,
                .size = VERTEX_BUFFER_SIZE
            };

            vkCmdCopyBuffer(cmd, staging_buffer.buffer, buffer.vtx_buffer.buffer, 1, &_vertex_copy);

            VkBufferCopy _index_copy = {
                .srcOffset = VERTEX_BUFFER_SIZE,
                .dstOffset = 0,
                .size = INDEX_BUFFER_SIZE
            };
            vkCmdCopyBuffer(cmd, staging_buffer.buffer, buffer.idx_buffer.buffer, 1, &_index_copy);
        });

        vmaDestroyBuffer(allocator, staging_buffer.buffer, staging_buffer.allocation);
        return buffer;
    }