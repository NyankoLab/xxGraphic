//==============================================================================
// xxGraphic : Metal 2 Header
//
// Copyright (c) 2019-2025 TAiGA
// https://github.com/metarutaiga/xxGraphic
//==============================================================================
#pragma once

#include "xxGraphic.h"

xxAPI    uint64_t       xxCreateInstanceMetal2();
xxInline char const*    xxGetInstanceNameMetal2() { return "Metal 2"; }

#ifdef MTL_INTERN
#undef xxCreateInstanceMetal2

//==============================================================================
//  Instance
//==============================================================================
xxAPI id        xxCreateInstanceMetal2();
//==============================================================================
//  Command Buffer
//==============================================================================
MTLSWAPCHAIN*   xxGetCommandBufferMetal2(id <MTLDevice> device, MTLSWAPCHAIN* swapchain);
//==============================================================================
//  Render Pass
//==============================================================================
MTLSWAPCHAIN*   xxBeginRenderPassMetal2(MTLSWAPCHAIN* swapchain, MTLFRAMEBUFFER* framebuffer, MTLRenderPassDescriptor* renderPass, int width, int height, float color[4], float depth, unsigned char stencil);
void            xxEndRenderPassMetal2(MTLSWAPCHAIN* swapchain, MTLFRAMEBUFFER* framebuffer, MTLRenderPassDescriptor* renderPass);
//==============================================================================
//  Sampler
//==============================================================================
id              xxCreateSamplerMetal2(id <MTLDevice> device, bool clampU, bool clampV, bool clampW, bool linearMag, bool linearMin, bool linearMip, int anisotropy);
//==============================================================================
//  Shader
//==============================================================================
id              xxCreateMeshShaderMetal2(id <MTLDevice> device, char const* shader);
id              xxCreateVertexShaderMetal2(id <MTLDevice> device, char const* shader, MTLVertexDescriptor* vertexAttribute);
id              xxCreateFragmentShaderMetal2(id <MTLDevice> device, char const* shader);
//==============================================================================
//  Pipeline
//==============================================================================
MTLPIPELINE*    xxCreatePipelineMetal2(id <MTLDevice> device, MTLRenderPassDescriptor* renderPass, MTLRenderPipelineColorAttachmentDescriptor* blendState, id <MTLDepthStencilState> depthStencilState, uint64_t rasterizerState, MTLVertexDescriptor* vertexAttribute, id <MTLFunction> meshShader, id <MTLFunction> vertexShader, id <MTLFunction> fragmentShader);
//==============================================================================
//  Command
//==============================================================================
void            xxSetViewportMetal2(MTLSWAPCHAIN* swapchain, int x, int y, int width, int height, float minZ, float maxZ);
void            xxSetScissorMetal2(MTLSWAPCHAIN* swapchain, int x, int y, int width, int height);
void            xxSetPipelineMetal2(MTLSWAPCHAIN* swapchain, MTLPIPELINE* pipeline);
void            xxSetMeshBuffersMetal2(MTLSWAPCHAIN* swapchain, int count, id <MTLBuffer> __unsafe_unretained* buffers);
void            xxSetVertexBuffersMetal2(MTLSWAPCHAIN* swapchain, int count, id <MTLBuffer> __unsafe_unretained* buffers, MTLVertexDescriptor* vertexAttribute);
void            xxSetMeshTexturesMetal2(MTLSWAPCHAIN* swapchain, int count, MTLTEXTURE** textures);
void            xxSetVertexTexturesMetal2(MTLSWAPCHAIN* swapchain, int count, MTLTEXTURE** textures);
void            xxSetFragmentTexturesMetal2(MTLSWAPCHAIN* swapchain, int count, MTLTEXTURE** textures);
void            xxSetMeshSamplersMetal2(MTLSWAPCHAIN* swapchain, int count, id <MTLSamplerState> __unsafe_unretained* samplers);
void            xxSetVertexSamplersMetal2(MTLSWAPCHAIN* swapchain, int count, id <MTLSamplerState> __unsafe_unretained* samplers);
void            xxSetFragmentSamplersMetal2(MTLSWAPCHAIN* swapchain, int count, id <MTLSamplerState> __unsafe_unretained* samplers);
void            xxSetMeshConstantBufferMetal2(MTLSWAPCHAIN* swapchain, id <MTLBuffer> __unsafe_unretained buffer, int size);
void            xxSetVertexConstantBufferMetal2(MTLSWAPCHAIN* swapchain, id <MTLBuffer> __unsafe_unretained buffer, int size);
void            xxSetFragmentConstantBufferMetal2(MTLSWAPCHAIN* swapchain, id <MTLBuffer> __unsafe_unretained buffer, int size);
void            xxDrawMetal2(MTLSWAPCHAIN* swapchain, int vertexCount, int instanceCount, int firstVertex, int firstInstance);
void            xxDrawMeshedMetal2(MTLSWAPCHAIN* swapchain, int x, int y, int z);
void            xxDrawIndexedMetal2(MTLSWAPCHAIN* swapchain, id <MTLBuffer> __unsafe_unretained indexBuffer, int indexCount, int vertexCount, int instanceCount, int firstIndex, int vertexOffset, int firstInstance);
//==============================================================================

#endif
