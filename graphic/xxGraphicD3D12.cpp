#include "xxGraphicD3D12.h"
#include "xxGraphicD3DAsm.h"
#include "xxGraphicInternal.h"

#include <d3d12.h>
#include <dxgi1_4.h>
typedef HRESULT (WINAPI *PFN_CREATE_DXGI_FACTORY1)(REFIID, void**);
#define NUM_BACK_BUFFERS 3
#define PERSISTENT_BUFFER 1

static HMODULE                      g_d3dLibrary = nullptr;
static HMODULE                      g_dxgiLibrary = nullptr;
static IDXGIFactory4*               g_dxgiFactory = nullptr;
static ID3D12CommandQueue*          g_commandQueue = nullptr;
static ID3D12Fence*                 g_fence = nullptr;
static HANDLE                       g_fenceEvent = nullptr;
static UINT64                       g_fenceValue = 0;
static UINT64                       g_fenceValues[NUM_BACK_BUFFERS] = {};
static ID3D12DescriptorHeap*        g_shaderHeap = nullptr;
static UINT                         g_shaderDescriptorSize = 0;
static UINT                         g_shaderAvailableHandle = 0;
static D3D12_CPU_DESCRIPTOR_HANDLE  g_shaderCPUHandles[256] = {};
static D3D12_GPU_DESCRIPTOR_HANDLE  g_shaderGPUHandles[256] = {};
static ID3D12DescriptorHeap*        g_samplerHeap = nullptr;
static UINT                         g_samplerDescriptorSize = 0;
static UINT                         g_samplerAvailableHandle = 0;
static D3D12_CPU_DESCRIPTOR_HANDLE  g_samplerCPUHandles[256] = {};
static D3D12_GPU_DESCRIPTOR_HANDLE  g_samplerGPUHandles[256] = {};
static ID3D12RootSignature*         g_rootSignature = nullptr;

//==============================================================================
//  Fence
//==============================================================================
static void signalFence(bool wait)
{
    g_commandQueue->Signal(g_fence, g_fenceValue);
    if (wait)
    {
        g_fence->SetEventOnCompletion(g_fenceValue, g_fenceEvent);
        WaitForSingleObjectEx(g_fenceEvent, 1000, FALSE);
    }
    g_fenceValue++;
}
//==============================================================================
//  Heap
//==============================================================================
static void createShaderHeap(D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle)
{
    g_shaderAvailableHandle--;
    cpuHandle = g_shaderCPUHandles[g_shaderAvailableHandle];
    gpuHandle = g_shaderGPUHandles[g_shaderAvailableHandle];
}
//------------------------------------------------------------------------------
static void destroyShaderHeap(D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle)
{
    if (cpuHandle.ptr == 0 && gpuHandle.ptr == 0)
        return;
    if (cpuHandle.ptr == 0)
    {
        cpuHandle.ptr = g_shaderHeap->GetCPUDescriptorHandleForHeapStart().ptr + (SIZE_T)(gpuHandle.ptr - g_shaderHeap->GetGPUDescriptorHandleForHeapStart().ptr);
    }
    if (gpuHandle.ptr == 0)
    {
        gpuHandle.ptr = g_shaderHeap->GetGPUDescriptorHandleForHeapStart().ptr + (UINT64)(cpuHandle.ptr - g_shaderHeap->GetCPUDescriptorHandleForHeapStart().ptr);
    }
    g_shaderCPUHandles[g_shaderAvailableHandle] = cpuHandle;
    g_shaderGPUHandles[g_shaderAvailableHandle] = gpuHandle;
    g_shaderAvailableHandle++;
}
//------------------------------------------------------------------------------
static void createSamplerHeap(D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle)
{
    g_samplerAvailableHandle--;
    cpuHandle = g_samplerCPUHandles[g_samplerAvailableHandle];
    gpuHandle = g_samplerGPUHandles[g_samplerAvailableHandle];
}
//------------------------------------------------------------------------------
static void destroySamplerHeap(D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle)
{
    if (cpuHandle.ptr == 0 && gpuHandle.ptr == 0)
        return;
    if (cpuHandle.ptr == 0)
    {
        cpuHandle.ptr = g_samplerHeap->GetCPUDescriptorHandleForHeapStart().ptr + (SIZE_T)(gpuHandle.ptr - g_samplerHeap->GetGPUDescriptorHandleForHeapStart().ptr);
    }
    if (gpuHandle.ptr == 0)
    {
        gpuHandle.ptr = g_samplerHeap->GetGPUDescriptorHandleForHeapStart().ptr + (UINT64)(cpuHandle.ptr - g_samplerHeap->GetCPUDescriptorHandleForHeapStart().ptr);
    }
    g_samplerCPUHandles[g_samplerAvailableHandle] = cpuHandle;
    g_samplerGPUHandles[g_samplerAvailableHandle] = gpuHandle;
    g_samplerAvailableHandle++;
}
//==============================================================================
//  Instance
//==============================================================================
uint64_t xxCreateInstanceD3D12()
{
    if (g_d3dLibrary == nullptr)
        g_d3dLibrary = LoadLibraryW(L"d3d12.dll");
    if (g_d3dLibrary == nullptr)
        return 0;

    if (g_dxgiLibrary == nullptr)
        g_dxgiLibrary = LoadLibraryW(L"dxgi.dll");
    if (g_dxgiLibrary == nullptr)
        return 0;

    xxRegisterFunction(D3D12);

    return reinterpret_cast<uint64_t>(g_d3dLibrary);
}
//------------------------------------------------------------------------------
void xxDestroyInstanceD3D12(uint64_t instance)
{
    if (g_dxgiLibrary)
    {
        FreeLibrary(g_dxgiLibrary);
        g_dxgiLibrary = nullptr;
    }

    if (g_d3dLibrary)
    {
        FreeLibrary(g_d3dLibrary);
        g_d3dLibrary = nullptr;
    }

    xxUnregisterFunction();
}
//==============================================================================
//  Device
//==============================================================================
uint64_t xxCreateDeviceD3D12(uint64_t instance)
{
    PFN_D3D12_CREATE_DEVICE D3D12CreateDevice;
    (void*&)D3D12CreateDevice = GetProcAddress(g_d3dLibrary, "D3D12CreateDevice");
    if (D3D12CreateDevice == nullptr)
        return 0;

    PFN_D3D12_SERIALIZE_ROOT_SIGNATURE D3D12SerializeRootSignature;
    (void*&)D3D12SerializeRootSignature = GetProcAddress(g_d3dLibrary, "D3D12SerializeRootSignature");
    if (D3D12SerializeRootSignature == nullptr)
        return 0;

#if defined(_DEBUG)
    ID3D12Debug* debug = nullptr;
    if (debug == nullptr)
    {
        PFN_D3D12_GET_DEBUG_INTERFACE D3D12GetDebugInterface;
        (void*&)D3D12GetDebugInterface = GetProcAddress(g_d3dLibrary, "D3D12GetDebugInterface");
        if (D3D12GetDebugInterface)
        {
            HRESULT hResult = D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
            if (hResult == S_OK)
            {
                debug->EnableDebugLayer();
                debug->Release();
            }
        }
    }
#endif

    ID3D12Device* d3dDevice = nullptr;
    HRESULT hResult = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3dDevice));
    if (hResult != S_OK)
        return 0;

    if (g_commandQueue == nullptr)
    {
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        HRESULT hResult = d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&g_commandQueue));
        if (hResult != S_OK)
            return 0;
    }

    if (g_fence == nullptr)
    {
        HRESULT hResult = d3dDevice->CreateFence(g_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_fence));
        if (hResult != S_OK)
            return 0;

        g_fenceValue++;
        memset(g_fenceValues, 0, sizeof(g_fenceValues));
    }

    if (g_fenceEvent == nullptr)
    {
        g_fenceEvent = CreateEventW(nullptr, FALSE, TRUE, nullptr);
        if (g_fenceEvent == nullptr)
            return 0;
    }

    if (g_shaderHeap == nullptr)
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors = 256;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        desc.NodeMask = 1;

        HRESULT hResult = d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_shaderHeap));
        if (hResult != S_OK)
            return 0;

        D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = g_shaderHeap->GetCPUDescriptorHandleForHeapStart();
        D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = g_shaderHeap->GetGPUDescriptorHandleForHeapStart();
        g_shaderDescriptorSize = d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        g_shaderAvailableHandle = 256;
        for (int i = 0; i < 256; ++i)
        {
            g_shaderCPUHandles[i] = cpuHandle;
            g_shaderGPUHandles[i] = gpuHandle;
            cpuHandle.ptr += g_shaderDescriptorSize;
            gpuHandle.ptr += g_shaderDescriptorSize;
        }
    }

    if (g_samplerHeap == nullptr)
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
        desc.NumDescriptors = 256;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        desc.NodeMask = 1;

        HRESULT hResult = d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_samplerHeap));
        if (hResult != S_OK)
            return 0;

        D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = g_samplerHeap->GetCPUDescriptorHandleForHeapStart();
        D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = g_samplerHeap->GetGPUDescriptorHandleForHeapStart();
        g_samplerDescriptorSize = d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
        g_samplerAvailableHandle = 256;
        for (int i = 0; i < 256; ++i)
        {
            g_samplerCPUHandles[i] = cpuHandle;
            g_samplerGPUHandles[i] = gpuHandle;
            cpuHandle.ptr += g_samplerDescriptorSize;
            gpuHandle.ptr += g_samplerDescriptorSize;
        }
    }

    if (g_rootSignature == nullptr)
    {
        D3D12_DESCRIPTOR_RANGE constantRange = {};
        constantRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        constantRange.NumDescriptors = 8;

        D3D12_DESCRIPTOR_RANGE resourceRange = {};
        resourceRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        resourceRange.NumDescriptors = 8;

        D3D12_DESCRIPTOR_RANGE samplerRange = {};
        samplerRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
        samplerRange.NumDescriptors = 8;

        D3D12_ROOT_PARAMETER parameters[6] = {};
        parameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        parameters[0].DescriptorTable.NumDescriptorRanges = 1;
        parameters[0].DescriptorTable.pDescriptorRanges = &constantRange;
        parameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
        parameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        parameters[1].DescriptorTable.NumDescriptorRanges = 1;
        parameters[1].DescriptorTable.pDescriptorRanges = &constantRange;
        parameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        parameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        parameters[2].DescriptorTable.NumDescriptorRanges = 1;
        parameters[2].DescriptorTable.pDescriptorRanges = &resourceRange;
        parameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
        parameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        parameters[3].DescriptorTable.NumDescriptorRanges = 1;
        parameters[3].DescriptorTable.pDescriptorRanges = &resourceRange;
        parameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        parameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        parameters[4].DescriptorTable.NumDescriptorRanges = 1;
        parameters[4].DescriptorTable.pDescriptorRanges = &samplerRange;
        parameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
        parameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        parameters[5].DescriptorTable.NumDescriptorRanges = 1;
        parameters[5].DescriptorTable.pDescriptorRanges = &samplerRange;
        parameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

        D3D12_ROOT_SIGNATURE_DESC desc = {};
        desc.NumParameters = xxCountOf(parameters);
        desc.pParameters = parameters;
        desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
                     D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
                     D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
                     D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

        ID3DBlob* blob = nullptr;
        HRESULT hResult = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, nullptr);
        if (hResult != S_OK)
            return 0;

        if (blob)
        {
            HRESULT hResult = d3dDevice->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&g_rootSignature));
            if (hResult != S_OK)
            {
                blob->Release();
                return 0;
            }
            blob->Release();
        }
    }

    signalFence(true);

    return reinterpret_cast<uint64_t>(d3dDevice);
}
//------------------------------------------------------------------------------
void xxDestroyDeviceD3D12(uint64_t device)
{
    if (g_fence)
    {
        for (int i = 0; i < NUM_BACK_BUFFERS; ++i)
            signalFence(true);

        g_fence->Release();
        g_fence = nullptr;
    }

    if (g_fenceEvent)
    {
        CloseHandle(g_fenceEvent);
        g_fenceEvent = nullptr;
    }

    if (g_shaderHeap)
    {
        g_shaderHeap->Release();
        g_shaderHeap = nullptr;
    }

    if (g_samplerHeap)
    {
        g_samplerHeap->Release();
        g_samplerHeap = nullptr;
    }

    if (g_rootSignature)
    {
        g_rootSignature->Release();
        g_rootSignature = nullptr;
    }

    if (g_commandQueue)
    {
        g_commandQueue->Release();
        g_commandQueue = nullptr;
    }

    if (g_dxgiFactory)
    {
        g_dxgiFactory->Release();
        g_dxgiFactory = nullptr;
    }

    ID3D12Device* d3dDevice = reinterpret_cast<ID3D12Device*>(device);
    if (d3dDevice == nullptr)
        return;

    d3dDevice->Release();
}
//------------------------------------------------------------------------------
void xxResetDeviceD3D12(uint64_t device)
{

}
//------------------------------------------------------------------------------
bool xxTestDeviceD3D12(uint64_t device)
{
    return true;
}
//------------------------------------------------------------------------------
const char* xxGetDeviceStringD3D12(uint64_t device)
{
    return "Direct3D 12.0";
}
//==============================================================================
//  Framebuffer
//==============================================================================
struct D3D12FRAMEBUFFER
{
    ID3D12Resource*             renderTargetResource;
    D3D12_CPU_DESCRIPTOR_HANDLE renderTargetHandle;
};
//==============================================================================
//  Swapchain
//==============================================================================
struct D3D12SWAPCHAIN : public D3D12FRAMEBUFFER
{
    IDXGISwapChain3*            dxgiSwapchain;
    ID3D12DescriptorHeap*       renderTargetHeap;
    ID3D12Resource*             renderTargetResources[NUM_BACK_BUFFERS];
    D3D12_CPU_DESCRIPTOR_HANDLE renderTargetHandles[NUM_BACK_BUFFERS];
    ID3D12CommandAllocator*     commandAllocators[NUM_BACK_BUFFERS];
    ID3D12GraphicsCommandList*  commandList;
};
//------------------------------------------------------------------------------
uint64_t xxCreateSwapchainD3D12(uint64_t device, void* view, unsigned int width, unsigned int height)
{
    ID3D12Device* d3dDevice = reinterpret_cast<ID3D12Device*>(device);
    if (d3dDevice == nullptr)
        return 0;
    D3D12SWAPCHAIN* d3dSwapchain = new D3D12SWAPCHAIN;
    if (d3dSwapchain == nullptr)
        return 0;

    if (g_dxgiFactory == nullptr)
    {
        PFN_CREATE_DXGI_FACTORY1 CreateDXGIFactory1;
        (void*&)CreateDXGIFactory1 = GetProcAddress(g_dxgiLibrary, "CreateDXGIFactory1");
        if (CreateDXGIFactory1 == nullptr)
        {
            delete d3dSwapchain;
            return 0;
        }

        HRESULT hResult = CreateDXGIFactory1(IID_PPV_ARGS(&g_dxgiFactory));
        if (hResult != S_OK)
        {
            delete d3dSwapchain;
            return 0;
        }
    }

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

    DXGI_SWAP_CHAIN_DESC1 desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = NUM_BACK_BUFFERS;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    IDXGISwapChain1* dxgiSwapchain1 = nullptr;
    if (dxgiSwapchain1 == nullptr)
    {
        HRESULT hResult = g_dxgiFactory->CreateSwapChainForHwnd(g_commandQueue, hWnd, &desc, nullptr, nullptr, &dxgiSwapchain1);
        if (hResult != S_OK)
        {
            delete d3dSwapchain;
            return 0;
        }
    }

    IDXGISwapChain3* dxgiSwapchain3 = nullptr;
    if (dxgiSwapchain3 == nullptr)
    {
        HRESULT hResult = dxgiSwapchain1->QueryInterface(IID_PPV_ARGS(&dxgiSwapchain3));
        if (hResult != S_OK)
        {
            dxgiSwapchain1->Release();
            delete d3dSwapchain;
            return 0;
        }
        dxgiSwapchain1->Release();
        dxgiSwapchain1 = nullptr;
    }
    d3dSwapchain->dxgiSwapchain = dxgiSwapchain3;

    ID3D12DescriptorHeap* renderTargetHeap = nullptr;
    if (renderTargetHeap == nullptr)
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        desc.NumDescriptors = NUM_BACK_BUFFERS;
        desc.NodeMask = 1;

        HRESULT hResult = d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&renderTargetHeap));
        if (hResult != S_OK)
        {
            delete d3dSwapchain;
            return 0;
        }
    }
    d3dSwapchain->renderTargetHeap = renderTargetHeap;

    D3D12_CPU_DESCRIPTOR_HANDLE renderTargetHandle = renderTargetHeap->GetCPUDescriptorHandleForHeapStart();
    for (int i = 0; i < NUM_BACK_BUFFERS; ++i)
    {
        d3dSwapchain->renderTargetHandles[i] = renderTargetHandle;
        renderTargetHandle.ptr += d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        ID3D12Resource* backBuffer = nullptr;
        if (backBuffer == nullptr)
        {
            HRESULT hResult = dxgiSwapchain3->GetBuffer(i, IID_PPV_ARGS(&backBuffer));
            if (hResult == S_OK)
            {
                d3dDevice->CreateRenderTargetView(backBuffer, nullptr, d3dSwapchain->renderTargetHandles[i]);
                d3dSwapchain->renderTargetResources[i] = backBuffer;
            }
        }

        ID3D12CommandAllocator* commandAllocator = nullptr;
        if (commandAllocator == nullptr)
        {
            HRESULT hResult = d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
            if (hResult == S_OK)
            {
                d3dSwapchain->commandAllocators[i] = commandAllocator;
            }
        }

        if (d3dSwapchain->renderTargetResources[i] == nullptr || d3dSwapchain->commandAllocators[i] == nullptr)
        {
            dxgiSwapchain3->Release();
            renderTargetHeap->Release();
            delete d3dSwapchain;
            return 0;
        }
    }

    int bufferIndex = dxgiSwapchain3->GetCurrentBackBufferIndex();
    ID3D12GraphicsCommandList* commandList = nullptr;
    HRESULT hResult = d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, d3dSwapchain->commandAllocators[bufferIndex], nullptr, IID_PPV_ARGS(&commandList));
    if (hResult == S_OK)
    {
        HRESULT hResult = commandList->Close();
        if (hResult == S_OK)
        {
            d3dSwapchain->commandList = commandList;
        }
    }

    return reinterpret_cast<uint64_t>(d3dSwapchain);
}
//------------------------------------------------------------------------------
void xxDestroySwapchainD3D12(uint64_t swapchain)
{
    D3D12SWAPCHAIN* d3dSwapchain = reinterpret_cast<D3D12SWAPCHAIN*>(swapchain);
    if (d3dSwapchain == nullptr)
        return;

    for (int i = 0; i < NUM_BACK_BUFFERS; ++i)
        signalFence(true);

    if (d3dSwapchain->dxgiSwapchain)
        d3dSwapchain->dxgiSwapchain->Release();
    if (d3dSwapchain->commandList)
        d3dSwapchain->commandList->Release();
    for (int i = 0; i < NUM_BACK_BUFFERS; ++i)
    {
        if (d3dSwapchain->renderTargetResources[i])
            d3dSwapchain->renderTargetResources[i]->Release();
        if (d3dSwapchain->commandAllocators[i])
            d3dSwapchain->commandAllocators[i]->Release();
    }
    if (d3dSwapchain->renderTargetHeap)
        d3dSwapchain->renderTargetHeap->Release();
    delete d3dSwapchain;
}
//------------------------------------------------------------------------------
void xxPresentSwapchainD3D12(uint64_t swapchain, void* view)
{
    D3D12SWAPCHAIN* d3dSwapchain = reinterpret_cast<D3D12SWAPCHAIN*>(swapchain);
    if (d3dSwapchain == nullptr)
        return;

    int bufferIndex = d3dSwapchain->dxgiSwapchain->GetCurrentBackBufferIndex();
    g_fenceValues[bufferIndex] = g_fenceValue;
    signalFence(false);

    d3dSwapchain->dxgiSwapchain->Present(0, 0);
}
//------------------------------------------------------------------------------
uint64_t xxGetFramebufferD3D12(uint64_t device, uint64_t swapchain)
{
    D3D12SWAPCHAIN* d3dSwapchain = reinterpret_cast<D3D12SWAPCHAIN*>(swapchain);
    if (d3dSwapchain == nullptr)
        return 0;

    int bufferIndex = d3dSwapchain->dxgiSwapchain->GetCurrentBackBufferIndex();
    d3dSwapchain->renderTargetResource = d3dSwapchain->renderTargetResources[bufferIndex];
    d3dSwapchain->renderTargetHandle = d3dSwapchain->renderTargetHandles[bufferIndex];

    return swapchain;
}
//==============================================================================
//  Command Buffer
//==============================================================================
uint64_t xxGetCommandBufferD3D12(uint64_t device, uint64_t swapchain)
{
    D3D12SWAPCHAIN* d3dSwapchain = reinterpret_cast<D3D12SWAPCHAIN*>(swapchain);
    if (d3dSwapchain == nullptr)
        return 0;

    int bufferIndex = d3dSwapchain->dxgiSwapchain->GetCurrentBackBufferIndex();
    if (g_fence->GetCompletedValue() < g_fenceValues[bufferIndex])
    {
        g_fence->SetEventOnCompletion(g_fenceValues[bufferIndex], g_fenceEvent);
        WaitForSingleObjectEx(g_fenceEvent, 1000, FALSE);
    }

    ID3D12CommandAllocator* commandAllocator = d3dSwapchain->commandAllocators[bufferIndex];
    ID3D12GraphicsCommandList* commandList = d3dSwapchain->commandList;
    commandAllocator->Reset();
    commandList->Reset(commandAllocator, nullptr);

    ID3D12DescriptorHeap* heaps[] = { g_shaderHeap, g_samplerHeap };
    commandList->SetDescriptorHeaps(xxCountOf(heaps), heaps);

    return reinterpret_cast<uint64_t>(commandList);
}
//------------------------------------------------------------------------------
bool xxBeginCommandBufferD3D12(uint64_t commandBuffer)
{
    ID3D12GraphicsCommandList* d3dCommandList = reinterpret_cast<ID3D12GraphicsCommandList*>(commandBuffer);
    if (d3dCommandList == nullptr)
        return false;

    return true;
}
//------------------------------------------------------------------------------
void xxEndCommandBufferD3D12(uint64_t commandBuffer)
{
    ID3D12GraphicsCommandList* d3dCommandList = reinterpret_cast<ID3D12GraphicsCommandList*>(commandBuffer);
    if (d3dCommandList == nullptr)
        return;

    d3dCommandList->Close();
}
//------------------------------------------------------------------------------
void xxSubmitCommandBufferD3D12(uint64_t commandBuffer)
{
    ID3D12CommandList* d3dCommandList = reinterpret_cast<ID3D12CommandList*>(commandBuffer);
    if (d3dCommandList == nullptr)
        return;

    g_commandQueue->ExecuteCommandLists(1, &d3dCommandList);
}
//==============================================================================
//  Render Pass
//==============================================================================
struct D3D12RENDERPASS
{
    float   color[4];
    float   depth;
    uint8_t stencil;
};
//------------------------------------------------------------------------------
uint64_t xxCreateRenderPassD3D12(uint64_t device, float r, float g, float b, float a, float depth, unsigned char stencil)
{
    D3D12RENDERPASS* d3dRenderPass = new D3D12RENDERPASS;

    d3dRenderPass->color[0] = r;
    d3dRenderPass->color[1] = g;
    d3dRenderPass->color[2] = b;
    d3dRenderPass->color[3] = a;
    d3dRenderPass->depth = depth;
    d3dRenderPass->stencil = stencil;

    return reinterpret_cast<uint64_t>(d3dRenderPass);
}
//------------------------------------------------------------------------------
void xxDestroyRenderPassD3D12(uint64_t renderPass)
{
    D3D12RENDERPASS* d3dRenderPass = reinterpret_cast<D3D12RENDERPASS*>(renderPass);

    delete d3dRenderPass;
}
//------------------------------------------------------------------------------
bool xxBeginRenderPassD3D12(uint64_t commandBuffer, uint64_t framebuffer, uint64_t renderPass)
{
    ID3D12GraphicsCommandList* d3dCommandList = reinterpret_cast<ID3D12GraphicsCommandList*>(commandBuffer);
    if (d3dCommandList == nullptr)
        return false;
    D3D12FRAMEBUFFER* d3dFramebuffer = reinterpret_cast<D3D12FRAMEBUFFER*>(framebuffer);
    if (d3dFramebuffer == nullptr)
        return false;
    D3D12RENDERPASS* d3dRenderPass = reinterpret_cast<D3D12RENDERPASS*>(renderPass);
    if (d3dRenderPass == nullptr)
        return false;

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = d3dFramebuffer->renderTargetResource;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

    d3dCommandList->ResourceBarrier(1, &barrier);
    d3dCommandList->ClearRenderTargetView(d3dFramebuffer->renderTargetHandle, d3dRenderPass->color, 0, nullptr);
    d3dCommandList->OMSetRenderTargets(1, &d3dFramebuffer->renderTargetHandle, FALSE, nullptr);

    return true;
}
//------------------------------------------------------------------------------
void xxEndRenderPassD3D12(uint64_t commandBuffer, uint64_t framebuffer, uint64_t renderPass)
{
    ID3D12GraphicsCommandList* d3dCommandList = reinterpret_cast<ID3D12GraphicsCommandList*>(commandBuffer);
    if (d3dCommandList == nullptr)
        return;
    D3D12FRAMEBUFFER* d3dFramebuffer = reinterpret_cast<D3D12FRAMEBUFFER*>(framebuffer);
    if (d3dFramebuffer == nullptr)
        return;

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = d3dFramebuffer->renderTargetResource;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

    d3dCommandList->ResourceBarrier(1, &barrier);
}
//==============================================================================
//  Buffer
//==============================================================================
struct D3D12RESOURCE
{
    ID3D12Resource*             resource;
    D3D12_CPU_DESCRIPTOR_HANDLE resourceCPUHandle;
    D3D12_GPU_DESCRIPTOR_HANDLE resourceGPUHandle;
    UINT                        size;
    void*                       cpuAddress;
};
//------------------------------------------------------------------------------
uint64_t xxCreateConstantBufferD3D12(uint64_t device, unsigned int size)
{
    ID3D12Device* d3dDevice = reinterpret_cast<ID3D12Device*>(device);
    if (d3dDevice == nullptr)
        return 0;
    D3D12RESOURCE* d3dResource = new D3D12RESOURCE;
    if (d3dResource == nullptr)
        return 0;

    D3D12_HEAP_PROPERTIES properties = {};
    properties.Type = D3D12_HEAP_TYPE_UPLOAD;
    properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    D3D12_RESOURCE_DESC desc = {};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Width = (size + 255) & ~255;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.SampleDesc.Count = 1;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    ID3D12Resource* resource = nullptr;
    HRESULT hResult = d3dDevice->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource));
    if (hResult != S_OK)
        return 0;

    D3D12_CONSTANT_BUFFER_VIEW_DESC viewDesc = {};
    viewDesc.BufferLocation = resource->GetGPUVirtualAddress();
    viewDesc.SizeInBytes = (size + 255) & ~255;

    d3dResource->resource = resource;
    d3dResource->resourceCPUHandle = {};
    d3dResource->resourceGPUHandle = {};
    createShaderHeap(d3dResource->resourceCPUHandle, d3dResource->resourceGPUHandle);
    d3dDevice->CreateConstantBufferView(&viewDesc, d3dResource->resourceCPUHandle);
    d3dResource->size = size;
    d3dResource->cpuAddress = nullptr;
#if PERSISTENT_BUFFER
    d3dResource->cpuAddress = xxMapBuffer(device, reinterpret_cast<uint64_t>(d3dResource));
#endif

    return reinterpret_cast<uint64_t>(d3dResource);
}
//------------------------------------------------------------------------------
uint64_t xxCreateIndexBufferD3D12(uint64_t device, unsigned int size)
{
    ID3D12Device* d3dDevice = reinterpret_cast<ID3D12Device*>(device);
    if (d3dDevice == nullptr)
        return 0;
    D3D12RESOURCE* d3dResource = new D3D12RESOURCE;
    if (d3dResource == nullptr)
        return 0;

    D3D12_HEAP_PROPERTIES properties = {};
    properties.Type = D3D12_HEAP_TYPE_UPLOAD;
    properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    D3D12_RESOURCE_DESC desc = {};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Width = size;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.SampleDesc.Count = 1;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    ID3D12Resource* resource = nullptr;
    HRESULT hResult = d3dDevice->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource));
    if (hResult != S_OK)
        return 0;

    d3dResource->resource = resource;
    d3dResource->resourceCPUHandle = {};
    d3dResource->resourceGPUHandle = {};
    d3dResource->size = size;
    d3dResource->cpuAddress = nullptr;
#if PERSISTENT_BUFFER
    d3dResource->cpuAddress = xxMapBuffer(device, reinterpret_cast<uint64_t>(d3dResource));
#endif

    return reinterpret_cast<uint64_t>(d3dResource);
}
//------------------------------------------------------------------------------
uint64_t xxCreateVertexBufferD3D12(uint64_t device, unsigned int size)
{
    ID3D12Device* d3dDevice = reinterpret_cast<ID3D12Device*>(device);
    if (d3dDevice == nullptr)
        return 0;
    D3D12RESOURCE* d3dResource = new D3D12RESOURCE;
    if (d3dResource == nullptr)
        return 0;

    D3D12_HEAP_PROPERTIES properties = {};
    properties.Type = D3D12_HEAP_TYPE_UPLOAD;
    properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    D3D12_RESOURCE_DESC desc = {};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Width = size;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.SampleDesc.Count = 1;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    ID3D12Resource* resource = nullptr;
    HRESULT hResult = d3dDevice->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource));
    if (hResult != S_OK)
        return 0;

    d3dResource->resource = resource;
    d3dResource->resourceCPUHandle = {};
    d3dResource->resourceGPUHandle = {};
    d3dResource->size = size;
    d3dResource->cpuAddress = nullptr;
#if PERSISTENT_BUFFER
    d3dResource->cpuAddress = xxMapBuffer(device, reinterpret_cast<uint64_t>(d3dResource));
#endif

    return reinterpret_cast<uint64_t>(d3dResource);
}
//------------------------------------------------------------------------------
void xxDestroyBufferD3D12(uint64_t buffer)
{
    D3D12RESOURCE* d3dResource = reinterpret_cast<D3D12RESOURCE*>(buffer);
    if (d3dResource == nullptr)
        return;

    for (int i = 0; i < NUM_BACK_BUFFERS; ++i)
        signalFence(true);

    if (d3dResource->resource)
    {
        if (d3dResource->cpuAddress)
        {
            D3D12_RANGE range = { 0, d3dResource->size };
            d3dResource->resource->Unmap(0, &range);
        }
        d3dResource->resource->Release();
    }
    destroyShaderHeap(d3dResource->resourceCPUHandle, d3dResource->resourceGPUHandle);
    delete d3dResource;
}
//------------------------------------------------------------------------------
void* xxMapBufferD3D12(uint64_t device, uint64_t buffer)
{
    D3D12RESOURCE* d3dResource = reinterpret_cast<D3D12RESOURCE*>(buffer);
    if (d3dResource == nullptr)
        return nullptr;
    if (d3dResource->cpuAddress)
        return d3dResource->cpuAddress;

    D3D12_RANGE range = { 0, d3dResource->size };
    void* ptr = nullptr;
    HRESULT hResult = d3dResource->resource->Map(0, &range, &ptr);
    if (hResult != S_OK)
        return nullptr;

    return ptr;
}
//------------------------------------------------------------------------------
void xxUnmapBufferD3D12(uint64_t device, uint64_t buffer)
{
    D3D12RESOURCE* d3dResource = reinterpret_cast<D3D12RESOURCE*>(buffer);
    if (d3dResource == nullptr)
        return;
    if (d3dResource->cpuAddress)
        return;

    D3D12_RANGE range = { 0, d3dResource->size };
    d3dResource->resource->Unmap(0, &range);
}
//==============================================================================
//  Texture
//==============================================================================
struct D3D12TEXTURE
{
    ID3D12Resource*             texture;
    D3D12_CPU_DESCRIPTOR_HANDLE textureCPUHandle;
    D3D12_GPU_DESCRIPTOR_HANDLE textureGPUHandle;
    UINT                        width;
    UINT                        height;
    UINT                        depth;
    ID3D12Resource*             upload;
    UINT                        uploadPitch;
    UINT                        uploadSize;
};
//------------------------------------------------------------------------------
uint64_t xxCreateTextureD3D12(uint64_t device, int format, unsigned int width, unsigned int height, unsigned int depth, unsigned int mipmap, unsigned int array)
{
    ID3D12Device* d3dDevice = reinterpret_cast<ID3D12Device*>(device);
    if (d3dDevice == nullptr)
        return 0;

    if (width == 0 || height == 0 || depth == 0 || mipmap == 0 || array == 0)
        return 0;

    D3D12TEXTURE* d3dTexture = new D3D12TEXTURE;
    if (d3dTexture == nullptr)
        return 0;

    D3D12_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
    ID3D12Resource* d3dResource = nullptr;

    D3D12_HEAP_PROPERTIES properties = {};
    properties.Type = D3D12_HEAP_TYPE_DEFAULT;
    properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    if (depth == 1 && array == 1)
    {
        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        desc.Width = width;
        desc.Height = height;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        desc.SampleDesc.Count = 1;

        ID3D12Resource* texture = nullptr;
        HRESULT hResult = d3dDevice->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&texture));
        if (hResult != S_OK)
        {
            delete d3dTexture;
            return 0;
        }
        d3dResource = texture;

        viewDesc.Format = desc.Format;
        viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        viewDesc.Texture2D.MipLevels = mipmap;
        viewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    }

    if (d3dResource == nullptr)
    {
        delete d3dTexture;
        return 0;
    }

    d3dTexture->texture = d3dResource;
    createShaderHeap(d3dTexture->textureCPUHandle, d3dTexture->textureGPUHandle);
    d3dDevice->CreateShaderResourceView(d3dResource, &viewDesc, d3dTexture->textureCPUHandle);
    d3dTexture->width = width;
    d3dTexture->height = height;
    d3dTexture->depth = depth;
    d3dTexture->upload = nullptr;
    d3dTexture->uploadPitch = (width * 4 + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u) & ~(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u);
    d3dTexture->uploadSize = height * d3dTexture->uploadPitch;

    return reinterpret_cast<uint64_t>(d3dTexture);
}
//------------------------------------------------------------------------------
void xxDestroyTextureD3D12(uint64_t texture)
{
    D3D12TEXTURE* d3dTexture = reinterpret_cast<D3D12TEXTURE*>(texture);
    if (d3dTexture == nullptr)
        return;

    for (int i = 0; i < NUM_BACK_BUFFERS; ++i)
        signalFence(true);

    if (d3dTexture->texture)
        d3dTexture->texture->Release();
    if (d3dTexture->textureCPUHandle.ptr && d3dTexture->textureGPUHandle.ptr)
        destroyShaderHeap(d3dTexture->textureCPUHandle, d3dTexture->textureGPUHandle);
    if (d3dTexture->upload)
        d3dTexture->upload->Release();
    delete d3dTexture;
}
//------------------------------------------------------------------------------
void* xxMapTextureD3D12(uint64_t device, uint64_t texture, unsigned int& stride, unsigned int level, unsigned int array, unsigned int mipmap)
{
    ID3D12Device* d3dDevice = reinterpret_cast<ID3D12Device*>(device);
    if (d3dDevice == nullptr)
        return nullptr;
    D3D12TEXTURE* d3dTexture = reinterpret_cast<D3D12TEXTURE*>(texture);
    if (d3dTexture == nullptr)
        return nullptr;

    if (d3dTexture->upload == nullptr)
    {
        D3D12_HEAP_PROPERTIES properties = {};
        properties.Type = D3D12_HEAP_TYPE_UPLOAD;
        properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Width = d3dTexture->uploadSize;
        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count = 1;
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        HRESULT hResult = d3dDevice->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&d3dTexture->upload));
        if (hResult != S_OK)
            return nullptr;
    }

    void* ptr = nullptr;
    HRESULT hResult = d3dTexture->upload->Map(0, nullptr, &ptr);
    if (hResult != S_OK)
        return nullptr;

    stride = d3dTexture->uploadPitch;

    return ptr;
}
//------------------------------------------------------------------------------
void xxUnmapTextureD3D12(uint64_t device, uint64_t texture, unsigned int level, unsigned int array, unsigned int mipmap)
{
    ID3D12Device* d3dDevice = reinterpret_cast<ID3D12Device*>(device);
    if (d3dDevice == nullptr)
        return;
    D3D12TEXTURE* d3dTexture = reinterpret_cast<D3D12TEXTURE*>(texture);
    if (d3dTexture == nullptr)
        return;
    if (d3dTexture->upload == nullptr)
        return;

    d3dTexture->upload->Unmap(0, nullptr);

    ID3D12CommandAllocator* commandAllocator = nullptr;
    HRESULT hResult = d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
    if (hResult == S_OK)
    {
        ID3D12GraphicsCommandList* commandList = nullptr;
        HRESULT hResult = d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, IID_PPV_ARGS(&commandList));
        if (hResult == S_OK)
        {
            D3D12_TEXTURE_COPY_LOCATION textureLocation = {};
            D3D12_TEXTURE_COPY_LOCATION uploadLocation = {};
            textureLocation.pResource = d3dTexture->texture;
            textureLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
            uploadLocation.pResource = d3dTexture->upload;
            uploadLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
            uploadLocation.PlacedFootprint.Footprint.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            uploadLocation.PlacedFootprint.Footprint.Width = d3dTexture->width;
            uploadLocation.PlacedFootprint.Footprint.Height = d3dTexture->height;
            uploadLocation.PlacedFootprint.Footprint.Depth = d3dTexture->depth;
            uploadLocation.PlacedFootprint.Footprint.RowPitch = d3dTexture->uploadPitch;
            commandList->CopyTextureRegion(&textureLocation, 0, 0, 0, &uploadLocation, nullptr);

            D3D12_RESOURCE_BARRIER barrier = {};
            barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            barrier.Transition.pResource = d3dTexture->texture;
            barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
            barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
            commandList->ResourceBarrier(1, &barrier);

            commandList->Close();
            g_commandQueue->ExecuteCommandLists(1, (ID3D12CommandList**)&commandList);
            signalFence(true);

            commandList->Release();
        }
        commandAllocator->Release();
    }
    d3dTexture->upload->Release();
    d3dTexture->upload = nullptr;
}
//==============================================================================
//  Sampler
//==============================================================================
uint64_t xxCreateSamplerD3D12(uint64_t device, bool clampU, bool clampV, bool clampW, bool linearMag, bool linearMin, bool linearMip, int anisotropy)
{
    ID3D12Device* d3dDevice = reinterpret_cast<ID3D12Device*>(device);
    if (d3dDevice == nullptr)
        return 0;

    D3D12_SAMPLER_DESC desc = {};
    desc.Filter = D3D12_ENCODE_BASIC_FILTER(linearMin ? D3D12_FILTER_TYPE_LINEAR : D3D12_FILTER_TYPE_POINT,
                                            linearMag ? D3D12_FILTER_TYPE_LINEAR : D3D12_FILTER_TYPE_POINT,
                                            linearMip ? D3D12_FILTER_TYPE_LINEAR : D3D12_FILTER_TYPE_POINT, D3D12_FILTER_REDUCTION_TYPE_STANDARD);
    desc.AddressU = clampU ? D3D12_TEXTURE_ADDRESS_MODE_CLAMP : D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    desc.AddressV = clampV ? D3D12_TEXTURE_ADDRESS_MODE_CLAMP : D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    desc.AddressW = clampW ? D3D12_TEXTURE_ADDRESS_MODE_CLAMP : D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    desc.MaxAnisotropy = anisotropy;
    desc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    if (anisotropy > 1)
    {
        desc.Filter = D3D12_ENCODE_ANISOTROPIC_FILTER(D3D12_FILTER_REDUCTION_TYPE_STANDARD);
    }

    D3D12_CPU_DESCRIPTOR_HANDLE samplerCPUHandle;
    D3D12_GPU_DESCRIPTOR_HANDLE samplerGPUHandle;
    createSamplerHeap(samplerCPUHandle, samplerGPUHandle);

    d3dDevice->CreateSampler(&desc, samplerCPUHandle);

    return static_cast<uint64_t>(samplerGPUHandle.ptr);
}
//------------------------------------------------------------------------------
void xxDestroySamplerD3D12(uint64_t sampler)
{
    D3D12_CPU_DESCRIPTOR_HANDLE samplerCPUHandle = { 0 };
    D3D12_GPU_DESCRIPTOR_HANDLE samplerGPUHandle = { sampler };
    if (samplerGPUHandle.ptr == 0)
        return;

    destroySamplerHeap(samplerCPUHandle, samplerGPUHandle);
}
//==============================================================================
//  Vertex Attribute
//==============================================================================
struct D3D12VERTEXATTRIBUTE
{
    D3D12_INPUT_ELEMENT_DESC    inputElements[16];
    UINT                        inputElementCount;
    int                         stride;
};
//------------------------------------------------------------------------------
uint64_t xxCreateVertexAttributeD3D12(uint64_t device, int count, ...)
{
    D3D12VERTEXATTRIBUTE* d3dVertexAttribute = new D3D12VERTEXATTRIBUTE;
    if (d3dVertexAttribute == nullptr)
        return 0;

    D3D12_INPUT_ELEMENT_DESC* inputElements = d3dVertexAttribute->inputElements;
    BYTE xyzIndex = 0;
    BYTE textureIndex = 0;
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

        D3D12_INPUT_ELEMENT_DESC& inputElement = inputElements[i];
        inputElement.SemanticName;
        inputElement.SemanticIndex;
        inputElement.Format;
        inputElement.InputSlot = stream;
        inputElement.AlignedByteOffset = offset;
        inputElement.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        inputElement.InstanceDataStepRate = 0;

        if (element == 3 && size == sizeof(float) * 3)
        {
            inputElement.Format = DXGI_FORMAT_R32G32B32_FLOAT;
            switch (xyzIndex)
            {
            case 0:
                inputElement.SemanticName = "POSITION";
                inputElement.SemanticIndex = 0;
                break;
            case 1:
                inputElement.SemanticName = "NORMAL";
                inputElement.SemanticIndex = 0;
                break;
            case 2:
                inputElement.SemanticName = "BINORMAL";
                inputElement.SemanticIndex = 0;
                break;
            case 3:
                inputElement.SemanticName = "TANGENT";
                inputElement.SemanticIndex = 0;
                break;
            default:
                break;
            }
            xyzIndex++;
            continue;
        }

        if (element == 4 && size == sizeof(char) * 4)
        {
            inputElement.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            inputElement.SemanticName = "COLOR";
            inputElement.SemanticIndex = 0;
            continue;
        }

        if (element == 2 && size == sizeof(float) * 2)
        {
            inputElement.Format = DXGI_FORMAT_R32G32_FLOAT;
            inputElement.SemanticName = "TEXCOORD";
            inputElement.SemanticIndex = textureIndex;
            textureIndex++;
            continue;
        }
    }
    va_end(args);

    d3dVertexAttribute->inputElementCount = count;
    d3dVertexAttribute->stride = stride;

    return reinterpret_cast<uint64_t>(d3dVertexAttribute);
}
//------------------------------------------------------------------------------
void xxDestroyVertexAttributeD3D12(uint64_t vertexAttribute)
{
    D3D12VERTEXATTRIBUTE* d3dVertexAttribute = reinterpret_cast<D3D12VERTEXATTRIBUTE*>(vertexAttribute);

    delete d3dVertexAttribute;
}
//==============================================================================
//  Shader
//==============================================================================
uint64_t xxCreateVertexShaderD3D12(uint64_t device, const char* shader, uint64_t vertexAttribute)
{
    ID3D12Device* d3dDevice = reinterpret_cast<ID3D12Device*>(device);
    if (d3dDevice == nullptr)
        return 0;

    if (strcmp(shader, "default") == 0)
    {
        return reinterpret_cast<uint64_t>(vertexShaderCode40);
    }

    return 0;
}
//------------------------------------------------------------------------------
uint64_t xxCreateFragmentShaderD3D12(uint64_t device, const char* shader)
{
    ID3D12Device* d3dDevice = reinterpret_cast<ID3D12Device*>(device);
    if (d3dDevice == nullptr)
        return 0;

    if (strcmp(shader, "default") == 0)
    {
        return reinterpret_cast<uint64_t>(pixelShaderCode40);
    }

    return 0;
}
//------------------------------------------------------------------------------
void xxDestroyShaderD3D12(uint64_t device, uint64_t shader)
{

}
//==============================================================================
//  Pipeline
//==============================================================================
uint64_t xxCreateBlendStateD3D12(uint64_t device, bool blending)
{
    D3D12_BLEND_DESC* d3dDesc = new D3D12_BLEND_DESC;
    if (d3dDesc == nullptr)
        return 0;
    memset(d3dDesc, 0, sizeof(D3D12_BLEND_DESC));

    D3D12_BLEND_DESC& desc = (*d3dDesc);
    desc.RenderTarget[0].BlendEnable = blending;
    desc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    desc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    desc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    desc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
    desc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    desc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    return reinterpret_cast<uint64_t>(d3dDesc);
}
//------------------------------------------------------------------------------
uint64_t xxCreateDepthStencilStateD3D12(uint64_t device, bool depthTest, bool depthWrite)
{
    D3D12_DEPTH_STENCIL_DESC* d3dDesc = new D3D12_DEPTH_STENCIL_DESC;
    if (d3dDesc == nullptr)
        return 0;
    memset(d3dDesc, 0, sizeof(D3D12_DEPTH_STENCIL_DESC));

    D3D12_DEPTH_STENCIL_DESC& desc = (*d3dDesc);
    desc.DepthEnable = depthTest;
    desc.DepthWriteMask = depthWrite ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
    desc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    desc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
    desc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
    desc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
    desc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    desc.BackFace = desc.FrontFace;

    return reinterpret_cast<uint64_t>(d3dDesc);
}
//------------------------------------------------------------------------------
uint64_t xxCreateRasterizerStateD3D12(uint64_t device, bool cull, bool scissor)
{
    D3D12_RASTERIZER_DESC* d3dDesc = new D3D12_RASTERIZER_DESC;
    if (d3dDesc == nullptr)
        return 0;
    memset(d3dDesc, 0, sizeof(D3D12_RASTERIZER_DESC));

    D3D12_RASTERIZER_DESC& desc = (*d3dDesc);
    desc.FillMode = D3D12_FILL_MODE_SOLID;
    desc.CullMode = D3D12_CULL_MODE_NONE;
    desc.DepthClipEnable = true;

    return reinterpret_cast<uint64_t>(d3dDesc);
}
//------------------------------------------------------------------------------
uint64_t xxCreatePipelineD3D12(uint64_t device, uint64_t blendState, uint64_t depthStencilState, uint64_t rasterizerState, uint64_t vertexAttribute, uint64_t vertexShader, uint64_t fragmentShader)
{
    ID3D12Device* d3dDevice = reinterpret_cast<ID3D12Device*>(device);
    if (d3dDevice == nullptr)
        return 0;
    D3D12_BLEND_DESC* d3dBlendState = reinterpret_cast<D3D12_BLEND_DESC*>(blendState);
    if (d3dBlendState == nullptr)
        return 0;
    D3D12_DEPTH_STENCIL_DESC* d3dDepthStencilState = reinterpret_cast<D3D12_DEPTH_STENCIL_DESC*>(depthStencilState);
    if (d3dDepthStencilState == nullptr)
        return 0;
    D3D12_RASTERIZER_DESC* d3dRasterizerState = reinterpret_cast<D3D12_RASTERIZER_DESC*>(rasterizerState);
    if (d3dRasterizerState == nullptr)
        return 0;
    D3D12VERTEXATTRIBUTE* d3dVertexAttribute = reinterpret_cast<D3D12VERTEXATTRIBUTE*>(vertexAttribute);
    if (d3dVertexAttribute == nullptr)
        return 0;
    DWORD* d3dVertexShader = reinterpret_cast<DWORD*>(vertexShader);
    if (d3dVertexShader == nullptr)
        return 0;
    DWORD* d3dPixelShader = reinterpret_cast<DWORD*>(fragmentShader);
    if (d3dPixelShader == nullptr)
        return 0;

    D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
    desc.pRootSignature = g_rootSignature;
    desc.VS = { d3dVertexShader, d3dVertexShader[6] };
    desc.PS = { d3dPixelShader, d3dPixelShader[6] };
    desc.BlendState = (*d3dBlendState);
    desc.SampleMask = UINT_MAX;
    desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    desc.RasterizerState = (*d3dRasterizerState);
    desc.DepthStencilState = (*d3dDepthStencilState);
    desc.InputLayout = { d3dVertexAttribute->inputElements, d3dVertexAttribute->inputElementCount };
    desc.NumRenderTargets = 1;
    desc.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.NodeMask = 1;

    ID3D12PipelineState* pipelineState = nullptr;
    HRESULT hResult = d3dDevice->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipelineState));
    if (hResult != S_OK)
        return 0;

    return reinterpret_cast<uint64_t>(pipelineState);
}
//------------------------------------------------------------------------------
void xxDestroyBlendStateD3D12(uint64_t blendState)
{
    D3D12_BLEND_DESC* d3dBlendState = reinterpret_cast<D3D12_BLEND_DESC*>(blendState);

    delete d3dBlendState;
}
//------------------------------------------------------------------------------
void xxDestroyDepthStencilStateD3D12(uint64_t depthStencilState)
{
    D3D12_DEPTH_STENCIL_DESC* d3dDepthStencilState = reinterpret_cast<D3D12_DEPTH_STENCIL_DESC*>(depthStencilState);

    delete d3dDepthStencilState;
}
//------------------------------------------------------------------------------
void xxDestroyRasterizerStateD3D12(uint64_t rasterizerState)
{
    D3D12_RASTERIZER_DESC* d3dRasterizerState = reinterpret_cast<D3D12_RASTERIZER_DESC*>(rasterizerState);

    delete d3dRasterizerState;
}
//------------------------------------------------------------------------------
void xxDestroyPipelineD3D12(uint64_t pipeline)
{
    ID3D12PipelineState* d3dPipelineState = reinterpret_cast<ID3D12PipelineState*>(pipeline);
    if (d3dPipelineState == nullptr)
        return;

    d3dPipelineState->Release();
}
//==============================================================================
//  Command
//==============================================================================
void xxSetViewportD3D12(uint64_t commandBuffer, int x, int y, int width, int height, float minZ, float maxZ)
{
    ID3D12GraphicsCommandList* d3dCommandList = reinterpret_cast<ID3D12GraphicsCommandList*>(commandBuffer);

    D3D12_VIEWPORT vp;
    vp.TopLeftX = (FLOAT)x;
    vp.TopLeftY = (FLOAT)y;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = minZ;
    vp.MaxDepth = maxZ;
    d3dCommandList->RSSetViewports(1, &vp);
}
//------------------------------------------------------------------------------
void xxSetScissorD3D12(uint64_t commandBuffer, int x, int y, int width, int height)
{
    ID3D12GraphicsCommandList* d3dCommandList = reinterpret_cast<ID3D12GraphicsCommandList*>(commandBuffer);

    D3D12_RECT rect;
    rect.left = x;
    rect.top = y;
    rect.right = x + width;
    rect.bottom = y + height;
    d3dCommandList->RSSetScissorRects(1, &rect);
}
//------------------------------------------------------------------------------
void xxSetPipelineD3D12(uint64_t commandBuffer, uint64_t pipeline)
{
    ID3D12GraphicsCommandList* d3dCommandList = reinterpret_cast<ID3D12GraphicsCommandList*>(commandBuffer);
    ID3D12PipelineState* d3dPipelineState = reinterpret_cast<ID3D12PipelineState*>(pipeline);

    d3dCommandList->SetGraphicsRootSignature(g_rootSignature);
    d3dCommandList->SetPipelineState(d3dPipelineState);
    d3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
//------------------------------------------------------------------------------
void xxSetIndexBufferD3D12(uint64_t commandBuffer, uint64_t buffer)
{
    ID3D12GraphicsCommandList* d3dCommandList = reinterpret_cast<ID3D12GraphicsCommandList*>(commandBuffer);
    D3D12RESOURCE* d3dBuffer = reinterpret_cast<D3D12RESOURCE*>(buffer);

    D3D12_INDEX_BUFFER_VIEW view = {};
    view.BufferLocation = d3dBuffer->resource->GetGPUVirtualAddress();
    view.SizeInBytes = d3dBuffer->size;
    view.Format = DXGI_FORMAT_R32_UINT;
    d3dCommandList->IASetIndexBuffer(&view);
}
//------------------------------------------------------------------------------
void xxSetVertexBuffersD3D12(uint64_t commandBuffer, int count, const uint64_t* buffers, uint64_t vertexAttribute)
{
    ID3D12GraphicsCommandList* d3dCommandList = reinterpret_cast<ID3D12GraphicsCommandList*>(commandBuffer);
    D3D12VERTEXATTRIBUTE* d3dVertexAttribute = reinterpret_cast<D3D12VERTEXATTRIBUTE*>(vertexAttribute);
    D3D12_VERTEX_BUFFER_VIEW views[16];

    for (int i = 0; i < count; ++i)
    {
        D3D12RESOURCE* d3dBuffer = reinterpret_cast<D3D12RESOURCE*>(buffers[i]);
        views[i].BufferLocation = d3dBuffer->resource->GetGPUVirtualAddress();
        views[i].SizeInBytes = d3dBuffer->size;
        views[i].StrideInBytes = d3dVertexAttribute->stride;
    }

    d3dCommandList->IASetVertexBuffers(0, count, views);
}
//------------------------------------------------------------------------------
void xxSetVertexTexturesD3D12(uint64_t commandBuffer, int count, const uint64_t* textures)
{
    ID3D12GraphicsCommandList* d3dCommandList = reinterpret_cast<ID3D12GraphicsCommandList*>(commandBuffer);

    for (int i = 0; i < count; ++i)
    {
        D3D12TEXTURE* d3dTexture = reinterpret_cast<D3D12TEXTURE*>(textures[i]);
        d3dCommandList->SetGraphicsRootDescriptorTable(2, d3dTexture->textureGPUHandle);
    }
}
//------------------------------------------------------------------------------
void xxSetFragmentTexturesD3D12(uint64_t commandBuffer, int count, const uint64_t* textures)
{
    ID3D12GraphicsCommandList* d3dCommandList = reinterpret_cast<ID3D12GraphicsCommandList*>(commandBuffer);

    for (int i = 0; i < count; ++i)
    {
        D3D12TEXTURE* d3dTexture = reinterpret_cast<D3D12TEXTURE*>(textures[i]);
        d3dCommandList->SetGraphicsRootDescriptorTable(3, d3dTexture->textureGPUHandle);
    }
}
//------------------------------------------------------------------------------
void xxSetVertexSamplersD3D12(uint64_t commandBuffer, int count, const uint64_t* samplers)
{
    ID3D12GraphicsCommandList* d3dCommandList = reinterpret_cast<ID3D12GraphicsCommandList*>(commandBuffer);

    for (int i = 0; i < count; ++i)
    {
        D3D12_GPU_DESCRIPTOR_HANDLE d3dHandle = { samplers[i] };
        d3dCommandList->SetGraphicsRootDescriptorTable(4, d3dHandle);
    }
}
//------------------------------------------------------------------------------
void xxSetFragmentSamplersD3D12(uint64_t commandBuffer, int count, const uint64_t* samplers)
{
    ID3D12GraphicsCommandList* d3dCommandList = reinterpret_cast<ID3D12GraphicsCommandList*>(commandBuffer);

    for (int i = 0; i < count; ++i)
    {
        D3D12_GPU_DESCRIPTOR_HANDLE d3dHandle = { samplers[i] };
        d3dCommandList->SetGraphicsRootDescriptorTable(5, d3dHandle);
    }
}
//------------------------------------------------------------------------------
void xxSetVertexConstantBufferD3D12(uint64_t commandBuffer, uint64_t buffer, unsigned int size)
{
    ID3D12GraphicsCommandList* d3dCommandList = reinterpret_cast<ID3D12GraphicsCommandList*>(commandBuffer);
    D3D12RESOURCE* d3dBuffer = reinterpret_cast<D3D12RESOURCE*>(buffer);

    d3dCommandList->SetGraphicsRootDescriptorTable(0, d3dBuffer->resourceGPUHandle);
}
//------------------------------------------------------------------------------
void xxSetFragmentConstantBufferD3D12(uint64_t commandBuffer, uint64_t buffer, unsigned int size)
{
    ID3D12GraphicsCommandList* d3dCommandList = reinterpret_cast<ID3D12GraphicsCommandList*>(commandBuffer);
    D3D12RESOURCE* d3dBuffer = reinterpret_cast<D3D12RESOURCE*>(buffer);

    d3dCommandList->SetGraphicsRootDescriptorTable(1, d3dBuffer->resourceGPUHandle);
}
//------------------------------------------------------------------------------
void xxDrawIndexedD3D12(uint64_t commandBuffer, int indexCount, int instanceCount, int firstIndex, int vertexOffset, int firstInstance)
{
    ID3D12GraphicsCommandList* d3dCommandList = reinterpret_cast<ID3D12GraphicsCommandList*>(commandBuffer);

    d3dCommandList->DrawIndexedInstanced(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}
//==============================================================================
//  Fixed-Function
//==============================================================================
void xxSetTransformD3D12(uint64_t commandBuffer, const float* world, const float* view, const float* projection)
{

}
//==============================================================================
