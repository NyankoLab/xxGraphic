#include "xxGraphicInternal.h"
#include "xxGraphicD3DAsm.h"
#include "xxGraphicD3D8PS.h"

#include "dxsdk/d3d8.h"

//==============================================================================
//  Resource Type
//==============================================================================
static uint64_t getResourceType(uint64_t resource)
{
    return resource & 7ull;
}
//------------------------------------------------------------------------------
static uint64_t getResourceData(uint64_t resource)
{
    return resource & ~7ull;
}
//==============================================================================
//  Instance
//==============================================================================
uint64_t xxCreateInstanceD3D8PS()
{
    uint64_t instance = xxCreateInstanceD3D8();
    if (instance == 0)
        return 0;

    xxCreateInstance = xxCreateInstanceD3D8PS;
    xxGetDeviceString = xxGetDeviceStringD3D8PS;
    xxCreateVertexAttribute = xxCreateVertexAttributeD3D8PS;
    xxDestroyVertexAttribute = xxDestroyVertexAttributeD3D8PS;
    xxCreateVertexShader = xxCreateVertexShaderD3D8PS;
    xxCreateFragmentShader = xxCreateFragmentShaderD3D8PS;
    xxDestroyShader = xxDestroyShaderD3D8PS;
    xxCreatePipeline = xxCreatePipelineD3D8PS;
    xxSetScissor = xxSetScissorD3D8PS;
    xxSetVertexBuffers = xxSetVertexBuffersD3D8PS;
    xxSetVertexConstantBuffer = xxSetVertexConstantBufferD3D8PS;
    xxSetFragmentConstantBuffer = xxSetFragmentConstantBufferD3D8PS;

    return instance;
}
//==============================================================================
//  Device
//==============================================================================
const char* xxGetDeviceStringD3D8PS(uint64_t device)
{
    return "Direct3D 8.0 Programmable Shader";
}
//==============================================================================
//  Vertex Attribute
//==============================================================================
struct D3DVERTEXATTRIBUTE8PS
{
    DWORD   declaration[16];
    int     stride;
};
//------------------------------------------------------------------------------
uint64_t xxCreateVertexAttributeD3D8PS(uint64_t device, int count, ...)
{
    LPDIRECT3DDEVICE8 d3dDevice = reinterpret_cast<LPDIRECT3DDEVICE8>(device);
    if (d3dDevice == nullptr)
        return 0;
    D3DVERTEXATTRIBUTE8PS* d3dVertexAttribute = new D3DVERTEXATTRIBUTE8PS;
    if (d3dVertexAttribute == nullptr)
        return 0;

    d3dVertexAttribute->declaration[0] = D3DVSD_STREAM(0);
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

        DWORD& declaration = d3dVertexAttribute->declaration[i + 1];

        if (element == 3 && size == sizeof(float) * 3)
        {
            declaration = D3DVSD_REG(i, D3DVSDT_FLOAT3);
            xyzIndex++;
            continue;
        }

        if (element == 4 && size == sizeof(char) * 4)
        {
            declaration = D3DVSD_REG(i, D3DVSDT_D3DCOLOR);
            continue;
        }

        if (element == 2 && size == sizeof(float) * 2)
        {
            declaration = D3DVSD_REG(i, D3DVSDT_FLOAT2);
            textureIndex++;
            continue;
        }
    }
    va_end(args);

    d3dVertexAttribute->declaration[count + 1] = D3DVSD_END();
    d3dVertexAttribute->stride = stride;

    return reinterpret_cast<uint64_t>(d3dVertexAttribute);
}
//------------------------------------------------------------------------------
void xxDestroyVertexAttributeD3D8PS(uint64_t vertexAttribute)
{
    D3DVERTEXATTRIBUTE8PS* d3dVertexAttribute = reinterpret_cast<D3DVERTEXATTRIBUTE8PS*>(vertexAttribute);

    delete d3dVertexAttribute;
}
//==============================================================================
//  Shader
//==============================================================================
uint64_t xxCreateVertexShaderD3D8PS(uint64_t device, const char* shader, uint64_t vertexAttribute)
{
    LPDIRECT3DDEVICE8 d3dDevice = reinterpret_cast<LPDIRECT3DDEVICE8>(device);
    if (d3dDevice == nullptr)
        return 0;
    D3DVERTEXATTRIBUTE8PS* d3dVertexAttribute = reinterpret_cast<D3DVERTEXATTRIBUTE8PS*>(vertexAttribute);
    if (d3dVertexAttribute == nullptr)
        return 0;

    if (strcmp(shader, "default") == 0)
    {
        DWORD d3dShader = 0;
        HRESULT hResult = d3dDevice->CreateVertexShader(d3dVertexAttribute->declaration, vertexShaderCode10, &d3dShader, 0);
        if (hResult != S_OK)
            return 0;

        return d3dShader | 0x100000000ull;
    }

    return 0;
}
//------------------------------------------------------------------------------
uint64_t xxCreateFragmentShaderD3D8PS(uint64_t device, const char* shader)
{
    LPDIRECT3DDEVICE8 d3dDevice = reinterpret_cast<LPDIRECT3DDEVICE8>(device);
    if (d3dDevice == nullptr)
        return 0;

    if (strcmp(shader, "default") == 0)
    {
        DWORD d3dShader = 0;
        HRESULT hResult = d3dDevice->CreatePixelShader(pixelShaderCode11, &d3dShader);
        if (hResult != S_OK)
            return 0;

        return d3dShader | 0x200000000ull;
    }

    return 0;
}
//------------------------------------------------------------------------------
void xxDestroyShaderD3D8PS(uint64_t device, uint64_t shader)
{
    LPDIRECT3DDEVICE8 d3dDevice = reinterpret_cast<LPDIRECT3DDEVICE8>(device);
    if (d3dDevice == nullptr)
        return;

    switch (shader & 0x300000000ull)
    {
    case 0x100000000ull:
    {
        DWORD d3dShader = static_cast<DWORD>(shader);

        d3dDevice->DeleteVertexShader(d3dShader);
        break;
    }
    case 0x200000000ull:
    {
        DWORD d3dShader = static_cast<DWORD>(shader);

        d3dDevice->DeletePixelShader(d3dShader);
        break;
    }
    default:
        break;
    }
}
//==============================================================================
//  Pipeline
//==============================================================================
uint64_t xxCreatePipelineD3D8PS(uint64_t device, uint64_t renderPass, uint64_t blendState, uint64_t depthStencilState, uint64_t rasterizerState, uint64_t vertexAttribute, uint64_t vertexShader, uint64_t fragmentShader)
{
    return xxCreatePipelineD3D8(device, blendState, depthStencilState, rasterizerState, 0, vertexShader, fragmentShader);
}
//==============================================================================
//  Command
//==============================================================================
void xxSetScissorD3D8PS(uint64_t commandEncoder, int x, int y, int width, int height)
{
    xxSetScissorD3D8(commandEncoder, x, y, width, height);

    LPDIRECT3DDEVICE8 d3dDevice = reinterpret_cast<LPDIRECT3DDEVICE8>(commandEncoder);

    D3DMATRIX projection;
    d3dDevice->GetTransform(D3DTS_PROJECTION, &projection);

    d3dDevice->SetVertexShaderConstant(0, projection.m[0], 16);
}
//------------------------------------------------------------------------------
void xxSetVertexBuffersD3D8PS(uint64_t commandEncoder, int count, const uint64_t* buffers, uint64_t vertexAttribute)
{
    LPDIRECT3DDEVICE8 d3dDevice = reinterpret_cast<LPDIRECT3DDEVICE8>(commandEncoder);
    D3DVERTEXATTRIBUTE8PS* d3dVertexAttribute = reinterpret_cast<D3DVERTEXATTRIBUTE8PS*>(vertexAttribute);

    for (int i = 0; i < count; ++i)
    {
        LPDIRECT3DVERTEXBUFFER8 d3dVertexBuffer = reinterpret_cast<LPDIRECT3DVERTEXBUFFER8>(getResourceData(buffers[i]));
        d3dDevice->SetStreamSource(i, d3dVertexBuffer, d3dVertexAttribute->stride);
    }
}
//------------------------------------------------------------------------------
void xxSetVertexConstantBufferD3D8PS(uint64_t commandEncoder, uint64_t buffer, unsigned int size)
{
    LPDIRECT3DDEVICE8 d3dDevice = reinterpret_cast<LPDIRECT3DDEVICE8>(commandEncoder);
    const float* d3dBuffer = reinterpret_cast<float*>(buffer);

    d3dDevice->SetVertexShaderConstant(0, d3dBuffer, size / sizeof(float));
}
//------------------------------------------------------------------------------
void xxSetFragmentConstantBufferD3D8PS(uint64_t commandEncoder, uint64_t buffer, unsigned int size)
{
    LPDIRECT3DDEVICE8 d3dDevice = reinterpret_cast<LPDIRECT3DDEVICE8>(commandEncoder);
    const float* d3dBuffer = reinterpret_cast<float*>(buffer);

    d3dDevice->SetPixelShaderConstant(0, d3dBuffer, size / sizeof(float));
}
//==============================================================================
