//==============================================================================
// xxGraphic : Metal Internal Header
//
// Copyright (c) 2019-2025 TAiGA
// https://github.com/metarutaiga/xxGraphic
//==============================================================================
#pragma once

#include "xxGraphic.h"
#include "xxGraphicInternal.h"

#pragma clang diagnostic ignored "-Wnullability-completeness"

#define MTLCreateSystemDefaultDevice MTLCreateSystemDefaultDevice_unused
#define MTLCopyAllDevices MTLCopyAllDevices_unused
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#if defined(xxMACOS)
#import <Cocoa/Cocoa.h>
#elif defined(xxIOS)
#import <UIKit/UIKit.h>
#endif
#undef MTLCreateSystemDefaultDevice
#undef MTLCopyAllDevices

extern Class classMTLCompileOptions;
extern Class classMTLDepthStencilDescriptor;
extern Class classMTLRenderPassDescriptor;
extern Class classMTLRenderPipelineColorAttachmentDescriptor;
extern Class classMTLRenderPipelineDescriptor;
extern Class classMTLSamplerDescriptor;
extern Class classMTLTextureDescriptor;
extern Class classMTLVertexDescriptor;

struct MTLPIPELINE;
struct MTLSWAPCHAIN;

//==============================================================================
//  MSL Shader
//==============================================================================
extern char const* const mtlDefaultShaderCode;

//==============================================================================
//  Blend Factor
//==============================================================================
inline MTLBlendFactor mtlBlendFactor(char const* name)
{
    return xxTemplateBlendFactor<MTLBlendFactor, MTLBlendFactorZero,
                                                 MTLBlendFactorOne,
                                                 MTLBlendFactorSourceColor,
                                                 MTLBlendFactorOneMinusSourceColor,
                                                 MTLBlendFactorDestinationColor,
                                                 MTLBlendFactorOneMinusDestinationColor,
                                                 MTLBlendFactorSourceAlpha,
                                                 MTLBlendFactorOneMinusSourceAlpha,
                                                 MTLBlendFactorDestinationAlpha,
                                                 MTLBlendFactorOneMinusDestinationAlpha>(name);
}
//==============================================================================
//  Blend Operation
//==============================================================================
inline MTLBlendOperation mtlBlendOp(char const* name)
{
    return xxTemplateBlendOp<MTLBlendOperation, MTLBlendOperationAdd,
                                                MTLBlendOperationSubtract,
                                                MTLBlendOperationReverseSubtract,
                                                MTLBlendOperationMin,
                                                MTLBlendOperationMax>(name);
}
//==============================================================================
//  Depth Comparison
//==============================================================================
inline MTLCompareFunction mtlCompareOp(char const* name)
{
    return xxTemplateCompareOp<MTLCompareFunction, MTLCompareFunctionNever,
                                                   MTLCompareFunctionLess,
                                                   MTLCompareFunctionEqual,
                                                   MTLCompareFunctionLessEqual,
                                                   MTLCompareFunctionGreater,
                                                   MTLCompareFunctionNotEqual,
                                                   MTLCompareFunctionGreaterEqual,
                                                   MTLCompareFunctionAlways>(name);
}
//==============================================================================
//  Framebuffer
//==============================================================================
struct MTLFRAMEBUFFER
{
    id <MTLTexture>                 texture;
    id <MTLTexture>                 depthstencil;
};
//==============================================================================
//  Swapchain
//==============================================================================
struct MTLSWAPCHAIN : public MTLFRAMEBUFFER
{
    CAMetalLayer*                   metalLayer;
    id <CAMetalDrawable>            drawable;
    id <MTLCommandQueue>            commandQueue;
    id <MTLCommandBuffer>           commandBuffer;
#if defined(xxMACOS)
    NSView*                         view;
#elif defined(xxIOS)
    UIView*                         view;
#endif
    int                             width;
    int                             height;
    float                           scale;

    // Metal 2
    MTLPIPELINE*                    pipeline;
    NSUInteger                      frameCount;
    NSString*                       frameString;
    id <MTLRenderCommandEncoder>    commandEncoder;
    int                             argumentBufferIndex;
    int                             argumentBufferStep;
    id <MTLBuffer>                  argumentBuffers[4];
    id <MTLArgumentEncoder>         meshArgumentEncoder;
    id <MTLArgumentEncoder>         vertexArgumentEncoder;
    id <MTLArgumentEncoder>         fragmentArgumentEncoder;
    bool                            argumentEncoderComplete;
};
//==============================================================================
//  Texture
//==============================================================================
struct MTLTEXTURE
{
    id <MTLTexture>                 texture;
    id <MTLBuffer>                  buffer;
    int                             strides[16];
    void*                           temporary;
};
//==============================================================================
//  Pipeline
//==============================================================================
struct MTLPIPELINE
{
    id <MTLRenderPipelineState>     pipeline;
    id <MTLDepthStencilState>       depthStencil;
    MTLCullMode                     cullMode;
    MTLTriangleFillMode             fillMode;
    bool                            scissorEnable;

    // Metal 2
    id <MTLFunction>                meshShader;
    id <MTLFunction>                vertexShader;
    id <MTLFunction>                fragmentShader;
    id <MTLArgumentEncoder>         meshArgumentEncoder;
    id <MTLArgumentEncoder>         vertexArgumentEncoder;
    id <MTLArgumentEncoder>         fragmentArgumentEncoder;
    NSUInteger                      meshArgumentEncodedLength;
    NSUInteger                      vertexArgumentEncodedLength;
    NSUInteger                      fragmentArgumentEncodedLength;
};
//==============================================================================

//==============================================================================
//  Argument Encoder
//==============================================================================
extern void mtlUpdateArgumentEncoderInternal(MTLSWAPCHAIN* swapchain);
inline void mtlUpdateArgumentEncoder(MTLSWAPCHAIN* swapchain)
{
    if (swapchain->argumentEncoderComplete == false)
        return;
    mtlUpdateArgumentEncoderInternal(swapchain);
}
//==============================================================================
