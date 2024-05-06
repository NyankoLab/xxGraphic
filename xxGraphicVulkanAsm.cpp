//==============================================================================
// xxGraphic : Vulkan Assembly Shader Source
//
// Copyright (c) 2019-2024 TAiGA
// https://github.com/metarutaiga/xxGraphic
//==============================================================================
#include "xxSystem.h"
#include "vulkan/spirv.h"
#include "vulkan/GLSL.std.450.h"
#include "internal/xxGraphicInternal.h"
#include "xxGraphicVulkanAsm.h"

//==============================================================================
//  Vulkan SPIR-V Assembly Shader
//==============================================================================
#define _glsl_std_450                   0x4c534c47, 0x6474732e, 0x3035342e
#define _Main                           0x6e69614d
#define Op(op, length)                  (op | (length << SpvWordCountShift))
//------------------------------------------------------------------------------
const uint32_t vertexShaderCodeSPIRV[] =
{
    SpvMagicNumber, SpvVersion, 0, 147, 0,
    Op(SpvOpCapability, 2),             SpvCapabilityShader,
    Op(SpvOpExtInstImport, 6),          1, _glsl_std_450, 0,
    Op(SpvOpMemoryModel, 3),            SpvAddressingModelLogical, SpvMemoryModelGLSL450,
    Op(SpvOpEntryPoint, 11),            SpvExecutionModelVertex, 4, _Main, 0, 23, 122, 128, 130, 132, 134,
    Op(SpvOpDecorate, 4),               12, SpvDecorationArrayStride, 16,
    Op(SpvOpMemberDecorate, 5),         13, 0, SpvDecorationOffset, 0,
    Op(SpvOpDecorate, 3),               13, SpvDecorationBlock,
    Op(SpvOpDecorate, 4),               15, SpvDecorationDescriptorSet, 0,
    Op(SpvOpDecorate, 4),               15, SpvDecorationBinding, 0,
    Op(SpvOpDecorate, 4),               23, SpvDecorationLocation, 0,
    Op(SpvOpMemberDecorate, 5),         120, 0, SpvDecorationBuiltIn, SpvBuiltInPosition,
    Op(SpvOpMemberDecorate, 5),         120, 1, SpvDecorationBuiltIn, SpvBuiltInPointSize,
    Op(SpvOpMemberDecorate, 5),         120, 2, SpvDecorationBuiltIn, SpvBuiltInClipDistance,
    Op(SpvOpMemberDecorate, 5),         120, 3, SpvDecorationBuiltIn, SpvBuiltInCullDistance,
    Op(SpvOpDecorate, 3),               120, SpvDecorationBlock,
    Op(SpvOpDecorate, 4),               128, SpvDecorationLocation, 1,
    Op(SpvOpDecorate, 4),               130, SpvDecorationLocation, 2,
    Op(SpvOpDecorate, 4),               132, SpvDecorationLocation, 0,
    Op(SpvOpDecorate, 4),               134, SpvDecorationLocation, 1,
    Op(SpvOpTypeVoid, 2),               2,
    Op(SpvOpTypeFunction, 3),           3, 2,
    Op(SpvOpTypeFloat, 3),              6, 32,
    Op(SpvOpTypeVector, 4),             7, 6, 4,
    Op(SpvOpTypeInt, 4),                10, 32, 0,
    Op(SpvOpConstant, 4),               10, 11, 12,
    Op(SpvOpTypeArray, 4),              12, 7, 11,
    Op(SpvOpTypeStruct, 3),             13, 12,
    Op(SpvOpTypePointer, 4),            14, SpvStorageClassUniform, 13,
    Op(SpvOpVariable, 4),               14, 15, SpvStorageClassUniform,
    Op(SpvOpTypeInt, 4),                16, 32, 1,
    Op(SpvOpConstant, 4),               16, 17, 0,
    Op(SpvOpTypePointer, 4),            18, SpvStorageClassUniform, 7,
    Op(SpvOpTypeVector, 4),             21, 6, 3,
    Op(SpvOpTypePointer, 4),            22, SpvStorageClassInput, 21,
    Op(SpvOpVariable, 4),               22, 23, SpvStorageClassInput,
    Op(SpvOpConstant, 4),               10, 24, 0,
    Op(SpvOpTypePointer, 4),            25, SpvStorageClassInput, 6,
    Op(SpvOpConstant, 4),               16, 29, 1,
    Op(SpvOpConstant, 4),               10, 32, 1,
    Op(SpvOpConstant, 4),               16, 38, 2,
    Op(SpvOpConstant, 4),               10, 41, 2,
    Op(SpvOpConstant, 4),               16, 47, 3,
    Op(SpvOpConstant, 4),               16, 53, 4,
    Op(SpvOpConstant, 4),               16, 60, 5,
    Op(SpvOpConstant, 4),               16, 68, 6,
    Op(SpvOpConstant, 4),               16, 76, 7,
    Op(SpvOpConstant, 4),               16, 85, 8,
    Op(SpvOpConstant, 4),               16, 91, 9,
    Op(SpvOpConstant, 4),               16, 99, 10,
    Op(SpvOpConstant, 4),               16, 107, 11,
    Op(SpvOpTypeArray, 4),              119, 6, 32,
    Op(SpvOpTypeStruct, 6),             120, 7, 6, 119, 119,
    Op(SpvOpTypePointer, 4),            121, SpvStorageClassOutput, 120,
    Op(SpvOpVariable, 4),               121, 122, SpvStorageClassOutput,
    Op(SpvOpTypePointer, 4),            124, SpvStorageClassOutput, 7,
    Op(SpvOpTypeVector, 4),             126, 6, 2,
    Op(SpvOpTypePointer, 4),            127, SpvStorageClassOutput, 126,
    Op(SpvOpVariable, 4),               127, 128, SpvStorageClassOutput,
    Op(SpvOpTypePointer, 4),            129, SpvStorageClassInput, 126,
    Op(SpvOpVariable, 4),               129, 130, SpvStorageClassInput,
    Op(SpvOpVariable, 4),               124, 132, SpvStorageClassOutput,
    Op(SpvOpTypePointer, 4),            133, SpvStorageClassInput, 7,
    Op(SpvOpVariable, 4),               133, 134, SpvStorageClassInput,
    Op(SpvOpFunction, 5),               2, 4, SpvFunctionControlMaskNone, 3,
    Op(SpvOpLabel, 2),                  5,
    Op(SpvOpAccessChain, 6),            18, 19, 15, 17, 17,
    Op(SpvOpLoad, 4),                   7, 20, 19,
    Op(SpvOpAccessChain, 5),            25, 26, 23, 24,
    Op(SpvOpLoad, 4),                   6, 27, 26,
    Op(SpvOpVectorTimesScalar, 5),      7, 28, 20, 27,
    Op(SpvOpAccessChain, 6),            18, 30, 15, 17, 29,
    Op(SpvOpLoad, 4),                   7, 31, 30,
    Op(SpvOpAccessChain, 5),            25, 33, 23, 32,
    Op(SpvOpLoad, 4),                   6, 34, 33,
    Op(SpvOpVectorTimesScalar, 5),      7, 35, 31, 34,
    Op(SpvOpFAdd, 5),                   7, 37, 28, 35,
    Op(SpvOpAccessChain, 6),            18, 39, 15, 17, 38,
    Op(SpvOpLoad, 4),                   7, 40, 39,
    Op(SpvOpAccessChain, 5),            25, 42, 23, 41,
    Op(SpvOpLoad, 4),                   6, 43, 42,
    Op(SpvOpVectorTimesScalar, 5),      7, 44, 40, 43,
    Op(SpvOpFAdd, 5),                   7, 46, 37, 44,
    Op(SpvOpAccessChain, 6),            18, 48, 15, 17, 47,
    Op(SpvOpLoad, 4),                   7, 49, 48,
    Op(SpvOpFAdd, 5),                   7, 51, 46, 49,
    Op(SpvOpAccessChain, 6),            18, 54, 15, 17, 53,
    Op(SpvOpLoad, 4),                   7, 55, 54,
    Op(SpvOpCompositeExtract, 5),       6, 58, 51, 0,
    Op(SpvOpVectorTimesScalar, 5),      7, 59, 55, 58,
    Op(SpvOpAccessChain, 6),            18, 61, 15, 17, 60,
    Op(SpvOpLoad, 4),                   7, 62, 61,
    Op(SpvOpCompositeExtract, 5),       6, 64, 51, 1,
    Op(SpvOpVectorTimesScalar, 5),      7, 65, 62, 64,
    Op(SpvOpFAdd, 5),                   7, 67, 59, 65,
    Op(SpvOpAccessChain, 6),            18, 69, 15, 17, 68,
    Op(SpvOpLoad, 4),                   7, 70, 69,
    Op(SpvOpCompositeExtract, 5),       6, 72, 51, 2,
    Op(SpvOpVectorTimesScalar, 5),      7, 73, 70, 72,
    Op(SpvOpFAdd, 5),                   7, 75, 67, 73,
    Op(SpvOpAccessChain, 6),            18, 77, 15, 17, 76,
    Op(SpvOpLoad, 4),                   7, 78, 77,
    Op(SpvOpCompositeExtract, 5),       6, 81, 51, 3,
    Op(SpvOpVectorTimesScalar, 5),      7, 82, 78, 81,
    Op(SpvOpFAdd, 5),                   7, 84, 75, 82,
    Op(SpvOpAccessChain, 6),            18, 86, 15, 17, 85,
    Op(SpvOpLoad, 4),                   7, 87, 86,
    Op(SpvOpCompositeExtract, 5),       6, 89, 84, 0,
    Op(SpvOpVectorTimesScalar, 5),      7, 90, 87, 89,
    Op(SpvOpAccessChain, 6),            18, 92, 15, 17, 91,
    Op(SpvOpLoad, 4),                   7, 93, 92,
    Op(SpvOpCompositeExtract, 5),       6, 95, 84, 1,
    Op(SpvOpVectorTimesScalar, 5),      7, 96, 93, 95,
    Op(SpvOpFAdd, 5),                   7, 98, 90, 96,
    Op(SpvOpAccessChain, 6),            18, 100, 15, 17, 99,
    Op(SpvOpLoad, 4),                   7, 101, 100,
    Op(SpvOpCompositeExtract, 5),       6, 103, 84, 2,
    Op(SpvOpVectorTimesScalar, 5),      7, 104, 101, 103,
    Op(SpvOpFAdd, 5),                   7, 106, 98, 104,
    Op(SpvOpAccessChain, 6),            18, 108, 15, 17, 107,
    Op(SpvOpLoad, 4),                   7, 109, 108,
    Op(SpvOpCompositeExtract, 5),       6, 111, 84, 3,
    Op(SpvOpVectorTimesScalar, 5),      7, 112, 109, 111,
    Op(SpvOpFAdd, 5),                   7, 114, 106, 112,
    Op(SpvOpCompositeExtract, 5),       6, 116, 114, 1,
    Op(SpvOpFNegate, 4),                6, 117, 116,
    Op(SpvOpCompositeInsert, 6),        7, 146, 117, 114, 1,
    Op(SpvOpAccessChain, 5),            124, 125, 122, 17,
    Op(SpvOpStore, 3),                  125, 146,
    Op(SpvOpLoad, 4),                   126, 131, 130,
    Op(SpvOpStore, 3),                  128, 131,
    Op(SpvOpLoad, 4),                   7, 135, 134,
    Op(SpvOpStore, 3),                  132, 135,
    Op(SpvOpReturn, 1),
    Op(SpvOpFunctionEnd, 1)
};
const uint32_t vertexShaderCodeSPIRVSize = sizeof(vertexShaderCodeSPIRV);
//------------------------------------------------------------------------------
const uint32_t fragmentShaderCodeSPIRV[] =
{
    SpvMagicNumber, SpvVersion, 0, 35, 0,
    Op(SpvOpCapability, 2),             SpvCapabilityShader,
    Op(SpvOpExtInstImport, 6),          1, _glsl_std_450, 0,
    Op(SpvOpMemoryModel, 3),            SpvAddressingModelLogical, SpvMemoryModelGLSL450,
    Op(SpvOpEntryPoint, 7),             SpvExecutionModelFragment, 4, _Main, 0, 9, 13,
    Op(SpvOpExecutionMode, 3),          4, SpvExecutionModeOriginUpperLeft,
    Op(SpvOpDecorate, 4),               9, SpvDecorationLocation, 0,
    Op(SpvOpDecorate, 4),               13, SpvDecorationLocation, 0,
    Op(SpvOpDecorate, 4),               21, SpvDecorationDescriptorSet, 0,
    Op(SpvOpDecorate, 4),               21, SpvDecorationBinding, xxGraphicDescriptor::FRAGMENT_TEXTURE,
    Op(SpvOpDecorate, 4),               25, SpvDecorationDescriptorSet, 0,
    Op(SpvOpDecorate, 4),               25, SpvDecorationBinding, xxGraphicDescriptor::FRAGMENT_SAMPLER,
    Op(SpvOpTypeVoid, 2),               2,
    Op(SpvOpTypeFunction, 3),           3, 2,
    Op(SpvOpTypeFloat, 3),              6, 32,
    Op(SpvOpTypeVector, 4),             7, 6, 4,
    Op(SpvOpTypePointer, 4),            8, SpvStorageClassOutput, 7,
    Op(SpvOpVariable, 4),               8, 9, SpvStorageClassOutput,
    Op(SpvOpTypeVector, 4),             10, 6, 2,
    Op(SpvOpTypeStruct, 4),             11, 7, 10,
    Op(SpvOpTypePointer, 4),            12, SpvStorageClassInput, 11,
    Op(SpvOpVariable, 4),               12, 13, SpvStorageClassInput,
    Op(SpvOpTypeInt, 4),                14, 32, 1,
    Op(SpvOpConstant, 4),               14, 15, 0,
    Op(SpvOpTypePointer, 4),            16, SpvStorageClassInput, 7,
    Op(SpvOpTypeImage, 9),              19, 6, SpvDim2D, 0, 0, 0, 1, SpvImageFormatUnknown,
    Op(SpvOpTypePointer, 4),            20, SpvStorageClassUniformConstant, 19,
    Op(SpvOpVariable, 4),               20, 21, SpvStorageClassUniformConstant,
    Op(SpvOpTypeSampler, 2),            23,
    Op(SpvOpTypePointer, 4),            24, SpvStorageClassUniformConstant, 23,
    Op(SpvOpVariable, 4),               24, 25, SpvStorageClassUniformConstant,
    Op(SpvOpTypeSampledImage, 3),       27, 19,
    Op(SpvOpConstant, 4),               14, 29, 1,
    Op(SpvOpTypePointer, 4),            30, SpvStorageClassInput, 10,
    Op(SpvOpFunction, 5),               2, 4, SpvFunctionControlMaskNone, 3,
    Op(SpvOpLabel, 2),                  5,
    Op(SpvOpAccessChain, 5),            16, 17, 13, 15,
    Op(SpvOpLoad, 4),                   7, 18, 17,
    Op(SpvOpLoad, 4),                   19, 22, 21,
    Op(SpvOpLoad, 4),                   23, 26, 25,
    Op(SpvOpSampledImage, 5),           27, 28, 22, 26,
    Op(SpvOpAccessChain, 5),            30, 31, 13, 29,
    Op(SpvOpLoad, 4),                   10, 32, 31,
    Op(SpvOpImageSampleImplicitLod, 5), 7, 33, 28, 32,
    Op(SpvOpFMul, 5),                   7, 34, 18, 33,
    Op(SpvOpStore, 3),                  9, 34,
    Op(SpvOpReturn, 1),
    Op(SpvOpFunctionEnd, 1)
};
const uint32_t fragmentShaderCodeSPIRVSize = sizeof(fragmentShaderCodeSPIRV);
//------------------------------------------------------------------------------
