#pragma once

#include "xxGraphic.h"

//==============================================================================
//  Instance
//==============================================================================
xxAPI uint64_t      xxCreateInstanceD3D8();
xxAPI void          xxDestroyInstanceD3D8(uint64_t instance);
//==============================================================================
//  Device
//==============================================================================
xxAPI uint64_t      xxCreateDeviceD3D8(uint64_t instance);
xxAPI void          xxDestroyDeviceD3D8(uint64_t device);
xxAPI void          xxResetDeviceD3D8(uint64_t device);
xxAPI bool          xxTestDeviceD3D8(uint64_t device);
xxAPI const char*   xxGetDeviceStringD3D8(uint64_t device);
//==============================================================================
//  Swapchain
//==============================================================================
xxAPI uint64_t      xxCreateSwapchainD3D8(uint64_t device, void* view, unsigned int width, unsigned int height);
xxAPI void          xxDestroySwapchainD3D8(uint64_t swapchain);
xxAPI void          xxPresentSwapchainD3D8(uint64_t swapchain, void* view);
//==============================================================================
//  Command Buffer
//==============================================================================
xxAPI uint64_t      xxGetCommandBufferD3D8(uint64_t device, uint64_t swapchain);
xxAPI bool          xxBeginCommandBufferD3D8(uint64_t commandBuffer);
xxAPI void          xxEndCommandBufferD3D8(uint64_t commandBuffer);
xxAPI void          xxSubmitCommandBufferD3D8(uint64_t commandBuffer);
//==============================================================================
//  Render Pass
//==============================================================================
xxAPI uint64_t      xxCreateRenderPassD3D8(uint64_t device, float r, float g, float b, float a, float depth, unsigned char stencil);
xxAPI void          xxDestroyRenderPassD3D8(uint64_t renderPass);
xxAPI bool          xxBeginRenderPassD3D8(uint64_t commandBuffer, uint64_t renderPass);
xxAPI void          xxEndRenderPassD3D8(uint64_t commandBuffer, uint64_t renderPass);
//==============================================================================
//  Buffer
//==============================================================================
xxAPI uint64_t      xxCreateConstantBufferD3D8(uint64_t device, unsigned int size);
xxAPI uint64_t      xxCreateIndexBufferD3D8(uint64_t device, unsigned int size);
xxAPI uint64_t      xxCreateVertexBufferD3D8(uint64_t device, unsigned int size);
xxAPI void          xxDestroyBufferD3D8(uint64_t buffer);
xxAPI void*         xxMapBufferD3D8(uint64_t device, uint64_t buffer);
xxAPI void          xxUnmapBufferD3D8(uint64_t device, uint64_t buffer);
//==============================================================================
//  Texture
//==============================================================================
xxAPI uint64_t      xxCreateTextureD3D8(uint64_t device, int format, unsigned int width, unsigned int height, unsigned int depth, unsigned int mipmap, unsigned int array);
xxAPI void          xxDestroyTextureD3D8(uint64_t texture);
xxAPI void*         xxMapTextureD3D8(uint64_t device, uint64_t texture, unsigned int& stride, unsigned int level, unsigned int array, unsigned int mipmap);
xxAPI void          xxUnmapTextureD3D8(uint64_t device, uint64_t texture, unsigned int level, unsigned int array, unsigned int mipmap);
//==============================================================================
//  Sampler
//==============================================================================
xxAPI uint64_t      xxCreateSamplerD3D8(uint64_t device, bool clampU, bool clampV, bool clampW, bool linearMag, bool linearMin, bool linearMip, int anisotropy);
xxAPI void          xxDestroySamplerD3D8(uint64_t sampler);
//==============================================================================
//  Vertex Attribute
//==============================================================================
xxAPI uint64_t      xxCreateVertexAttributeD3D8(uint64_t device, int count, ...);
xxAPI void          xxDestroyVertexAttributeD3D8(uint64_t vertexAttribute);
//==============================================================================
//  Shader
//==============================================================================
xxAPI uint64_t      xxCreateVertexShaderD3D8(uint64_t device, const char* shader, uint64_t vertexAttribute);
xxAPI uint64_t      xxCreateFragmentShaderD3D8(uint64_t device, const char* shader);
xxAPI void          xxDestroyShaderD3D8(uint64_t device, uint64_t shader);
//==============================================================================
//  Pipeline
//==============================================================================
xxAPI uint64_t      xxCreateBlendStateD3D8(uint64_t device, bool blending);
xxAPI uint64_t      xxCreateDepthStencilStateD3D8(uint64_t device, bool depthTest, bool depthWrite);
xxAPI uint64_t      xxCreateRasterizerStateD3D8(uint64_t device, bool cull, bool scissor);
xxAPI uint64_t      xxCreatePipelineD3D8(uint64_t device, uint64_t blendState, uint64_t depthStencilState, uint64_t rasterizerState, uint64_t vertexAttribute, uint64_t vertexShader, uint64_t fragmentShader);
xxAPI void          xxDestroyBlendStateD3D8(uint64_t blendState);
xxAPI void          xxDestroyDepthStencilStateD3D8(uint64_t depthStencilState);
xxAPI void          xxDestroyRasterizerStateD3D8(uint64_t rasterizerState);
xxAPI void          xxDestroyPipelineD3D8(uint64_t pipelineState);
//==============================================================================
//  Command
//==============================================================================
xxAPI void          xxSetViewportD3D8(uint64_t commandBuffer, int x, int y, int width, int height, float minZ, float maxZ);
xxAPI void          xxSetScissorD3D8(uint64_t commandBuffer, int x, int y, int width, int height);
xxAPI void          xxSetPipelineD3D8(uint64_t commandBuffer, uint64_t pipeline);
xxAPI void          xxSetIndexBufferD3D8(uint64_t commandBuffer, uint64_t buffer);
xxAPI void          xxSetVertexBuffersD3D8(uint64_t commandBuffer, int count, const uint64_t* buffers, uint64_t vertexAttribute);
xxAPI void          xxSetVertexTexturesD3D8(uint64_t commandBuffer, int count, const uint64_t* textures);
xxAPI void          xxSetFragmentTexturesD3D8(uint64_t commandBuffer, int count, const uint64_t* textures);
xxAPI void          xxSetVertexSamplersD3D8(uint64_t commandBuffer, int count, const uint64_t* samplers);
xxAPI void          xxSetFragmentSamplersD3D8(uint64_t commandBuffer, int count, const uint64_t* samplers);
xxAPI void          xxSetVertexConstantBufferD3D8(uint64_t commandBuffer, uint64_t buffer, unsigned int size);
xxAPI void          xxSetFragmentConstantBufferD3D8(uint64_t commandBuffer, uint64_t buffer, unsigned int size);
xxAPI void          xxDrawIndexedD3D8(uint64_t commandBuffer, int indexCount, int instanceCount, int firstIndex, int vertexOffset, int firstInstance);
//==============================================================================
//  Fixed-Function
//==============================================================================
xxAPI void          xxSetTransformD3D8(uint64_t commandBuffer, const float* world, const float* view, const float* projection);
