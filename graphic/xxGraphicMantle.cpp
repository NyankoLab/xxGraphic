#include "xxGraphicInternal.h"
#include "xxGraphicMantleAsm.h"
#include "xxGraphicMantle.h"

#define GR_PROTOTYPES 1
#include "gr/mantle.h"
#include "gr/mantleWsiWinExt.h"
#define NUM_BACK_BUFFERS 3
#define NUM_DESCRIPTOR_COUNT        (8)
#define BASE_VERTEX_CONSTANT        (0)
#define BASE_PIXEL_CONSTANT         (1)
#define BASE_VERTEX_ATTRIBUTE       (2)
#define BASE_VERTEX_TEXTURE         (2 + 16)
#define BASE_PIXEL_TEXTURE          (2 + 16 + NUM_DESCRIPTOR_COUNT * 2)
#define BASE_VERTEX_SAMPLER         (2 + 16 + NUM_DESCRIPTOR_COUNT * 3)
#define BASE_PIXEL_SAMPLER          (2 + 16 + NUM_DESCRIPTOR_COUNT * 4)

static HMODULE                      g_mantleLibrary = nullptr;
static GR_QUEUE                     g_queue = GR_NULL_HANDLE;
static GR_FENCE                     g_fences[NUM_BACK_BUFFERS] = {};
static int                          g_fenceIndex = 0;
static GR_MEMORY_HEAP_PROPERTIES    g_heapProps = {};
static GR_UINT                      g_suitableHeap = 0;

//==============================================================================
//  Instance
//==============================================================================
static bool grSymbolFailed = false;
static void* grSymbol(const char* name, bool* failed)
{
    void* ptr = nullptr;

    if (ptr == nullptr && g_mantleLibrary)
        ptr = GetProcAddress(g_mantleLibrary, name);

    if (ptr == nullptr)
        xxLog("Mantle", "%s is not found", name);

    grSymbolFailed |= (ptr == nullptr);
    if (failed)
        (*failed) |= (ptr == nullptr);

    return ptr;
}
#define grSymbol(var) (void*&)var = grSymbol(#var, nullptr);
//------------------------------------------------------------------------------
uint64_t xxCreateInstanceMantle()
{
    if (g_mantleLibrary == nullptr)
        g_mantleLibrary = LoadLibraryW(L"mantle32.dll");
    if (g_mantleLibrary == nullptr)
        g_mantleLibrary = LoadLibraryW(L"mantle64.dll");
    if (g_mantleLibrary == nullptr)
        return 0;

    grSymbolFailed = false;
    grSymbol(grInitAndEnumerateGpus);
    grSymbol(grGetGpuInfo);
    grSymbol(grCreateDevice);
    grSymbol(grDestroyDevice);
    grSymbol(grGetExtensionSupport);
    grSymbol(grGetDeviceQueue);
    grSymbol(grQueueWaitIdle);
    grSymbol(grDeviceWaitIdle);
    grSymbol(grQueueSubmit);
    grSymbol(grQueueSetGlobalMemReferences);
    grSymbol(grGetMemoryHeapCount);
    grSymbol(grGetMemoryHeapInfo);
    grSymbol(grAllocMemory);
    grSymbol(grFreeMemory);
    grSymbol(grSetMemoryPriority);
    grSymbol(grMapMemory);
    grSymbol(grUnmapMemory);
    grSymbol(grRemapVirtualMemoryPages);
    grSymbol(grPinSystemMemory);
    grSymbol(grDestroyObject);
    grSymbol(grGetObjectInfo);
    grSymbol(grBindObjectMemory);
    grSymbol(grGetFormatInfo);
    grSymbol(grCreateImage);
    grSymbol(grGetImageSubresourceInfo);
    grSymbol(grCreateSampler);
    grSymbol(grCreateImageView);
    grSymbol(grCreateColorTargetView);
    grSymbol(grCreateShader);
    grSymbol(grCreateGraphicsPipeline);
    grSymbol(grCreateComputePipeline);
    grSymbol(grStorePipeline);
    grSymbol(grLoadPipeline);
    grSymbol(grCreateDescriptorSet);
    grSymbol(grBeginDescriptorSetUpdate);
    grSymbol(grEndDescriptorSetUpdate);
    grSymbol(grAttachSamplerDescriptors);
    grSymbol(grAttachImageViewDescriptors);
    grSymbol(grAttachMemoryViewDescriptors);
    grSymbol(grAttachNestedDescriptors);
    grSymbol(grClearDescriptorSetSlots);
    grSymbol(grCreateViewportState);
    grSymbol(grCreateRasterState);
    grSymbol(grCreateColorBlendState);
    grSymbol(grCreateDepthStencilState);
    grSymbol(grCreateMsaaState);
    grSymbol(grCreateQueryPool);
    grSymbol(grGetQueryPoolResults);
    grSymbol(grCreateFence);
    grSymbol(grGetFenceStatus);
    grSymbol(grWaitForFences);
    grSymbol(grCreateQueueSemaphore);
    grSymbol(grSignalQueueSemaphore);
    grSymbol(grWaitQueueSemaphore);
    grSymbol(grCreateEvent);
    grSymbol(grGetEventStatus);
    grSymbol(grSetEvent);
    grSymbol(grResetEvent);
    grSymbol(grGetMultiGpuCompatibility);
    grSymbol(grOpenSharedMemory);
    grSymbol(grOpenSharedQueueSemaphore);
    grSymbol(grOpenPeerMemory);
    grSymbol(grOpenPeerImage);
    grSymbol(grCreateCommandBuffer);
    grSymbol(grBeginCommandBuffer);
    grSymbol(grEndCommandBuffer);
    grSymbol(grResetCommandBuffer);
    grSymbol(grCmdBindPipeline);
    grSymbol(grCmdBindStateObject);
    grSymbol(grCmdBindDescriptorSet);
    grSymbol(grCmdBindDynamicMemoryView);
    grSymbol(grCmdBindIndexData);
    grSymbol(grCmdBindTargets);
    grSymbol(grCmdPrepareMemoryRegions);
    grSymbol(grCmdPrepareImages);
    grSymbol(grCmdDraw);
    grSymbol(grCmdDrawIndexed);
    grSymbol(grCmdDrawIndirect);
    grSymbol(grCmdDrawIndexedIndirect);
    grSymbol(grCmdDispatch);
    grSymbol(grCmdDispatchIndirect);
    grSymbol(grCmdCopyMemory);
    grSymbol(grCmdCopyImage);
    grSymbol(grCmdCopyMemoryToImage);
    grSymbol(grCmdCopyImageToMemory);
    grSymbol(grCmdResolveImage);
    grSymbol(grCmdCloneImageData);
    grSymbol(grCmdUpdateMemory);
    grSymbol(grCmdFillMemory);
    grSymbol(grCmdClearColorImage);
    grSymbol(grCmdClearColorImageRaw);
    grSymbol(grCmdClearDepthStencil);
    grSymbol(grCmdSetEvent);
    grSymbol(grCmdResetEvent);
    grSymbol(grCmdMemoryAtomic);
    grSymbol(grCmdBeginQuery);
    grSymbol(grCmdEndQuery);
    grSymbol(grCmdResetQueryPool);
    grSymbol(grCmdWriteTimestamp);
    grSymbol(grCmdInitAtomicCounters);
    grSymbol(grCmdLoadAtomicCounters);
    grSymbol(grCmdSaveAtomicCounters);
    if (grSymbolFailed)
        return 0;

    grSymbolFailed = false;
    grSymbol(grWsiWinGetDisplays);
    grSymbol(grWsiWinGetDisplayModeList);
    grSymbol(grWsiWinTakeFullscreenOwnership);
    grSymbol(grWsiWinReleaseFullscreenOwnership);
    grSymbol(grWsiWinSetGammaRamp);
    grSymbol(grWsiWinWaitForVerticalBlank);
    grSymbol(grWsiWinGetScanLine);
    grSymbol(grWsiWinCreatePresentableImage);
    grSymbol(grWsiWinQueuePresent);
    grSymbol(grWsiWinSetMaxQueuedFrames);
    if (grSymbolFailed)
        return 0;

    GR_APPLICATION_INFO appInfo = {};
    appInfo.apiVersion = GR_API_VERSION;

    GR_PHYSICAL_GPU gpus[GR_MAX_PHYSICAL_GPUS] = {};
    GR_UINT gpuCount = GR_MAX_PHYSICAL_GPUS;

    if (grInitAndEnumerateGpus(&appInfo, nullptr, &gpuCount, gpus) != GR_SUCCESS)
        return 0;

    xxRegisterFunction(Mantle);

    return reinterpret_cast<uint64_t>(gpus[0]);
}
//------------------------------------------------------------------------------
void xxDestroyInstanceMantle(uint64_t instance)
{
    if (g_mantleLibrary)
    {
        FreeLibrary(g_mantleLibrary);
        g_mantleLibrary = nullptr;
    }

    xxUnregisterFunction();
}
//==============================================================================
//  Device
//==============================================================================
uint64_t xxCreateDeviceMantle(uint64_t instance)
{
    GR_PHYSICAL_GPU gpu = reinterpret_cast<GR_PHYSICAL_GPU>(instance);

    GR_DEVICE_QUEUE_CREATE_INFO queueInfo = {};
    queueInfo.queueType = GR_QUEUE_UNIVERSAL;
    queueInfo.queueCount = 1;

    static const GR_CHAR* const extensions[] = { "GR_WSI_WINDOWS" };

    GR_DEVICE_CREATE_INFO deviceInfo = {};
    deviceInfo.queueRecordCount = 1;
    deviceInfo.pRequestedQueues = &queueInfo;
    deviceInfo.extensionCount = 1;
    deviceInfo.ppEnabledExtensionNames = extensions;
    deviceInfo.flags |= GR_DEVICE_CREATE_VALIDATION;
    deviceInfo.maxValidationLevel = GR_VALIDATION_LEVEL_4;

    GR_DEVICE device = GR_NULL_HANDLE;
    if (grCreateDevice(gpu, &deviceInfo, &device) != GR_SUCCESS)
        return 0;

    if (grGetDeviceQueue(device, GR_QUEUE_UNIVERSAL, 0, &g_queue) != GR_SUCCESS)
        return 0;

    GR_FENCE_CREATE_INFO fenceCreateInfo = {};
    for (int i = 0; i < NUM_BACK_BUFFERS; ++i)
    {
        GR_FENCE fence = GR_NULL_HANDLE;
        grCreateFence(device, &fenceCreateInfo, &fence);
        g_fences[i] = fence;
    }

    GR_UINT heapCount;
    grGetMemoryHeapCount(device, &heapCount);

    for (GR_UINT i = 0; i < heapCount; i++)
    {
        g_heapProps = {};
        GR_SIZE heapPropSize = sizeof(g_heapProps);

        grGetMemoryHeapInfo(device, i, GR_INFO_TYPE_MEMORY_HEAP_PROPERTIES, &heapPropSize, &g_heapProps);
        if (g_heapProps.flags & GR_MEMORY_HEAP_CPU_VISIBLE)
        {
            g_suitableHeap = i;
            break;
        }
    }

    return reinterpret_cast<uint64_t>(device);
}
//------------------------------------------------------------------------------
void xxDestroyDeviceMantle(uint64_t device)
{
    GR_DEVICE grDevice = reinterpret_cast<GR_DEVICE>(device);
    if (grDevice == GR_NULL_HANDLE)
        return;

    for (int i = 0; i < NUM_BACK_BUFFERS; ++i)
    {
        GR_FENCE fence = g_fences[i];
        grDestroyObject(fence);
    }

    grDestroyDevice(grDevice);
}
//------------------------------------------------------------------------------
void xxResetDeviceMantle(uint64_t device)
{

}
//------------------------------------------------------------------------------
bool xxTestDeviceMantle(uint64_t device)
{
    return true;
}
//------------------------------------------------------------------------------
const char* xxGetDeviceStringMantle(uint64_t device)
{
    return "Mantle";
}
//==============================================================================
//  Framebuffer
//==============================================================================
struct FRAMEBUFFERGR
{
    GR_IMAGE                colorImage;
    GR_GPU_MEMORY           colorMemory;
    GR_COLOR_TARGET_VIEW    colorTargetView;
    GR_IMAGE                depthStencilImage;
    GR_GPU_MEMORY           depthStencilMemory;
    GR_COLOR_TARGET_VIEW    depthStencilTargetView;
};
//==============================================================================
//  Swapchain
//==============================================================================
struct SWAPCHAINGR : public FRAMEBUFFERGR
{
    GR_CMD_BUFFER   commandBuffers[NUM_BACK_BUFFERS];
    int             commandBufferIndex;
    void*           view;
};
//------------------------------------------------------------------------------
uint64_t xxCreateSwapchainMantle(uint64_t device, void* view, unsigned int width, unsigned int height)
{
    GR_DEVICE grDevice = reinterpret_cast<GR_DEVICE>(device);
    if (grDevice == GR_NULL_HANDLE)
        return 0;
    SWAPCHAINGR* grSwapchain = new SWAPCHAINGR;
    if (grSwapchain == nullptr)
        return 0;

    HWND hWnd = (HWND)view;
    if (width == 0 || height == 0)
    {
        RECT rect = {};
        if (GetClientRect(hWnd, &rect) == TRUE)
        {
            width = rect.right - rect.left;
            height = rect.bottom - rect.top;
        }
    }

    GR_WSI_WIN_PRESENTABLE_IMAGE_CREATE_INFO imageCreateInfo = {};
    imageCreateInfo.format.channelFormat = GR_CH_FMT_B8G8R8A8;
    imageCreateInfo.format.numericFormat = GR_NUM_FMT_UNORM;
    imageCreateInfo.usage = GR_IMAGE_USAGE_COLOR_TARGET;
    imageCreateInfo.extent.width = width;
    imageCreateInfo.extent.height = height;

    GR_IMAGE colorImage = GR_NULL_HANDLE;
    GR_GPU_MEMORY colorMemory = GR_NULL_HANDLE;
    if (grWsiWinCreatePresentableImage(grDevice, &imageCreateInfo, &colorImage, &colorMemory) != GR_SUCCESS)
        return 0;

    GR_COLOR_TARGET_VIEW_CREATE_INFO colorTargetViewCreateInfo = {};
    colorTargetViewCreateInfo.image = colorImage;
    colorTargetViewCreateInfo.arraySize = 1;
    colorTargetViewCreateInfo.baseArraySlice = 0;
    colorTargetViewCreateInfo.mipLevel = 0;
    colorTargetViewCreateInfo.format.channelFormat = GR_CH_FMT_B8G8R8A8;
    colorTargetViewCreateInfo.format.numericFormat = GR_NUM_FMT_UNORM;

    GR_COLOR_TARGET_VIEW colorTargetView = GR_NULL_HANDLE;
    grCreateColorTargetView(grDevice, &colorTargetViewCreateInfo, &colorTargetView);

    GR_MEMORY_REF imageMemoryRef = {};
    imageMemoryRef.mem = colorMemory;

    GR_IMAGE_SUBRESOURCE_RANGE imageColorRange = {};
    imageColorRange.aspect = GR_IMAGE_ASPECT_COLOR;
    imageColorRange.baseMipLevel = 0;
    imageColorRange.mipLevels = 1;
    imageColorRange.baseArraySlice = 0;
    imageColorRange.arraySize = 1;

    GR_CMD_BUFFER_CREATE_INFO bufferCreateInfo = {};
    bufferCreateInfo.queueType = GR_QUEUE_UNIVERSAL;

    GR_CMD_BUFFER initCmdBuffer;
    grCreateCommandBuffer(grDevice, &bufferCreateInfo, &initCmdBuffer);
    grBeginCommandBuffer(initCmdBuffer, GR_CMD_BUFFER_OPTIMIZE_ONE_TIME_SUBMIT);

    GR_IMAGE_STATE_TRANSITION initTransition = {};
    initTransition.image = colorImage;
    initTransition.oldState = GR_IMAGE_STATE_UNINITIALIZED;
    initTransition.newState = GR_WSI_WIN_IMAGE_STATE_PRESENT_WINDOWED;
    initTransition.subresourceRange = imageColorRange;
    grCmdPrepareImages(initCmdBuffer, 1, &initTransition);

    grEndCommandBuffer(initCmdBuffer);

    GR_FENCE fence = g_fences[g_fenceIndex];
    grQueueSubmit(g_queue, 1, &initCmdBuffer, 1, &imageMemoryRef, fence);
    grWaitForFences(grDevice, 1, &fence, GR_TRUE, 2.0f);
    grDestroyObject(initCmdBuffer);

    g_fenceIndex++;
    if (g_fenceIndex >= NUM_BACK_BUFFERS)
        g_fenceIndex = 0;

    grSwapchain->colorImage = colorImage;
    grSwapchain->colorMemory = colorMemory;
    grSwapchain->colorTargetView = colorTargetView;
    for (int i = 0; i < NUM_BACK_BUFFERS; ++i)
    {
        GR_CMD_BUFFER commandBuffer;
        grCreateCommandBuffer(grDevice, &bufferCreateInfo, &commandBuffer);
        grSwapchain->commandBuffers[i] = commandBuffer;
    }
    grSwapchain->commandBufferIndex = 0;
    grSwapchain->view = view;

    return reinterpret_cast<uint64_t>(grSwapchain);
}
//------------------------------------------------------------------------------
void xxDestroySwapchainMantle(uint64_t swapchain)
{
    SWAPCHAINGR* grSwapchain = reinterpret_cast<SWAPCHAINGR*>(swapchain);
    if (grSwapchain == nullptr)
        return;

    delete grSwapchain;
}
//------------------------------------------------------------------------------
void xxPresentSwapchainMantle(uint64_t swapchain)
{
    SWAPCHAINGR* grSwapchain = reinterpret_cast<SWAPCHAINGR*>(swapchain);
    if (grSwapchain == nullptr)
        return;

    GR_WSI_WIN_PRESENT_INFO presentInfo = {};
    presentInfo.hWndDest = (HWND)grSwapchain->view;
    presentInfo.srcImage = grSwapchain->colorImage;
    presentInfo.presentMode = GR_WSI_WIN_PRESENT_MODE_WINDOWED;

    grWsiWinQueuePresent(g_queue, &presentInfo);

    grSwapchain->commandBufferIndex++;
    if (grSwapchain->commandBufferIndex >= NUM_BACK_BUFFERS)
        grSwapchain->commandBufferIndex = 0;
}
//------------------------------------------------------------------------------
uint64_t xxGetCommandBufferMantle(uint64_t device, uint64_t swapchain)
{
    GR_DEVICE grDevice = reinterpret_cast<GR_DEVICE>(device);
    if (grDevice == GR_NULL_HANDLE)
        return 0;
    SWAPCHAINGR* grSwapchain = reinterpret_cast<SWAPCHAINGR*>(swapchain);
    if (grSwapchain == nullptr)
        return 0;

    GR_FENCE fence = g_fences[g_fenceIndex];
    grWaitForFences(grDevice, 1, &fence, GR_TRUE, 1.0f);

    g_fenceIndex++;
    if (g_fenceIndex >= NUM_BACK_BUFFERS)
        g_fenceIndex = 0;

    GR_CMD_BUFFER commandBuffer = grSwapchain->commandBuffers[grSwapchain->commandBufferIndex];
    grResetCommandBuffer(commandBuffer);

    return reinterpret_cast<uint64_t>(commandBuffer);
}
//------------------------------------------------------------------------------
uint64_t xxGetFramebufferMantle(uint64_t device, uint64_t swapchain)
{
    FRAMEBUFFERGR* grFramebuffer = reinterpret_cast<SWAPCHAINGR*>(swapchain);
    if (grFramebuffer == nullptr)
        return 0;

    return reinterpret_cast<uint64_t>(grFramebuffer);
}
//==============================================================================
//  Command Buffer
//==============================================================================
bool xxBeginCommandBufferMantle(uint64_t commandBuffer)
{
    GR_CMD_BUFFER grCommandBuffer = reinterpret_cast<GR_CMD_BUFFER>(commandBuffer);
    if (grCommandBuffer == GR_NULL_HANDLE)
        return false;

    grBeginCommandBuffer(grCommandBuffer, 0);

    return true;
}
//------------------------------------------------------------------------------
void xxEndCommandBufferMantle(uint64_t commandBuffer)
{
    GR_CMD_BUFFER grCommandBuffer = reinterpret_cast<GR_CMD_BUFFER>(commandBuffer);

    grEndCommandBuffer(grCommandBuffer);
}
//------------------------------------------------------------------------------
void xxSubmitCommandBufferMantle(uint64_t commandBuffer)
{
    GR_CMD_BUFFER grCommandBuffer = reinterpret_cast<GR_CMD_BUFFER>(commandBuffer);

    grQueueSubmit(g_queue, 1, &grCommandBuffer, 0, nullptr, g_fences[g_fenceIndex]);
}
//==============================================================================
//  Render Pass
//==============================================================================
struct RENDERPASSGR
{
    float   color[4];
    float   depth;
    uint8_t stencil;
};
//------------------------------------------------------------------------------
uint64_t xxCreateRenderPassMantle(uint64_t device, float r, float g, float b, float a, float depth, unsigned char stencil)
{
    RENDERPASSGR* grRenderPass = new RENDERPASSGR;

    grRenderPass->color[0] = r;
    grRenderPass->color[1] = g;
    grRenderPass->color[2] = b;
    grRenderPass->color[3] = a;
    grRenderPass->depth = depth;
    grRenderPass->stencil = stencil;

    return reinterpret_cast<uint64_t>(grRenderPass);
}
//------------------------------------------------------------------------------
void xxDestroyRenderPassMantle(uint64_t renderPass)
{
    RENDERPASSGR* grRenderPass = reinterpret_cast<RENDERPASSGR*>(renderPass);

    delete grRenderPass;
}
//------------------------------------------------------------------------------
bool xxBeginRenderPassMantle(uint64_t commandBuffer, uint64_t framebuffer, uint64_t renderPass)
{
    GR_CMD_BUFFER grCommandBuffer = reinterpret_cast<GR_CMD_BUFFER>(commandBuffer);
    if (grCommandBuffer == GR_NULL_HANDLE)
        return false;
    FRAMEBUFFERGR* grFramebuffer = reinterpret_cast<SWAPCHAINGR*>(framebuffer);
    if (grFramebuffer == nullptr)
        return false;
    RENDERPASSGR* grRenderPass = reinterpret_cast<RENDERPASSGR*>(renderPass);
    if (grRenderPass == nullptr)
        return false;

    grCmdClearColorImage(grCommandBuffer, grFramebuffer->colorImage, grRenderPass->color, 0, nullptr);
    grCmdClearDepthStencil(grCommandBuffer, grFramebuffer->depthStencilImage, grRenderPass->depth, grRenderPass->stencil, 0, nullptr);

    return true;
}
//------------------------------------------------------------------------------
void xxEndRenderPassMantle(uint64_t commandBuffer, uint64_t framebuffer, uint64_t renderPass)
{

}
//==============================================================================
//  Buffer
//==============================================================================
uint64_t xxCreateConstantBufferMantle(uint64_t device, unsigned int size)
{
    GR_DEVICE grDevice = reinterpret_cast<GR_DEVICE>(device);
    if (grDevice == GR_NULL_HANDLE)
        return 0;

    GR_GPU_MEMORY memory;
    GR_MEMORY_ALLOC_INFO allocInfo = {};
    allocInfo.size = (1 + size / g_heapProps.pageSize) * g_heapProps.pageSize;
    allocInfo.alignment = 0;
    allocInfo.memPriority = GR_MEMORY_PRIORITY_HIGH;
    allocInfo.heapCount = 1;
    allocInfo.heaps[0] = g_suitableHeap;
    grAllocMemory(grDevice, &allocInfo, &memory);

    return reinterpret_cast<uint64_t>(memory);
}
//------------------------------------------------------------------------------
uint64_t xxCreateIndexBufferMantle(uint64_t device, unsigned int size)
{
    GR_DEVICE grDevice = reinterpret_cast<GR_DEVICE>(device);
    if (grDevice == GR_NULL_HANDLE)
        return 0;

    GR_GPU_MEMORY memory;
    GR_MEMORY_ALLOC_INFO allocInfo = {};
    allocInfo.size = (1 + size / g_heapProps.pageSize) * g_heapProps.pageSize;
    allocInfo.alignment = 0;
    allocInfo.memPriority = GR_MEMORY_PRIORITY_HIGH;
    allocInfo.heapCount = 1;
    allocInfo.heaps[0] = g_suitableHeap;
    grAllocMemory(grDevice, &allocInfo, &memory);

    return reinterpret_cast<uint64_t>(memory);
}
//------------------------------------------------------------------------------
uint64_t xxCreateVertexBufferMantle(uint64_t device, unsigned int size)
{
    GR_DEVICE grDevice = reinterpret_cast<GR_DEVICE>(device);
    if (grDevice == GR_NULL_HANDLE)
        return 0;

    GR_MEMORY_ALLOC_INFO allocInfo = {};
    allocInfo.size = (1 + size / g_heapProps.pageSize) * g_heapProps.pageSize;
    allocInfo.alignment = 0;
    allocInfo.memPriority = GR_MEMORY_PRIORITY_HIGH;
    allocInfo.heapCount = 1;
    allocInfo.heaps[0] = g_suitableHeap;

    GR_GPU_MEMORY memory = GR_NULL_HANDLE;
    grAllocMemory(grDevice, &allocInfo, &memory);

    return reinterpret_cast<uint64_t>(memory);
}
//------------------------------------------------------------------------------
void xxDestroyBufferMantle(uint64_t device, uint64_t buffer)
{
    GR_GPU_MEMORY memory = reinterpret_cast<GR_GPU_MEMORY>(buffer);
    if (memory == GR_NULL_HANDLE)
        return;

    grFreeMemory(memory);
}
//------------------------------------------------------------------------------
void* xxMapBufferMantle(uint64_t device, uint64_t buffer)
{
    GR_GPU_MEMORY memory = reinterpret_cast<GR_GPU_MEMORY>(buffer);
    if (memory == GR_NULL_HANDLE)
        return nullptr;

    void* ptr = nullptr;
    grMapMemory(memory, 0, &ptr);

    return ptr;
}
//------------------------------------------------------------------------------
void xxUnmapBufferMantle(uint64_t device, uint64_t buffer)
{
    GR_GPU_MEMORY memory = reinterpret_cast<GR_GPU_MEMORY>(buffer);
    if (memory == GR_NULL_HANDLE)
        return;

    grUnmapMemory(memory);
}
//==============================================================================
//  Texture
//==============================================================================
uint64_t xxCreateTextureMantle(uint64_t device, int format, unsigned int width, unsigned int height, unsigned int depth, unsigned int mipmap, unsigned int array)
{
    return 0;
}
//------------------------------------------------------------------------------
void xxDestroyTextureMantle(uint64_t texture)
{

}
//------------------------------------------------------------------------------
void* xxMapTextureMantle(uint64_t device, uint64_t texture, unsigned int& stride, unsigned int level, unsigned int array, unsigned int mipmap)
{
    return nullptr;
}
//------------------------------------------------------------------------------
void xxUnmapTextureMantle(uint64_t device, uint64_t texture, unsigned int level, unsigned int array, unsigned int mipmap)
{

}
//==============================================================================
//  Sampler
//==============================================================================
uint64_t xxCreateSamplerMantle(uint64_t device, bool clampU, bool clampV, bool clampW, bool linearMag, bool linearMin, bool linearMip, int anisotropy)
{
    GR_DEVICE grDevice = reinterpret_cast<GR_DEVICE>(device);
    if (grDevice == GR_NULL_HANDLE)
        return 0;

    static const GR_TEX_FILTER filters[2][2][2] =
    {
        {
            { GR_TEX_FILTER_MAG_POINT_MIN_POINT_MIP_POINT, GR_TEX_FILTER_MAG_POINT_MIN_POINT_MIP_LINEAR },
            { GR_TEX_FILTER_MAG_POINT_MIN_LINEAR_MIP_POINT, GR_TEX_FILTER_MAG_POINT_MIN_LINEAR_MIP_LINEAR },
        },  {
            { GR_TEX_FILTER_MAG_LINEAR_MIN_POINT_MIP_POINT, GR_TEX_FILTER_MAG_LINEAR_MIN_POINT_MIP_LINEAR },
            { GR_TEX_FILTER_MAG_LINEAR_MIN_LINEAR_MIP_POINT, GR_TEX_FILTER_MAG_LINEAR_MIN_LINEAR_MIP_LINEAR },
        }
    };

    GR_SAMPLER_CREATE_INFO info = {};
    info.filter = filters[linearMag][linearMin][linearMip];
    info.addressU = clampU ? GR_TEX_ADDRESS_CLAMP : GR_TEX_ADDRESS_WRAP;
    info.addressV = clampV ? GR_TEX_ADDRESS_CLAMP : GR_TEX_ADDRESS_WRAP;
    info.addressW = clampW ? GR_TEX_ADDRESS_CLAMP : GR_TEX_ADDRESS_WRAP;
    info.maxAnisotropy = anisotropy;
    info.compareFunc = GR_COMPARE_ALWAYS;

    GR_SAMPLER sampler = GR_NULL_HANDLE;
    grCreateSampler(grDevice, &info, &sampler);

    return reinterpret_cast<uint64_t>(sampler);
}
//------------------------------------------------------------------------------
void xxDestroySamplerMantle(uint64_t sampler)
{
    GR_SAMPLER grSampler = reinterpret_cast<GR_SAMPLER>(sampler);
    if (grSampler == GR_NULL_HANDLE)
        return;

    grDestroyObject(grSampler);
}
//==============================================================================
//  Vertex Attribute
//==============================================================================
struct VERTEXATTRIBUTEGR
{
    GR_MEMORY_VIEW_ATTACH_INFO  infos[16];
    int                         count;
    int                         stride;
};
//------------------------------------------------------------------------------
uint64_t xxCreateVertexAttributeMantle(uint64_t device, int count, ...)
{
    VERTEXATTRIBUTEGR* grVertexAttribute = new VERTEXATTRIBUTEGR;
    if (grVertexAttribute == nullptr)
        return 0;

    GR_MEMORY_VIEW_ATTACH_INFO* infos = grVertexAttribute->infos;
    int stride = 0;

    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; ++i)
    {
        int stream = va_arg(args, int);
        int offset = va_arg(args, int);
        int element = va_arg(args, int);
        int size = va_arg(args, int);

        stride += size;

        GR_MEMORY_VIEW_ATTACH_INFO& info = infos[i];

        info.mem = GR_NULL_HANDLE;
        info.offset = offset;
        info.range = size;
        info.stride = 0;
        info.format.channelFormat = GR_CH_FMT_R32;
        info.format.numericFormat = GR_NUM_FMT_FLOAT;
        info.state = GR_MEMORY_STATE_GRAPHICS_SHADER_READ_ONLY;

        if (element == 3 && size == sizeof(float) * 3)
        {
            info.format.channelFormat = GR_CH_FMT_R32G32B32;
            continue;
        }

        if (element == 4 && size == sizeof(char) * 4)
        {
            info.format.channelFormat = GR_CH_FMT_B8G8R8A8;
            info.format.numericFormat = GR_NUM_FMT_UNORM;
            continue;
        }

        if (element == 2 && size == sizeof(float) * 2)
        {
            info.format.channelFormat = GR_CH_FMT_R32G32;
            continue;
        }
    }
    va_end(args);

    for (int i = 0; i < count; ++i)
    {
        infos[i].stride = stride;
    }

    grVertexAttribute->count = count;
    grVertexAttribute->stride = stride;

    return reinterpret_cast<uint64_t>(grVertexAttribute);
}
//------------------------------------------------------------------------------
void xxDestroyVertexAttributeMantle(uint64_t vertexAttribute)
{
    VERTEXATTRIBUTEGR* grVertexAttribute = reinterpret_cast<VERTEXATTRIBUTEGR*>(vertexAttribute);

    delete grVertexAttribute;
}
//==============================================================================
//  Shader
//==============================================================================
uint64_t xxCreateVertexShaderMantle(uint64_t device, const char* shader, uint64_t vertexAttribute)
{
    GR_DEVICE grDevice = reinterpret_cast<GR_DEVICE>(device);
    if (grDevice == GR_NULL_HANDLE)
        return 0;

    if (strcmp(shader, "default") == 0)
    {
        GR_SHADER_CREATE_INFO info = {};
        info.pCode = vertexShaderCodeIL;
        info.codeSize = sizeof(vertexShaderCodeIL);

        GR_SHADER grShader = GR_NULL_HANDLE;
        grCreateShader(grDevice, &info, &grShader);

        return reinterpret_cast<uint64_t>(grShader);
    }

    return 0;
}
//------------------------------------------------------------------------------
uint64_t xxCreateFragmentShaderMantle(uint64_t device, const char* shader)
{
    GR_DEVICE grDevice = reinterpret_cast<GR_DEVICE>(device);
    if (grDevice == GR_NULL_HANDLE)
        return 0;

    if (strcmp(shader, "default") == 0)
    {
        GR_SHADER_CREATE_INFO info = {};
        info.pCode = pixelShaderCodeIL;
        info.codeSize = sizeof(pixelShaderCodeIL);

        GR_SHADER grShader = GR_NULL_HANDLE;
        grCreateShader(grDevice, &info, &grShader);

        return reinterpret_cast<uint64_t>(grShader);
    }

    return 0;
}
//------------------------------------------------------------------------------
void xxDestroyShaderMantle(uint64_t device, uint64_t shader)
{
    GR_SHADER grShader = reinterpret_cast<GR_SHADER>(shader);
    if (grShader == GR_NULL_HANDLE)
        return;

    grDestroyObject(grShader);
}
//==============================================================================
//  Pipeline
//==============================================================================
uint64_t xxCreateBlendStateMantle(uint64_t device, bool blending)
{
    GR_DEVICE grDevice = reinterpret_cast<GR_DEVICE>(device);
    if (grDevice == GR_NULL_HANDLE)
        return 0;

    GR_COLOR_BLEND_STATE_CREATE_INFO info = {};
    info.target[0].blendEnable = blending ? GR_TRUE : GR_FALSE;
    info.target[0].srcBlendColor = GR_BLEND_SRC_ALPHA;
    info.target[0].destBlendColor = GR_BLEND_ONE_MINUS_SRC_ALPHA;
    info.target[0].blendFuncColor = GR_BLEND_FUNC_ADD;
    info.target[0].srcBlendAlpha = GR_BLEND_SRC_ALPHA;
    info.target[0].destBlendAlpha = GR_BLEND_ONE_MINUS_SRC_ALPHA;
    info.target[0].blendFuncAlpha = GR_BLEND_FUNC_ADD;

    GR_COLOR_BLEND_STATE_OBJECT blendState = GR_NULL_HANDLE;
    grCreateColorBlendState(grDevice, &info, &blendState);

    return reinterpret_cast<uint64_t>(blendState);
}
//------------------------------------------------------------------------------
uint64_t xxCreateDepthStencilStateMantle(uint64_t device, bool depthTest, bool depthWrite)
{
    GR_DEVICE grDevice = reinterpret_cast<GR_DEVICE>(device);
    if (grDevice == GR_NULL_HANDLE)
        return 0;

    GR_DEPTH_STENCIL_STATE_CREATE_INFO info = {};
    info.depthEnable = depthTest ? GR_TRUE : GR_FALSE;
    info.depthWriteEnable = depthWrite ? GR_TRUE : GR_FALSE;
    info.depthFunc = GR_COMPARE_LESS_EQUAL;

    GR_DEPTH_STENCIL_STATE_OBJECT depthStencilState = GR_NULL_HANDLE;
    grCreateDepthStencilState(grDevice, &info, &depthStencilState);

    return reinterpret_cast<uint64_t>(depthStencilState);
}
//------------------------------------------------------------------------------
uint64_t xxCreateRasterizerStateMantle(uint64_t device, bool cull, bool scissor)
{
    GR_DEVICE grDevice = reinterpret_cast<GR_DEVICE>(device);
    if (grDevice == GR_NULL_HANDLE)
        return 0;

    GR_RASTER_STATE_CREATE_INFO info = {};
    info.fillMode = GR_FILL_SOLID;
    info.cullMode = GR_CULL_NONE;
    info.frontFace = GR_FRONT_FACE_CCW;

    GR_RASTER_STATE_OBJECT rasterizerState = GR_NULL_HANDLE;
    grCreateRasterState(grDevice, &info, &rasterizerState);

    return reinterpret_cast<uint64_t>(rasterizerState);
}
//------------------------------------------------------------------------------
uint64_t xxCreatePipelineMantle(uint64_t device, uint64_t blendState, uint64_t depthStencilState, uint64_t rasterizerState, uint64_t vertexAttribute, uint64_t vertexShader, uint64_t fragmentShader)
{
    GR_DEVICE grDevice = reinterpret_cast<GR_DEVICE>(device);
    if (grDevice == GR_NULL_HANDLE)
        return 0;
    GR_SHADER grVertexShader = reinterpret_cast<GR_SHADER>(vertexShader);
    if (grVertexShader == GR_NULL_HANDLE)
        return 0;
    GR_SHADER grPixelShader = reinterpret_cast<GR_SHADER>(fragmentShader);
    if (grPixelShader == GR_NULL_HANDLE)
        return 0;

    GR_DESCRIPTOR_SLOT_INFO slotInfo = {};
    slotInfo.slotObjectType = GR_SLOT_SHADER_RESOURCE;
    slotInfo.shaderEntityIndex = 0;

    GR_GRAPHICS_PIPELINE_CREATE_INFO pipelineInfo = {};
    pipelineInfo.vs.shader = grVertexShader;
    pipelineInfo.vs.descriptorSetMapping[0].descriptorCount = 1;
    pipelineInfo.vs.descriptorSetMapping[0].pDescriptorInfo = &slotInfo;
    pipelineInfo.vs.dynamicMemoryViewMapping.slotObjectType = GR_SLOT_UNUSED;
    pipelineInfo.ps.shader = grPixelShader;
    pipelineInfo.ps.dynamicMemoryViewMapping.slotObjectType = GR_SLOT_UNUSED;

    pipelineInfo.iaState.topology = GR_TOPOLOGY_TRIANGLE_LIST;
    pipelineInfo.rsState.depthClipEnable = GR_FALSE;
    pipelineInfo.cbState.logicOp = GR_LOGIC_OP_COPY;
    pipelineInfo.cbState.target[0].blendEnable = GR_FALSE;
    pipelineInfo.cbState.target[0].channelWriteMask = 0xF;
    pipelineInfo.cbState.target[0].format.channelFormat = GR_CH_FMT_B8G8R8A8;
    pipelineInfo.cbState.target[0].format.numericFormat = GR_NUM_FMT_UNORM;
    pipelineInfo.dbState.format.channelFormat = GR_CH_FMT_R32G8;
    pipelineInfo.dbState.format.numericFormat = GR_NUM_FMT_DS;

    GR_PIPELINE pipeline = GR_NULL_HANDLE;
    grCreateGraphicsPipeline(grDevice, &pipelineInfo, &pipeline);

    return reinterpret_cast<uint64_t>(pipeline);
}
//------------------------------------------------------------------------------
void xxDestroyBlendStateMantle(uint64_t blendState)
{
    GR_COLOR_BLEND_STATE_OBJECT grBlendState = reinterpret_cast<GR_COLOR_BLEND_STATE_OBJECT>(blendState);
    if (grBlendState == GR_NULL_HANDLE)
        return;

    grDestroyObject(grBlendState);
}
//------------------------------------------------------------------------------
void xxDestroyDepthStencilStateMantle(uint64_t depthStencilState)
{
    GR_DEPTH_STENCIL_STATE_OBJECT grDepthStencilState = reinterpret_cast<GR_DEPTH_STENCIL_STATE_OBJECT>(depthStencilState);
    if (grDepthStencilState == GR_NULL_HANDLE)
        return;

    grDestroyObject(grDepthStencilState);
}
//------------------------------------------------------------------------------
void xxDestroyRasterizerStateMantle(uint64_t rasterizerState)
{
    GR_RASTER_STATE_OBJECT grRasterizerState = reinterpret_cast<GR_PIPELINE>(rasterizerState);
    if (grRasterizerState == GR_NULL_HANDLE)
        return;

    grDestroyObject(grRasterizerState);
}
//------------------------------------------------------------------------------
void xxDestroyPipelineMantle(uint64_t pipeline)
{
    GR_PIPELINE grPipeline = reinterpret_cast<GR_PIPELINE>(pipeline);
    if (grPipeline == GR_NULL_HANDLE)
        return;

    grDestroyObject(grPipeline);
}
//==============================================================================
//  Command
//==============================================================================
void xxSetViewportMantle(uint64_t commandBuffer, int x, int y, int width, int height, float minZ, float maxZ)
{

}
//------------------------------------------------------------------------------
void xxSetScissorMantle(uint64_t commandBuffer, int x, int y, int width, int height)
{

}
//------------------------------------------------------------------------------
void xxSetPipelineMantle(uint64_t commandBuffer, uint64_t pipeline)
{
    GR_CMD_BUFFER grCommandBuffer = reinterpret_cast<GR_CMD_BUFFER>(commandBuffer);
    GR_PIPELINE grPipeline = reinterpret_cast<GR_PIPELINE>(pipeline);

    grCmdBindPipeline(grCommandBuffer, GR_PIPELINE_BIND_POINT_GRAPHICS, grPipeline);
}
//------------------------------------------------------------------------------
void xxSetIndexBufferMantle(uint64_t commandBuffer, uint64_t buffer)
{
    GR_CMD_BUFFER grCommandBuffer = reinterpret_cast<GR_CMD_BUFFER>(commandBuffer);
    GR_GPU_MEMORY grMemory = reinterpret_cast<GR_GPU_MEMORY>(buffer);

    grCmdBindIndexData(grCommandBuffer, grMemory, 0, GR_INDEX_32);
}
//------------------------------------------------------------------------------
void xxSetVertexBuffersMantle(uint64_t commandBuffer, int count, const uint64_t* buffers, uint64_t vertexAttribute)
{
    VERTEXATTRIBUTEGR* grVertexAttribute = reinterpret_cast<VERTEXATTRIBUTEGR*>(vertexAttribute);
    GR_MEMORY_VIEW_ATTACH_INFO* infos = grVertexAttribute->infos;

    for (int i = 0; i < count; ++i)
    {
        GR_GPU_MEMORY grMemory = reinterpret_cast<GR_GPU_MEMORY>(buffers[i]);
        GR_MEMORY_VIEW_ATTACH_INFO& info = infos[i];
        info.mem = grMemory;
    }

    grAttachMemoryViewDescriptors(0, BASE_VERTEX_ATTRIBUTE, count, infos);
}
//------------------------------------------------------------------------------
void xxSetVertexTexturesMantle(uint64_t commandBuffer, int count, const uint64_t* textures)
{
    GR_IMAGE_VIEW_ATTACH_INFO infos[8];

    for (int i = 0; i < count; ++i)
    {
        GR_IMAGE_VIEW_ATTACH_INFO& info = infos[i];
        info.view;
        info.state = GR_MEMORY_STATE_GRAPHICS_SHADER_READ_ONLY;
    }

    grAttachImageViewDescriptors(0, BASE_VERTEX_TEXTURE, count, infos);
}
//------------------------------------------------------------------------------
void xxSetFragmentTexturesMantle(uint64_t commandBuffer, int count, const uint64_t* textures)
{
    GR_IMAGE_VIEW_ATTACH_INFO infos[8];

    for (int i = 0; i < count; ++i)
    {
        GR_IMAGE_VIEW_ATTACH_INFO& info = infos[i];
        info.view;
        info.state = GR_MEMORY_STATE_GRAPHICS_SHADER_READ_ONLY;
    }

    grAttachImageViewDescriptors(0, BASE_PIXEL_TEXTURE, count, infos);
}
//------------------------------------------------------------------------------
void xxSetVertexSamplersMantle(uint64_t commandBuffer, int count, const uint64_t* samplers)
{
    GR_SAMPLER grSamplers[8];

    for (int i = 0; i < count; ++i)
    {
        GR_SAMPLER grSampler = reinterpret_cast<GR_SAMPLER>(samplers[i]);
        grSamplers[i] = grSampler;
    }

    grAttachSamplerDescriptors(0, BASE_VERTEX_SAMPLER, count, grSamplers);
}
//------------------------------------------------------------------------------
void xxSetFragmentSamplersMantle(uint64_t commandBuffer, int count, const uint64_t* samplers)
{
    GR_SAMPLER grSamplers[8];

    for (int i = 0; i < count; ++i)
    {
        GR_SAMPLER grSampler = reinterpret_cast<GR_SAMPLER>(samplers[i]);
        grSamplers[i] = grSampler;
    }

    grAttachSamplerDescriptors(0, BASE_PIXEL_SAMPLER, count, grSamplers);
}
//------------------------------------------------------------------------------
void xxSetVertexConstantBufferMantle(uint64_t commandBuffer, uint64_t buffer, unsigned int size)
{
    GR_GPU_MEMORY grMemory = reinterpret_cast<GR_GPU_MEMORY>(buffer);

    GR_MEMORY_VIEW_ATTACH_INFO info = {};
    info.mem = grMemory;
    info.offset = 0;
    info.range = size;
    info.stride = 0;
    info.format.channelFormat = GR_CH_FMT_R32G32B32A32;
    info.format.numericFormat = GR_NUM_FMT_FLOAT;
    info.state = GR_MEMORY_STATE_GRAPHICS_SHADER_READ_ONLY;

    grAttachMemoryViewDescriptors(0, BASE_VERTEX_CONSTANT, 1, &info);
}
//------------------------------------------------------------------------------
void xxSetFragmentConstantBufferMantle(uint64_t commandBuffer, uint64_t buffer, unsigned int size)
{
    GR_GPU_MEMORY grMemory = reinterpret_cast<GR_GPU_MEMORY>(buffer);

    GR_MEMORY_VIEW_ATTACH_INFO info = {};
    info.mem = grMemory;
    info.offset = 0;
    info.range = size;
    info.stride = 0;
    info.format.channelFormat = GR_CH_FMT_R32G32B32A32;
    info.format.numericFormat = GR_NUM_FMT_FLOAT;
    info.state = GR_MEMORY_STATE_GRAPHICS_SHADER_READ_ONLY;

    grAttachMemoryViewDescriptors(0, BASE_PIXEL_CONSTANT, 1, &info);
}
//------------------------------------------------------------------------------
void xxDrawIndexedMantle(uint64_t commandBuffer, int indexCount, int instanceCount, int firstIndex, int vertexOffset, int firstInstance)
{
    GR_CMD_BUFFER grCommandBuffer = reinterpret_cast<GR_CMD_BUFFER>(commandBuffer);

    grCmdDrawIndexed(grCommandBuffer, firstIndex, indexCount, vertexOffset, firstInstance, instanceCount);
}
//==============================================================================
//  Fixed-Function
//==============================================================================
void xxSetTransformMantle(uint64_t commandBuffer, const float* world, const float* view, const float* projection)
{

}
//==============================================================================
