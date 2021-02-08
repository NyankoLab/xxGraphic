//=====================================================================
// Copyright 2015 (c), Advanced Micro Devices, Inc. All rights reserved.
//=====================================================================
#ifndef _MANTLE_H_
#define _MANTLE_H_

#ifdef __cplusplus
extern "C" {
#endif

// DEFINES

#define GR_API_VERSION              1
#define GR_MAX_PHYSICAL_GPUS        4
#define GR_MAX_PHYSICAL_GPU_NAME    256
#define GR_MAX_VIEWPORTS            16
#define GR_MAX_MEMORY_HEAPS         8
#define GR_MAX_COLOR_TARGETS        16
#define GR_MAX_DESCRIPTOR_SETS      2
#define GR_STDCALL                  __stdcall
#define GR_NULL_HANDLE              0

#define GR_TRUE                     1
#define GR_FALSE                    0

// TYPES

typedef char GR_CHAR;
typedef int GR_INT;
typedef unsigned int GR_UINT;
typedef float GR_FLOAT;
typedef void GR_VOID;

typedef signed char GR_INT8;
typedef signed short GR_INT16;
typedef signed int GR_INT32;
typedef signed long long GR_INT64;
typedef unsigned char GR_UINT8;
typedef unsigned short GR_UINT16;
typedef unsigned int GR_UINT32;
typedef unsigned long long GR_UINT64;
typedef int GR_BOOL;

typedef int GR_ENUM;
typedef int GR_FLAGS;
typedef unsigned int GR_SAMPLE_MASK;

typedef size_t GR_SIZE;
typedef unsigned long long GR_GPU_SIZE;

typedef void* GR_HANDLE;

typedef GR_HANDLE GR_BASE_OBJECT;
typedef GR_HANDLE GR_CMD_BUFFER;
typedef GR_HANDLE GR_COLOR_BLEND_STATE_OBJECT;
typedef GR_HANDLE GR_COLOR_TARGET_VIEW;
typedef GR_HANDLE GR_DEPTH_STENCIL_STATE_OBJECT;
typedef GR_HANDLE GR_DEPTH_STENCIL_VIEW;
typedef GR_HANDLE GR_DESCRIPTOR_SET;
typedef GR_HANDLE GR_DEVICE;
typedef GR_HANDLE GR_EVENT;
typedef GR_HANDLE GR_FENCE;
typedef GR_HANDLE GR_GPU_MEMORY;
typedef GR_HANDLE GR_IMAGE_VIEW;
typedef GR_HANDLE GR_IMAGE;
typedef GR_HANDLE GR_MSAA_STATE_OBJECT;
typedef GR_HANDLE GR_OBJECT;
typedef GR_HANDLE GR_PHYSICAL_GPU;
typedef GR_HANDLE GR_PIPELINE;
typedef GR_HANDLE GR_QUERY_POOL;
typedef GR_HANDLE GR_QUEUE_SEMAPHORE;
typedef GR_HANDLE GR_QUEUE;
typedef GR_HANDLE GR_RASTER_STATE_OBJECT;
typedef GR_HANDLE GR_SAMPLER;
typedef GR_HANDLE GR_SHADER;
typedef GR_HANDLE GR_STATE_OBJECT;
typedef GR_HANDLE GR_VIEWPORT_STATE_OBJECT;

// ENUMERATIONS

typedef enum _GR_ATOMIC_OP
{
    GR_ATOMIC_ADD_INT32     = 0x2d00,
    GR_ATOMIC_SUB_INT32     = 0x2d01,
    GR_ATOMIC_MIN_UINT32    = 0x2d02,
    GR_ATOMIC_MAX_UINT32    = 0x2d03,
    GR_ATOMIC_MIN_SINT32    = 0x2d04,
    GR_ATOMIC_MAX_SINT32    = 0x2d05,
    GR_ATOMIC_AND_INT32     = 0x2d06,
    GR_ATOMIC_OR_INT32      = 0x2d07,
    GR_ATOMIC_XOR_INT32     = 0x2d08,
    GR_ATOMIC_INC_UINT32    = 0x2d09,
    GR_ATOMIC_DEC_UINT32    = 0x2d0a,
    GR_ATOMIC_ADD_INT64     = 0x2d0b,
    GR_ATOMIC_SUB_INT64     = 0x2d0c,
    GR_ATOMIC_MIN_UINT64    = 0x2d0d,
    GR_ATOMIC_MAX_UINT64    = 0x2d0e,
    GR_ATOMIC_MIN_SINT64    = 0x2d0f,
    GR_ATOMIC_MAX_SINT64    = 0x2d10,
    GR_ATOMIC_AND_INT64     = 0x2d11,
    GR_ATOMIC_OR_INT64      = 0x2d12,
    GR_ATOMIC_XOR_INT64     = 0x2d13,
    GR_ATOMIC_INC_UINT64    = 0x2d14,
    GR_ATOMIC_DEC_UINT64    = 0x2d15,
} GR_ATOMIC_OP;

typedef enum _GR_BORDER_COLOR_TYPE
{
    GR_BORDER_COLOR_WHITE               = 0x1c00,
    GR_BORDER_COLOR_TRANSPARENT_BLACK   = 0x1c01,
    GR_BORDER_COLOR_OPAQUE_BLACK        = 0x1c02,
} GR_BORDER_COLOR_TYPE;

typedef enum _GR_BLEND
{
    GR_BLEND_ZERO                       = 0x2900,
    GR_BLEND_ONE                        = 0x2901,
    GR_BLEND_SRC_COLOR                  = 0x2902,
    GR_BLEND_ONE_MINUS_SRC_COLOR        = 0x2903,
    GR_BLEND_DEST_COLOR                 = 0x2904,
    GR_BLEND_ONE_MINUS_DEST_COLOR       = 0x2905,
    GR_BLEND_SRC_ALPHA                  = 0x2906,
    GR_BLEND_ONE_MINUS_SRC_ALPHA        = 0x2907,
    GR_BLEND_DEST_ALPHA                 = 0x2908,
    GR_BLEND_ONE_MINUS_DEST_ALPHA       = 0x2909,
    GR_BLEND_CONSTANT_COLOR             = 0x290a,
    GR_BLEND_ONE_MINUS_CONSTANT_COLOR   = 0x290b,
    GR_BLEND_CONSTANT_ALPHA             = 0x290c,
    GR_BLEND_ONE_MINUS_CONSTANT_ALPHA   = 0x290d,
    GR_BLEND_SRC_ALPHA_SATURATE         = 0x290e,
    GR_BLEND_SRC1_COLOR                 = 0x290f,
    GR_BLEND_ONE_MINUS_SRC1_COLOR       = 0x2910,
    GR_BLEND_SRC1_ALPHA                 = 0x2911,
    GR_BLEND_ONE_MINUS_SRC1_ALPHA       = 0x2912,
} GR_BLEND;

typedef enum _GR_BLEND_FUNC
{
    GR_BLEND_FUNC_ADD               = 0x2a00,
    GR_BLEND_FUNC_SUBTRACT          = 0x2a01,
    GR_BLEND_FUNC_REVERSE_SUBTRACT  = 0x2a02,
    GR_BLEND_FUNC_MIN               = 0x2a03,
    GR_BLEND_FUNC_MAX               = 0x2a04,
} GR_BLEND_FUNC;

typedef enum _GR_CHANNEL_FORMAT {
    GR_CH_FMT_UNDEFINED     = 0,
    GR_CH_FMT_R4G4          = 1,
    GR_CH_FMT_R4G4B4A4      = 2,
    GR_CH_FMT_R5G6B5        = 3,
    GR_CH_FMT_B5G6R5        = 4,
    GR_CH_FMT_R5G5B5A1      = 5,
    GR_CH_FMT_R8            = 6,
    GR_CH_FMT_R8G8          = 7,
    GR_CH_FMT_R8G8B8A8      = 8,
    GR_CH_FMT_B8G8R8A8      = 9,
    GR_CH_FMT_R10G11B11     = 10,
    GR_CH_FMT_R11G11B10     = 11,
    GR_CH_FMT_R10G10B10A2   = 12,
    GR_CH_FMT_R16           = 13,
    GR_CH_FMT_R16G16        = 14,
    GR_CH_FMT_R16G16B16A16  = 15,
    GR_CH_FMT_R32           = 16,
    GR_CH_FMT_R32G32        = 17,
    GR_CH_FMT_R32G32B32     = 18,
    GR_CH_FMT_R32G32B32A32  = 19,
    GR_CH_FMT_R16G8         = 20,
    GR_CH_FMT_R32G8         = 21,
    GR_CH_FMT_R9G9B9E5      = 22,
    GR_CH_FMT_BC1           = 23,
    GR_CH_FMT_BC2           = 24,
    GR_CH_FMT_BC3           = 25,
    GR_CH_FMT_BC4           = 26,
    GR_CH_FMT_BC5           = 27,
    GR_CH_FMT_BC6U          = 28,
    GR_CH_FMT_BC6S          = 29,
    GR_CH_FMT_BC7           = 30,
} GR_CHANNEL_FORMAT;

typedef enum _GR_CHANNEL_SWIZZLE
{
    GR_CHANNEL_SWIZZLE_ZERO = 0x1800,
    GR_CHANNEL_SWIZZLE_ONE  = 0x1801,
    GR_CHANNEL_SWIZZLE_R    = 0x1802,
    GR_CHANNEL_SWIZZLE_G    = 0x1803,
    GR_CHANNEL_SWIZZLE_B    = 0x1804,
    GR_CHANNEL_SWIZZLE_A    = 0x1805,
} GR_CHANNEL_SWIZZLE;

typedef enum _GR_COMPARE_FUNC
{
    GR_COMPARE_NEVER            = 0x2500,
    GR_COMPARE_LESS             = 0x2501,
    GR_COMPARE_EQUAL            = 0x2502,
    GR_COMPARE_LESS_EQUAL       = 0x2503,
    GR_COMPARE_GREATER          = 0x2504,
    GR_COMPARE_NOT_EQUAL        = 0x2505,
    GR_COMPARE_GREATER_EQUAL    = 0x2506,
    GR_COMPARE_ALWAYS           = 0x2507,
} GR_COMPARE_FUNC;

typedef enum _GR_CULL_MODE
{
    GR_CULL_NONE    = 0x2700,
    GR_CULL_FRONT   = 0x2701,
    GR_CULL_BACK    = 0x2702,
} GR_CULL_MODE;

typedef enum _GR_DESCRIPTOR_SET_SLOT_TYPE
{
    GR_SLOT_UNUSED              = 0x1900,
    GR_SLOT_SHADER_RESOURCE     = 0x1901,
    GR_SLOT_SHADER_UAV          = 0x1902,
    GR_SLOT_SHADER_SAMPLER      = 0x1903,
    GR_SLOT_NEXT_DESCRIPTOR_SET = 0x1904,
} GR_DESCRIPTOR_SET_SLOT_TYPE;

typedef enum _GR_FACE_ORIENTATION
{
    GR_FRONT_FACE_CCW   = 0x2800,
    GR_FRONT_FACE_CW    = 0x2801,
} GR_FACE_ORIENTATION;

typedef enum _GR_FILL_MODE
{
    GR_FILL_SOLID       = 0x2600,
    GR_FILL_WIREFRAME   = 0x2601,
} GR_FILL_MODE;

typedef enum _GR_HEAP_MEMORY_TYPE
{
    GR_HEAP_MEMORY_OTHER    = 0x2f00,
    GR_HEAP_MEMORY_LOCAL    = 0x2f01,
    GR_HEAP_MEMORY_REMOTE   = 0x2f02,
    GR_HEAP_MEMORY_EMBEDDED = 0x2f03,
} GR_HEAP_MEMORY_TYPE;

typedef enum _GR_IMAGE_ASPECT
{
    GR_IMAGE_ASPECT_COLOR   = 0x1700,
    GR_IMAGE_ASPECT_DEPTH   = 0x1701,
    GR_IMAGE_ASPECT_STENCIL = 0x1702,
} GR_IMAGE_ASPECT;

typedef enum _GR_IMAGE_STATE
{
    GR_IMAGE_STATE_DATA_TRANSFER                = 0x1300,
    GR_IMAGE_STATE_GRAPHICS_SHADER_READ_ONLY    = 0x1301,
    GR_IMAGE_STATE_GRAPHICS_SHADER_WRITE_ONLY   = 0x1302,
    GR_IMAGE_STATE_GRAPHICS_SHADER_READ_WRITE   = 0x1303,
    GR_IMAGE_STATE_COMPUTE_SHADER_READ_ONLY     = 0x1304,
    GR_IMAGE_STATE_COMPUTE_SHADER_WRITE_ONLY    = 0x1305,
    GR_IMAGE_STATE_COMPUTE_SHADER_READ_WRITE    = 0x1306,
    GR_IMAGE_STATE_MULTI_SHADER_READ_ONLY       = 0x1307,
    GR_IMAGE_STATE_TARGET_AND_SHADER_READ_ONLY  = 0x1308,
    GR_IMAGE_STATE_UNINITIALIZED                = 0x1309,
    GR_IMAGE_STATE_TARGET_RENDER_ACCESS_OPTIMAL = 0x130a,
    GR_IMAGE_STATE_TARGET_SHADER_ACCESS_OPTIMAL = 0x130b,
    GR_IMAGE_STATE_CLEAR                        = 0x130c,
    GR_IMAGE_STATE_RESOLVE_SOURCE               = 0x130d,
    GR_IMAGE_STATE_RESOLVE_DESTINATION          = 0x130e,
    GR_IMAGE_STATE_DISCARD                      = 0x131f,
    GR_IMAGE_STATE_DATA_TRANSFER_SOURCE         = 0x1310,
    GR_IMAGE_STATE_DATA_TRANSFER_DESTINATION    = 0x1311,
} GR_IMAGE_STATE;

typedef enum _GR_IMAGE_TILING
{
    GR_LINEAR_TILING    = 0x1500,
    GR_OPTIMAL_TILING   = 0x1501,
} GR_IMAGE_TILING;

typedef enum _GR_IMAGE_TYPE
{
    GR_IMAGE_1D = 0x1400,
    GR_IMAGE_2D = 0x1401,
    GR_IMAGE_3D = 0x1402,
} GR_IMAGE_TYPE;

typedef enum _GR_IMAGE_VIEW_TYPE
{
    GR_IMAGE_VIEW_1D    = 0x1600,
    GR_IMAGE_VIEW_2D    = 0x1601,
    GR_IMAGE_VIEW_3D    = 0x1602,
    GR_IMAGE_VIEW_CUBE  = 0x1603,
} GR_IMAGE_VIEW_TYPE;

typedef enum _GR_INDEX_TYPE
{
    GR_INDEX_16 = 0x2100,
    GR_INDEX_32 = 0x2101,
} GR_INDEX_TYPE;

typedef enum _GR_INFO_TYPE
{
    GR_INFO_TYPE_PHYSICAL_GPU_PROPERTIES        = 0x6100,
    GR_INFO_TYPE_PHYSICAL_GPU_PERFORMANCE       = 0x6101,
    GR_INFO_TYPE_PHYSICAL_GPU_QUEUE_PROPERTIES  = 0x6102,
    GR_INFO_TYPE_PHYSICAL_GPU_MEMORY_PROPERTIES = 0x6103,
    GR_INFO_TYPE_PHYSICAL_GPU_IMAGE_PROPERTIES  = 0x6104,
    GR_INFO_TYPE_MEMORY_HEAP_PROPERTIES         = 0x6200,
    GR_INFO_TYPE_FORMAT_PROPERTIES              = 0x6300,
    GR_INFO_TYPE_SUBRESOURCE_LAYOUT             = 0x6400,
    GR_INFO_TYPE_MEMORY_REQUIREMENTS            = 0x6800,
    GR_INFO_TYPE_PARENT_DEVICE                  = 0x6801,
    GR_INFO_TYPE_PARENT_PHYSICAL_GPU            = 0x6802,
} GR_INFO_TYPE;

typedef enum _GR_LOGIC_OP
{
    GR_LOGIC_OP_COPY            = 0x2c00,
    GR_LOGIC_OP_CLEAR           = 0x2c01,
    GR_LOGIC_OP_AND             = 0x2c02,
    GR_LOGIC_OP_AND_REVERSE     = 0x2c03,
    GR_LOGIC_OP_AND_INVERTED    = 0x2c04,
    GR_LOGIC_OP_NOOP            = 0x2c05,
    GR_LOGIC_OP_XOR             = 0x2c06,
    GR_LOGIC_OP_OR              = 0x2c07,
    GR_LOGIC_OP_NOR             = 0x2c08,
    GR_LOGIC_OP_EQUIV           = 0x2c09,
    GR_LOGIC_OP_INVERT          = 0x2c0a,
    GR_LOGIC_OP_OR_REVERSE      = 0x2c0b,
    GR_LOGIC_OP_COPY_INVERTED   = 0x2c0c,
    GR_LOGIC_OP_OR_INVERTED     = 0x2c0d,
    GR_LOGIC_OP_NAND            = 0x2c0e,
    GR_LOGIC_OP_SET             = 0x2c0f,
} GR_LOGIC_OP;

typedef enum _GR_MEMORY_PRIORITY
{
    GR_MEMORY_PRIORITY_NORMAL       = 0x1100,
    GR_MEMORY_PRIORITY_HIGH         = 0x1101,
    GR_MEMORY_PRIORITY_LOW          = 0x1102,
    GR_MEMORY_PRIORITY_UNUSED       = 0x1103,
    GR_MEMORY_PRIORITY_VERY_HIGH    = 0x1104,
    GR_MEMORY_PRIORITY_VERY_LOW     = 0x1105,
} GR_MEMORY_PRIORITY;

typedef enum _GR_MEMORY_STATE
{
    GR_MEMORY_STATE_DATA_TRANSFER               = 0x1200,
    GR_MEMORY_STATE_GRAPHICS_SHADER_READ_ONLY   = 0x1201,
    GR_MEMORY_STATE_GRAPHICS_SHADER_WRITE_ONLY  = 0x1202,
    GR_MEMORY_STATE_GRAPHICS_SHADER_READ_WRITE  = 0x1203,
    GR_MEMORY_STATE_COMPUTE_SHADER_READ_ONLY    = 0x1204,
    GR_MEMORY_STATE_COMPUTE_SHADER_WRITE_ONLY   = 0x1205,
    GR_MEMORY_STATE_COMPUTE_SHADER_READ_WRITE   = 0x1206,
    GR_MEMORY_STATE_MULTI_USE_READ_ONLY         = 0x1207,
    GR_MEMORY_STATE_INDEX_DATA                  = 0x1208,
    GR_MEMORY_STATE_INDIRECT_ARG                = 0x1209,
    GR_MEMORY_STATE_WRITE_TIMESTAMP             = 0x120a,
    GR_MEMORY_STATE_QUEUE_ATOMIC                = 0x120b,
    GR_MEMORY_STATE_DISCARD                     = 0x120c,
    GR_MEMORY_STATE_DATA_TRANSFER_SOURCE        = 0x120d,
    GR_MEMORY_STATE_DATA_TRANSFER_DESTINATION   = 0x120e,
} GR_MEMORY_STATE;

typedef enum _GR_NUM_FORMAT
{
    GR_NUM_FMT_UNDEFINED    = 0,
    GR_NUM_FMT_UNORM        = 1,
    GR_NUM_FMT_SNORM        = 2,
    GR_NUM_FMT_UINT         = 3,
    GR_NUM_FMT_SINT         = 4,
    GR_NUM_FMT_FLOAT        = 5,
    GR_NUM_FMT_SRGB         = 6,
    GR_NUM_FMT_DS           = 7,
} GR_NUM_FORMAT;

typedef enum _GR_PHYSICAL_GPU_TYPE
{
    GR_GPU_TYPE_OTHER       = 0x3000,
    GR_GPU_TYPE_INTEGRATED  = 0x3001,
    GR_GPU_TYPE_DISCRETE    = 0x3002,
    GR_GPU_TYPE_VIRTUAL     = 0x3003,
} GR_PHYSICAL_GPU_TYPE;

typedef enum _GR_PIPELINE_BIND_POINT
{
    GR_PIPELINE_BIND_POINT_COMPUTE  = 0x1e00,
    GR_PIPELINE_BIND_POINT_GRAPHICS = 0x1e01,
} GR_PIPELINE_BIND_POINT;

typedef enum _GR_PRIMITIVE_TOPOLOGY
{
    GR_TOPOLOGY_POINT_LIST          = 0x2000,
    GR_TOPOLOGY_LINE_LIST           = 0x2001,
    GR_TOPOLOGY_LINE_STRIP          = 0x2002,
    GR_TOPOLOGY_TRIANGLE_LIST       = 0x2003,
    GR_TOPOLOGY_TRIANGLE_STRIP      = 0x2004,
    GR_TOPOLOGY_RECT_LIST           = 0x2005,
    GR_TOPOLOGY_QUAD_LIST           = 0x2006,
    GR_TOPOLOGY_QUAD_STRIP          = 0x2007,
    GR_TOPOLOGY_LINE_LIST_ADJ       = 0x2008,
    GR_TOPOLOGY_LINE_STRIP_ADJ      = 0x2009,
    GR_TOPOLOGY_TRIANGLE_LIST_ADJ   = 0x200a,
    GR_TOPOLOGY_TRIANGLE_STRIP_ADJ  = 0x200b,
    GR_TOPOLOGY_PATCH               = 0x200c,
} GR_PRIMITIVE_TOPOLOGY;

typedef enum _GR_QUERY_TYPE
{
    GR_QUERY_OCCLUSION              = 0x1a00,
    GR_QUERY_PIPELINE_STATISTICS    = 0x1a01,
} GR_QUERY_TYPE;

typedef enum _GR_QUEUE_TYPE
{
    GR_QUEUE_UNIVERSAL  = 0x1000,
    GR_QUEUE_COMPUTE    = 0x1001,
} GR_QUEUE_TYPE;

typedef enum _GR_RESULT
{
    GR_SUCCESS          = 0x10000,
    GR_UNSUPPORTED,
    GR_NOT_READY,
    GR_TIMEOUT,
    GR_EVENT_SET,
    GR_EVENT_RESET,

    GR_ERROR_UNKNOWN    = 0x11000,
    GR_ERROR_UNAVAILABLE,
    GR_ERROR_INITIALIZATION_FAILED,
    GR_ERROR_OUT_OF_MEMORY,
    GR_ERROR_OUT_OF_GPU_MEMORY,
    GR_ERROR_DEVICE_ALREADY_CREATED,
    GR_ERROR_DEVICE_LOST,
    GR_ERROR_INVALID_POINTER,
    GR_ERROR_INVALID_VALUE,
    GR_ERROR_INVALID_HANDLE,
    GR_ERROR_INVALID_ORDINAL,
    GR_ERROR_INVALID_MEMORY_SIZE,
    GR_ERROR_INVALID_EXTENSION,
    GR_ERROR_INVALID_FLAGS,
    GR_ERROR_INVALID_ALIGNMENT,
    GR_ERROR_INVALID_FORMAT,
    GR_ERROR_INVALID_IMAGE,
    GR_ERROR_INVALID_DESCRIPTOR_SET_DATA,
    GR_ERROR_INVALID_QUEUE_TYPE,
    GR_ERROR_INVALID_OBJECT_TYPE,
    GR_ERROR_UNSUPPORTED_SHADER_IL_VERSION,
    GR_ERROR_BAD_SHADER_CODE,
    GR_ERROR_BAD_PIPELINE_DATA,
    GR_ERROR_TOO_MANY_MEMORY_REFERENCES,
    GR_ERROR_NOT_MAPPABLE,
    GR_ERROR_MEMORY_MAP_FAILED,
    GR_ERROR_MEMORY_UNMAP_FAILED,
    GR_ERROR_INCOMPATIBLE_DEVICE,
    GR_ERROR_INCOMPATIBLE_DRIVER,
    GR_ERROR_INCOMPLETE_COMMAND_BUFFER,
    GR_ERROR_BUILDING_COMMAND_BUFFER,
    GR_ERROR_MEMORY_NOT_BOUND,
    GR_ERROR_INCOMPATIBLE_QUEUE,
    GR_ERROR_NOT_SHAREABLE
} GR_RESULT;

typedef enum _GR_STATE_BIND_POINT
{
    GR_STATE_BIND_VIEWPORT      = 0x1f00,
    GR_STATE_BIND_RASTER        = 0x1f01,
    GR_STATE_BIND_DEPTH_STENCIL = 0x1f02,
    GR_STATE_BIND_COLOR_BLEND   = 0x1f03,
    GR_STATE_BIND_MSAA          = 0x1f04,
} GR_STATE_BIND_POINT;

typedef enum _GR_STENCIL_OP
{
    GR_STENCIL_OP_KEEP      = 0x2b00,
    GR_STENCIL_OP_ZERO      = 0x2b01,
    GR_STENCIL_OP_REPLACE   = 0x2b02,
    GR_STENCIL_OP_INC_CLAMP = 0x2b03,
    GR_STENCIL_OP_DEC_CLAMP = 0x2b04,
    GR_STENCIL_OP_INVERT    = 0x2b05,
    GR_STENCIL_OP_INC_WRAP  = 0x2b06,
    GR_STENCIL_OP_DEC_WRAP  = 0x2b07,
} GR_STENCIL_OP;

typedef enum _GR_SYSTEM_ALLOC_TYPE
{
    GR_SYSTEM_ALLOC_API_OBJECT      = 0x2e00,
    GR_SYSTEM_ALLOC_INTERNAL        = 0x2e01,
    GR_SYSTEM_ALLOC_INTERNAL_TEMP   = 0x2e02,
    GR_SYSTEM_ALLOC_INTERNAL_SHADER = 0x2e03,
    GR_SYSTEM_ALLOC_DEBUG           = 0x2e04,
} GR_SYSTEM_ALLOC_TYPE;

typedef enum _GR_TEX_ADDRESS
{
    GR_TEX_ADDRESS_WRAP         = 0x2400,
    GR_TEX_ADDRESS_MIRROR       = 0x2401,
    GR_TEX_ADDRESS_CLAMP        = 0x2402,
    GR_TEX_ADDRESS_MIRROR_ONCE  = 0x2403,
    GR_TEX_ADDRESS_CLAMP_BORDER = 0x2404,
} GR_TEX_ADDRESS;

typedef enum _GR_TEX_FILTER
{
    GR_TEX_FILTER_MAG_POINT_MIN_POINT_MIP_POINT     = 0x2340,
    GR_TEX_FILTER_MAG_LINEAR_MIN_POINT_MIP_POINT    = 0x2341,
    GR_TEX_FILTER_MAG_POINT_MIN_LINEAR_MIP_POINT    = 0x2344,
    GR_TEX_FILTER_MAG_LINEAR_MIN_LINEAR_MIP_POINT   = 0x2345,
    GR_TEX_FILTER_MAG_POINT_MIN_POINT_MIP_LINEAR    = 0x2380,
    GR_TEX_FILTER_MAG_LINEAR_MIN_POINT_MIP_LINEAR   = 0x2381,
    GR_TEX_FILTER_MAG_POINT_MIN_LINEAR_MIP_LINEAR   = 0x2384,
    GR_TEX_FILTER_MAG_LINEAR_MIN_LINEAR_MIP_LINEAR  = 0x2385,
    GR_TEX_FILTER_ANISOTROPIC                       = 0x238f,
} GR_TEX_FILTER;

typedef enum _GR_TIMESTAMP_TYPE
{
    GR_TIMESTAMP_TOP    = 0x1b00,
    GR_TIMESTAMP_BOTTOM = 0x1b01,
} GR_TIMESTAMP_TYPE;

typedef enum _GR_VALIDATION_LEVEL
{
    GR_VALIDATION_LEVEL_0   = 0x8000,
    GR_VALIDATION_LEVEL_1   = 0x8001,
    GR_VALIDATION_LEVEL_2   = 0x8002,
    GR_VALIDATION_LEVEL_3   = 0x8003,
    GR_VALIDATION_LEVEL_4   = 0x8004,
} GR_VALIDATION_LEVEL;

typedef enum _GR_CMD_BUFFER_BUILD_FLAGS
{
    GR_CMD_BUFFER_OPTIMIZE_GPU_SMALL_BATCH          = 0x00000001,
    GR_CMD_BUFFER_OPTIMIZE_PIPELINE_SWITCH          = 0x00000002,
    GR_CMD_BUFFER_OPTIMIZE_ONE_TIME_SUBMIT          = 0x00000004,
    GR_CMD_BUFFER_OPTIMIZE_DESCRIPTOR_SET_SWITCH    = 0x00000008,
} GR_CMD_BUFFER_BUILD_FLAGS;

typedef enum _GR_DEPTH_STENCIL_VIEW_CREATE_FLAGS
{
    GR_DEPTH_STENCIL_VIEW_CREATE_READ_ONLY_DEPTH    = 0x00000001,
    GR_DEPTH_STENCIL_VIEW_CREATE_READ_ONLY_STENCIL  = 0x00000002,
} GR_DEPTH_STENCIL_VIEW_CREATE_FLAGS;

typedef enum _GR_DEVICE_CREATE_FLAGS
{
    GR_DEVICE_CREATE_VALIDATION = 0x00000001,
} GR_DEVICE_CREATE_FLAGS;

typedef enum _GR_FORMAT_FEATURE_FLAGS
{
    GR_FORMAT_IMAGE_SHADER_READ     = 0x00000001,
    GR_FORMAT_IMAGE_SHADER_WRITE    = 0x00000002,
    GR_FORMAT_IMAGE_COPY            = 0x00000004,
    GR_FORMAT_MEMORY_SHADER_ACCESS  = 0x00000008,
    GR_FORMAT_COLOR_TARGET_WRITE    = 0x00000010,
    GR_FORMAT_COLOR_TARGET_BLEND    = 0x00000020,
    GR_FORMAT_DEPTH_TARGET          = 0x00000040,
    GR_FORMAT_STENCIL_TARGET        = 0x00000080,
    GR_FORMAT_MSAA_TARGET           = 0x00000100,
    GR_FORMAT_CONVERSION            = 0x00000200,
} GR_FORMAT_FEATURE_FLAGS;

typedef enum _GR_GPU_COMPATIBILITY_FLAGS
{
    GR_GPU_COMPAT_ASIC_FEATURES         = 0x00000001,
    GR_GPU_COMPAT_IQ_MATCH              = 0x00000002,
    GR_GPU_COMPAT_PEER_WRITE_TRANSFER   = 0x00000004,
    GR_GPU_COMPAT_SHARED_MEMORY         = 0x00000008,
    GR_GPU_COMPAT_SHARED_SYNC           = 0x00000010,
    GR_GPU_COMPAT_SHARED_GPU0_DISPLAY   = 0x00000020,
    GR_GPU_COMPAT_SHARED_GPU1_DISPLAY   = 0x00000040,
} GR_GPU_COMPATIBILITY_FLAGS;

typedef enum _GR_IMAGE_CREATE_FLAGS
{
    GR_IMAGE_CREATE_INVARIANT_DATA      = 0x00000001,
    GR_IMAGE_CREATE_CLONEABLE           = 0x00000002,
    GR_IMAGE_CREATE_SHAREABLE           = 0x00000004,
    GR_IMAGE_CREATE_VIEW_FORMAT_CHANGE  = 0x00000008,
} GR_IMAGE_CREATE_FLAGS;

typedef enum _GR_IMAGE_USAGE_FLAGS
{
    GR_IMAGE_USAGE_SHADER_ACCESS_READ   = 0x00000001,
    GR_IMAGE_USAGE_SHADER_ACCESS_WRITE  = 0x00000002,
    GR_IMAGE_USAGE_COLOR_TARGET         = 0x00000004,
    GR_IMAGE_USAGE_DEPTH_STENCIL        = 0x00000008,
} GR_IMAGE_USAGE_FLAGS;

typedef enum _GR_MEMORY_ALLOC_FLAGS
{
    GR_MEMORY_ALLOC_VIRTUAL     = 0x00000001,
    GR_MEMORY_ALLOC_SHAREABLE   = 0x00000002,
} GR_MEMORY_ALLOC_FLAGS;

typedef enum _GR_MEMORY_HEAP_FLAGS
{
    GR_MEMORY_HEAP_CPU_VISIBLE          = 0x00000001,
    GR_MEMORY_HEAP_CPU_GPU_COHERENT     = 0x00000002,
    GR_MEMORY_HEAP_CPU_UNCACHED         = 0x00000004,
    GR_MEMORY_HEAP_CPU_WRITE_COMBINED   = 0x00000008,
    GR_MEMORY_HEAP_HOLDS_PINNED         = 0x00000010,
    GR_MEMORY_HEAP_SHAREABLE            = 0x00000020,
} GR_MEMORY_HEAP_FLAGS;

typedef enum _GR_MEMORY_PROPERTY_FLAGS
{
    GR_MEMORY_MIGRATION_SUPPORT         = 0x00000001,
    GR_MEMORY_VIRTUAL_REMAPPING_SUPPORT = 0x00000002,
    GR_MEMORY_PINNING_SUPPORT           = 0x00000004,
    GR_MEMORY_PREFER_GLOBAL_REFS        = 0x00000008,
} GR_MEMORY_PROPERTY_FLAGS;

typedef enum _GR_MEMORY_REF_FLAGS
{
    GR_MEMORY_REF_READ_ONLY = 0x00000001,
} GR_MEMORY_REF_FLAGS;

typedef enum _GR_PIPELINE_CREATE_FLAGS
{
    GR_PIPELINE_CREATE_DISABLE_OPTIMIZATION = 0x00000001,
} GR_PIPELINE_CREATE_FLAGS;

typedef enum _GR_QUERY_CONTROL_FLAGS
{
    GR_QUERY_IMPRECISE_DATA = 0x00000001,
} GR_QUERY_CONTROL_FLAGS;

typedef enum _GR_SEMAPHORE_CREATE_FLAGS
{
    GR_SEMAPHORE_CREATE_SHAREABLE   = 0x00000001,
} GR_SEMAPHORE_CREATE_FLAGS;

typedef enum _GR_SHADER_CREATE_FLAGS
{
    GR_SHADER_CREATE_ALLOW_RE_Z = 0x00000001,
} GR_SHADER_CREATE_FLAGS;

// CALLBACKS

typedef GR_VOID* (GR_STDCALL *GR_ALLOC_FUNCTION)(
    GR_SIZE size,
    GR_SIZE alignment,
    GR_ENUM allocType);

typedef GR_VOID (GR_STDCALL *GR_FREE_FUNCTION)(
    GR_VOID* pMem);

// OFFSET RANGES

typedef struct _GR_EXTENT2D
{
    GR_INT width;
    GR_INT height;
} GR_EXTENT2D;

typedef struct _GR_EXTENT3D
{
    GR_INT width;
    GR_INT height;
    GR_INT depth;
} GR_EXTENT3D;

typedef struct _GR_IMAGE_SUBRESOURCE
{
    GR_ENUM aspect;
    GR_UINT mipLevel;
    GR_UINT arraySlice;
} GR_IMAGE_SUBRESOURCE;

typedef struct _GR_IMAGE_SUBRESOURCE_RANGE
{
    GR_ENUM aspect;
    GR_UINT baseMipLevel;
    GR_UINT mipLevels;
    GR_UINT baseArraySlice;
    GR_UINT arraySize;
} GR_IMAGE_SUBRESOURCE_RANGE;

typedef struct _GR_OFFSET2D
{
    GR_INT x;
    GR_INT y;
} GR_OFFSET2D;

typedef struct _GR_OFFSET3D
{
    GR_INT x;
    GR_INT y;
    GR_INT z;
} GR_OFFSET3D;

// DATA STRUCTURES

typedef struct _GR_ALLOC_CALLBACKS
{
    GR_ALLOC_FUNCTION pfnAlloc;
    GR_FREE_FUNCTION pfnFree;
} GR_ALLOC_CALLBACKS;

typedef struct _GR_APPLICATION_INFO
{
    const GR_CHAR* pAppName;
    GR_UINT32 appVersion;
    const GR_CHAR* pEngineName;
    GR_UINT32 engineVersion;
    GR_UINT32 apiVersion;
} GR_APPLICATION_INFO;

typedef struct _GR_CHANNEL_MAPPING
{
    GR_ENUM r;
    GR_ENUM g;
    GR_ENUM b;
    GR_ENUM a;
} GR_CHANNEL_MAPPING;

typedef struct _GR_COLOR_TARGET_BIND_INFO
{
    GR_COLOR_TARGET_VIEW view;
    GR_ENUM colorTargetState;
} GR_COLOR_TARGET_BIND_INFO;

typedef struct _GR_COLOR_TARGET_BLEND_STATE
{
    GR_BOOL blendEnable;
    GR_ENUM srcBlendColor;
    GR_ENUM destBlendColor;
    GR_ENUM blendFuncColor;
    GR_ENUM srcBlendAlpha;
    GR_ENUM destBlendAlpha;
    GR_ENUM blendFuncAlpha;
} GR_COLOR_TARGET_BLEND_STATE;

typedef struct _GR_DEPTH_STENCIL_BIND_INFO
{
    GR_DEPTH_STENCIL_VIEW view;
    GR_ENUM depthState;
    GR_ENUM stencilState;
} GR_DEPTH_STENCIL_BIND_INFO;

typedef struct _GR_DEPTH_STENCIL_OP
{
    GR_ENUM stencilFailOp;
    GR_ENUM stencilPassOp;
    GR_ENUM stencilDepthFailOp;
    GR_ENUM stencilFunc;
    GR_UINT8 stencilRef;
} GR_DEPTH_STENCIL_OP;

typedef struct _GR_DESCRIPTOR_SET_ATTACH_INFO
{
    GR_DESCRIPTOR_SET descriptorSet;
    GR_UINT slotOffset;
} GR_DESCRIPTOR_SET_ATTACH_INFO;

typedef struct _GR_DESCRIPTOR_SLOT_INFO
{
    GR_ENUM slotObjectType;
    union
    {
        GR_UINT shaderEntityIndex;
        const struct _GR_DESCRIPTOR_SET_MAPPING* pNextLevelSet;
    };
} GR_DESCRIPTOR_SLOT_INFO;

typedef struct _GR_DESCRIPTOR_SET_MAPPING
{
    GR_UINT descriptorCount;
    const GR_DESCRIPTOR_SLOT_INFO* pDescriptorInfo;
} GR_DESCRIPTOR_SET_MAPPING;

typedef struct _GR_DISPATCH_INDIRECT_ARG
{
    GR_UINT32 x;
    GR_UINT32 y;
    GR_UINT32 z;
} GR_DISPATCH_INDIRECT_ARG;

typedef struct _GR_DRAW_INDEXED_INDIRECT_ARG
{
    GR_UINT32 indexCount;
    GR_UINT32 instanceCount;
    GR_UINT32 firstIndex;
    GR_INT32 vertexOffset;
    GR_UINT32 firstInstance;
} GR_DRAW_INDEXED_INDIRECT_ARG;

typedef struct _GR_DRAW_INDIRECT_ARG
{
    GR_UINT32 vertexCount;
    GR_UINT32 instanceCount;
    GR_UINT32 firstVertex;
    GR_UINT32 firstInstance;
} GR_DRAW_INDIRECT_ARG;

typedef struct _GR_DYNAMIC_MEMORY_VIEW_SLOT_INFO
{
    GR_ENUM slotObjectType;
    GR_UINT shaderEntityIndex;
} GR_DYNAMIC_MEMORY_VIEW_SLOT_INFO;

typedef struct _GR_FORMAT
{
    GR_UINT32 channelFormat : 16;
    GR_UINT32 numericFormat : 16;
} GR_FORMAT;

typedef struct _GR_FORMAT_PROPERTIES
{
    GR_FLAGS linearTilingFeatures;
    GR_FLAGS optimalTilingFeatures;
} GR_FORMAT_PROPERTIES;

typedef struct _GR_GPU_COMPATIBILITY_INFO
{
    GR_FLAGS compatibilityFlags;
} GR_GPU_COMPATIBILITY_INFO;

typedef struct _GR_IMAGE_COPY
{
    GR_IMAGE_SUBRESOURCE srcSubresource;
    GR_OFFSET3D srcOffset;
    GR_IMAGE_SUBRESOURCE destSubresource;
    GR_OFFSET3D destOffset;
    GR_EXTENT3D extent;
} GR_IMAGE_COPY;

typedef struct _GR_IMAGE_RESOLVE
{
    GR_IMAGE_SUBRESOURCE srcSubresource;
    GR_OFFSET2D srcOffset;
    GR_IMAGE_SUBRESOURCE destSubresource;
    GR_OFFSET2D destOffset;
    GR_EXTENT2D extent;
} GR_IMAGE_RESOLVE;

typedef struct _GR_IMAGE_STATE_TRANSITION
{
    GR_IMAGE image;
    GR_ENUM oldState;
    GR_ENUM newState;
    GR_IMAGE_SUBRESOURCE_RANGE subresourceRange;
} GR_IMAGE_STATE_TRANSITION;

typedef struct _GR_IMAGE_VIEW_ATTACH_INFO
{
    GR_IMAGE_VIEW view;
    GR_ENUM state;
} GR_IMAGE_VIEW_ATTACH_INFO;

typedef struct _GR_LINK_CONST_BUFFER
{
    GR_UINT bufferId;
    GR_SIZE bufferSize;
    const GR_VOID* pBufferData;
} GR_LINK_CONST_BUFFER;

typedef struct _GR_MEMORY_ALLOC_INFO
{
    GR_GPU_SIZE size;
    GR_GPU_SIZE alignment;
    GR_FLAGS flags;
    GR_UINT heapCount;
    GR_UINT heaps[GR_MAX_MEMORY_HEAPS];
    GR_ENUM memPriority;
} GR_MEMORY_ALLOC_INFO;

typedef struct _GR_MEMORY_COPY
{
    GR_GPU_SIZE srcOffset;
    GR_GPU_SIZE destOffset;
    GR_GPU_SIZE copySize;
} GR_MEMORY_COPY;

typedef struct _GR_MEMORY_HEAP_PROPERTIES
{
    GR_ENUM heapMemoryType;
    GR_GPU_SIZE heapSize;
    GR_GPU_SIZE pageSize;
    GR_FLAGS flags;
    GR_FLOAT gpuReadPerfRating;
    GR_FLOAT gpuWritePerfRating;
    GR_FLOAT cpuReadPerfRating;
    GR_FLOAT cpuWritePerfRating;
} GR_MEMORY_HEAP_PROPERTIES;

typedef struct _GR_MEMORY_IMAGE_COPY
{
    GR_GPU_SIZE memOffset;
    GR_IMAGE_SUBRESOURCE imageSubresource;
    GR_OFFSET3D imageOffset;
    GR_EXTENT3D imageExtent;
} GR_MEMORY_IMAGE_COPY;

typedef struct _GR_MEMORY_OPEN_INFO
{
    GR_GPU_MEMORY sharedMem;
} GR_MEMORY_OPEN_INFO;

typedef struct _GR_MEMORY_REF
{
    GR_GPU_MEMORY mem;
    GR_FLAGS flags;
} GR_MEMORY_REF;

typedef struct _GR_MEMORY_REQUIREMENTS
{
    GR_GPU_SIZE size;
    GR_GPU_SIZE alignment;
    GR_UINT heapCount;
    GR_UINT heaps[GR_MAX_MEMORY_HEAPS];
} GR_MEMORY_REQUIREMENTS;

typedef struct _GR_MEMORY_STATE_TRANSITION
{
    GR_GPU_MEMORY mem;
    GR_ENUM oldState;
    GR_ENUM newState;
    GR_GPU_SIZE offset;
    GR_GPU_SIZE regionSize;
} GR_MEMORY_STATE_TRANSITION;

typedef struct _GR_MEMORY_VIEW_ATTACH_INFO
{
    GR_GPU_MEMORY mem;
    GR_GPU_SIZE offset;
    GR_GPU_SIZE range;
    GR_GPU_SIZE stride;
    GR_FORMAT format;
    GR_ENUM state;
} GR_MEMORY_VIEW_ATTACH_INFO;

typedef struct _GR_PARENT_DEVICE
{
    GR_DEVICE device;
} GR_PARENT_DEVICE;

typedef struct _GR_PARENT_PHYSICAL_GPU
{
    GR_PHYSICAL_GPU gpu;
} GR_PARENT_PHYSICAL_GPU;

typedef struct _GR_PEER_IMAGE_OPEN_INFO
{
    GR_IMAGE originalImage;
} GR_PEER_IMAGE_OPEN_INFO;

typedef struct _GR_PEER_MEMORY_OPEN_INFO
{
    GR_GPU_MEMORY originalMem;
} GR_PEER_MEMORY_OPEN_INFO;

typedef struct _GR_PHYSICAL_GPU_IMAGE_PROPERTIES
{
    GR_UINT maxSliceWidth;
    GR_UINT maxSliceHeight;
    GR_UINT maxDepth;
    GR_UINT maxArraySlices;
    GR_UINT reserved1;
    GR_UINT reserved2;
    GR_GPU_SIZE maxMemoryAlignment;
    GR_UINT32 sparseImageSupportLevel;
    GR_FLAGS flags;
} GR_PHYSICAL_GPU_IMAGE_PROPERTIES;

typedef struct _GR_PHYSICAL_GPU_MEMORY_PROPERTIES
{
    GR_FLAGS flags;
    GR_GPU_SIZE virtualMemPageSize;
    GR_GPU_SIZE maxVirtualMemSize;
    GR_GPU_SIZE maxPhysicalMemSize;
} GR_PHYSICAL_GPU_MEMORY_PROPERTIES;

typedef struct _GR_PHYSICAL_GPU_PERFORMANCE
{
    GR_FLOAT maxGpuClock;
    GR_FLOAT aluPerClock;
    GR_FLOAT texPerClock;
    GR_FLOAT primsPerClock;
    GR_FLOAT pixelsPerClock;
} GR_PHYSICAL_GPU_PERFORMANCE;

typedef struct _GR_PHYSICAL_GPU_PROPERTIES
{
    GR_UINT32 apiVersion;
    GR_UINT32 driverVersion;
    GR_UINT32 vendorId;
    GR_UINT32 deviceId;
    GR_ENUM gpuType;
    GR_CHAR gpuName[GR_MAX_PHYSICAL_GPU_NAME];
    GR_UINT maxMemRefsPerSubmission;
    GR_GPU_SIZE reserved;
    GR_GPU_SIZE maxInlineMemoryUpdateSize;
    GR_UINT maxBoundDescriptorSets;
    GR_UINT maxThreadGroupSize;
    GR_UINT64 timestampFrequency;
    GR_BOOL multiColorTargetClears;
} GR_PHYSICAL_GPU_PROPERTIES;

typedef struct _GR_PHYSICAL_GPU_QUEUE_PROPERTIES
{
    GR_ENUM queueType;
    GR_UINT queueCount;
    GR_UINT maxAtomicCounters;
    GR_BOOL supportsTimestamps;
} GR_PHYSICAL_GPU_QUEUE_PROPERTIES;

typedef struct _GR_PIPELINE_CB_TARGET_STATE
{
    GR_BOOL blendEnable;
    GR_FORMAT format;
    GR_UINT8 channelWriteMask;
} GR_PIPELINE_CB_TARGET_STATE;

typedef struct _GR_PIPELINE_CB_STATE
{
    GR_BOOL alphaToCoverageEnable;
    GR_BOOL dualSourceBlendEnable;
    GR_ENUM logicOp;
    GR_PIPELINE_CB_TARGET_STATE target[GR_MAX_COLOR_TARGETS];
} GR_PIPELINE_CB_STATE;

typedef struct _GR_PIPELINE_DB_STATE
{
    GR_FORMAT format;
} GR_PIPELINE_DB_STATE;

typedef struct _GR_PIPELINE_IA_STATE
{
    GR_ENUM topology;
    GR_BOOL disableVertexReuse;
} GR_PIPELINE_IA_STATE;

typedef struct _GR_PIPELINE_RS_STATE
{
    GR_BOOL depthClipEnable;
} GR_PIPELINE_RS_STATE;

typedef struct _GR_PIPELINE_SHADER
{
    GR_SHADER shader;
    GR_DESCRIPTOR_SET_MAPPING descriptorSetMapping[GR_MAX_DESCRIPTOR_SETS];
    GR_UINT linkConstBufferCount;
    const GR_LINK_CONST_BUFFER* pLinkConstBufferInfo;
    GR_DYNAMIC_MEMORY_VIEW_SLOT_INFO dynamicMemoryViewMapping;
} GR_PIPELINE_SHADER;

typedef struct _GR_PIPELINE_STATISTICS_DATA
{
    GR_UINT64 psInvocations;
    GR_UINT64 cPrimitives;
    GR_UINT64 cInvocations;
    GR_UINT64 vsInvocations;
    GR_UINT64 gsInvocations;
    GR_UINT64 gsPrimitives;
    GR_UINT64 iaPrimitives;
    GR_UINT64 iaVertices;
    GR_UINT64 hsInvocations;
    GR_UINT64 dsInvocations;
    GR_UINT64 csInvocations;
} GR_PIPELINE_STATISTICS_DATA;

typedef struct _GR_PIPELINE_TESS_STATE
{
    GR_UINT patchControlPoints;
    GR_FLOAT optimalTessFactor;
} GR_PIPELINE_TESS_STATE;

typedef struct _GR_QUEUE_SEMAPHORE_OPEN_INFO
{
    GR_QUEUE_SEMAPHORE sharedSemaphore;
} GR_QUEUE_SEMAPHORE_OPEN_INFO;

typedef struct _GR_RECT
{
    GR_OFFSET2D offset;
    GR_EXTENT2D extent;
} GR_RECT;

typedef struct _GR_SUBRESOURCE_LAYOUT
{
    GR_GPU_SIZE offset;
    GR_GPU_SIZE size;
    GR_GPU_SIZE rowPitch;
    GR_GPU_SIZE depthPitch;
} GR_SUBRESOURCE_LAYOUT;

typedef struct _GR_VIEWPORT
{
    GR_FLOAT originX;
    GR_FLOAT originY;
    GR_FLOAT width;
    GR_FLOAT height;
    GR_FLOAT minDepth;
    GR_FLOAT maxDepth;
} GR_VIEWPORT;

typedef struct _GR_VIRTUAL_MEMORY_REMAP_RANGE
{
    GR_GPU_MEMORY virtualMem;
    GR_GPU_SIZE virtualStartPage;
    GR_GPU_MEMORY realMem;
    GR_GPU_SIZE realStartPage;
    GR_GPU_SIZE pageCount;
} GR_VIRTUAL_MEMORY_REMAP_RANGE;

// CREATE INFO

typedef struct _GR_CMD_BUFFER_CREATE_INFO
{
    GR_ENUM queueType;
    GR_FLAGS flags;
} GR_CMD_BUFFER_CREATE_INFO;

typedef struct _GR_COLOR_BLEND_STATE_CREATE_INFO
{
    GR_COLOR_TARGET_BLEND_STATE target[GR_MAX_COLOR_TARGETS];
    GR_FLOAT blendConst[4];
} GR_COLOR_BLEND_STATE_CREATE_INFO;

typedef struct _GR_COLOR_TARGET_VIEW_CREATE_INFO
{
    GR_IMAGE image;
    GR_FORMAT format;
    GR_UINT mipLevel;
    GR_UINT baseArraySlice;
    GR_UINT arraySize;
} GR_COLOR_TARGET_VIEW_CREATE_INFO;

typedef struct _GR_COMPUTE_PIPELINE_CREATE_INFO
{
    GR_PIPELINE_SHADER cs;
    GR_FLAGS flags;
} GR_COMPUTE_PIPELINE_CREATE_INFO;

typedef struct _GR_DEPTH_STENCIL_STATE_CREATE_INFO
{
    GR_BOOL depthEnable;
    GR_BOOL depthWriteEnable;
    GR_ENUM depthFunc;
    GR_BOOL depthBoundsEnable;
    GR_FLOAT minDepth;
    GR_FLOAT maxDepth;
    GR_BOOL stencilEnable;
    GR_UINT8 stencilReadMask;
    GR_UINT8 stencilWriteMask;
    GR_DEPTH_STENCIL_OP front;
    GR_DEPTH_STENCIL_OP back;
} GR_DEPTH_STENCIL_STATE_CREATE_INFO;

typedef struct _GR_DEPTH_STENCIL_VIEW_CREATE_INFO
{
    GR_IMAGE image;
    GR_UINT mipLevel;
    GR_UINT baseArraySlice;
    GR_UINT arraySize;
    GR_FLAGS flags;
} GR_DEPTH_STENCIL_VIEW_CREATE_INFO;

typedef struct _GR_DESCRIPTOR_SET_CREATE_INFO
{
    GR_UINT slots;
} GR_DESCRIPTOR_SET_CREATE_INFO;

typedef struct _GR_DEVICE_QUEUE_CREATE_INFO
{
    GR_ENUM queueType;
    GR_UINT queueCount;
} GR_DEVICE_QUEUE_CREATE_INFO;

typedef struct _GR_DEVICE_CREATE_INFO
{
    GR_UINT queueRecordCount;
    const GR_DEVICE_QUEUE_CREATE_INFO* pRequestedQueues;
    GR_UINT extensionCount;
    const GR_CHAR* const* ppEnabledExtensionNames;
    GR_ENUM maxValidationLevel;
    GR_FLAGS flags;
} GR_DEVICE_CREATE_INFO;

typedef struct _GR_EVENT_CREATE_INFO
{
    GR_FLAGS flags;
} GR_EVENT_CREATE_INFO;

typedef struct _GR_FENCE_CREATE_INFO
{
    GR_FLAGS flags;
} GR_FENCE_CREATE_INFO;

typedef struct _GR_GRAPHICS_PIPELINE_CREATE_INFO
{
    GR_PIPELINE_SHADER vs;
    GR_PIPELINE_SHADER hs;
    GR_PIPELINE_SHADER ds;
    GR_PIPELINE_SHADER gs;
    GR_PIPELINE_SHADER ps;
    GR_PIPELINE_IA_STATE iaState;
    GR_PIPELINE_TESS_STATE tessState;
    GR_PIPELINE_RS_STATE rsState;
    GR_PIPELINE_CB_STATE cbState;
    GR_PIPELINE_DB_STATE dbState;
    GR_FLAGS flags;
} GR_GRAPHICS_PIPELINE_CREATE_INFO;

typedef struct _GR_IMAGE_CREATE_INFO
{
    GR_ENUM imageType;
    GR_FORMAT format;
    GR_EXTENT3D extent;
    GR_UINT mipLevels;
    GR_UINT arraySize;
    GR_UINT samples;
    GR_ENUM tiling;
    GR_FLAGS usage;
    GR_FLAGS flags;
} GR_IMAGE_CREATE_INFO;

typedef struct _GR_IMAGE_VIEW_CREATE_INFO
{
    GR_IMAGE image;
    GR_ENUM viewType;
    GR_FORMAT format;
    GR_CHANNEL_MAPPING channels;
    GR_IMAGE_SUBRESOURCE_RANGE subresourceRange;
    GR_FLOAT minLod;
} GR_IMAGE_VIEW_CREATE_INFO;

typedef struct _GR_MSAA_STATE_CREATE_INFO
{
    GR_UINT samples;
    GR_SAMPLE_MASK sampleMask;
} GR_MSAA_STATE_CREATE_INFO;

typedef struct _GR_QUERY_POOL_CREATE_INFO
{
    GR_ENUM queryType;
    GR_UINT slots;
} GR_QUERY_POOL_CREATE_INFO;

typedef struct _GR_QUEUE_SEMAPHORE_CREATE_INFO
{
    GR_UINT initialCount;
    GR_FLAGS flags;
} GR_QUEUE_SEMAPHORE_CREATE_INFO;

typedef struct _GR_RASTER_STATE_CREATE_INFO
{
    GR_ENUM fillMode;
    GR_ENUM cullMode;
    GR_ENUM frontFace;
    GR_INT depthBias;
    GR_FLOAT depthBiasClamp;
    GR_FLOAT slopeScaledDepthBias;
} GR_RASTER_STATE_CREATE_INFO;

typedef struct _GR_SAMPLER_CREATE_INFO
{
    GR_ENUM filter;
    GR_ENUM addressU;
    GR_ENUM addressV;
    GR_ENUM addressW;
    GR_FLOAT mipLodBias;
    GR_UINT maxAnisotropy;
    GR_ENUM compareFunc;
    GR_FLOAT minLod;
    GR_FLOAT maxLod;
    GR_ENUM borderColor;
} GR_SAMPLER_CREATE_INFO;

typedef struct _GR_SHADER_CREATE_INFO
{
    GR_SIZE codeSize;
    const GR_VOID* pCode;
    GR_FLAGS flags;
} GR_SHADER_CREATE_INFO;

typedef struct _GR_VIEWPORT_STATE_CREATE_INFO
{
    GR_UINT viewportCount;
    GR_BOOL scissorEnable;
    GR_VIEWPORT viewports[GR_MAX_VIEWPORTS];
    GR_RECT scissors[GR_MAX_VIEWPORTS];
} GR_VIEWPORT_STATE_CREATE_INFO;

// FUNCTIONS

// INITIALIZATION AND DEVICE FUNCTIONS
typedef GR_RESULT (GR_STDCALL *PFN_grInitAndEnumerateGpus)(const GR_APPLICATION_INFO* pAppInfo, const GR_ALLOC_CALLBACKS* pAllocCb, GR_UINT* pGpuCount, GR_PHYSICAL_GPU gpus[GR_MAX_PHYSICAL_GPUS]);
typedef GR_RESULT (GR_STDCALL *PFN_grGetGpuInfo)(GR_PHYSICAL_GPU gpu, GR_ENUM infoType, GR_SIZE* pDataSize, GR_VOID* pData);
typedef GR_RESULT (GR_STDCALL *PFN_grCreateDevice)(GR_PHYSICAL_GPU gpu, const GR_DEVICE_CREATE_INFO* pCreateInfo, GR_DEVICE* pDevice);
typedef GR_RESULT (GR_STDCALL *PFN_grDestroyDevice)(GR_DEVICE device);

// EXTENSION DISCOVERY FUNCTIONS
typedef GR_RESULT (GR_STDCALL *PFN_grGetExtensionSupport)(GR_PHYSICAL_GPU gpu, const GR_CHAR* pExtName);

// QUEUE FUNCTIONS
typedef GR_RESULT (GR_STDCALL *PFN_grGetDeviceQueue)(GR_DEVICE device, GR_ENUM queueType, GR_UINT queueId, GR_QUEUE* pQueue);
typedef GR_RESULT (GR_STDCALL *PFN_grQueueWaitIdle)(GR_QUEUE queue);
typedef GR_RESULT (GR_STDCALL *PFN_grDeviceWaitIdle)(GR_QUEUE queue);
typedef GR_RESULT (GR_STDCALL *PFN_grQueueSubmit)(GR_QUEUE queue, GR_UINT cmdBufferCount, const GR_CMD_BUFFER* pCmdBuffers, GR_UINT memRefCount, const GR_MEMORY_REF* pMemRefs, GR_FENCE fence);
typedef GR_RESULT (GR_STDCALL *PFN_grQueueSetGlobalMemReferences)(GR_QUEUE queue, GR_UINT memRefCount, const GR_MEMORY_REF* pMemRefs);

// MEMORY MANAGEMENT FUNCTIONS
typedef GR_RESULT (GR_STDCALL *PFN_grGetMemoryHeapCount)(GR_DEVICE device, GR_UINT* pCount);
typedef GR_RESULT (GR_STDCALL *PFN_grGetMemoryHeapInfo)(GR_DEVICE device, GR_UINT heapId, GR_ENUM infoType, GR_SIZE* pDataSize, GR_VOID* pData);
typedef GR_RESULT (GR_STDCALL *PFN_grAllocMemory)(GR_DEVICE device, const GR_MEMORY_ALLOC_INFO* pAllocInfo, GR_GPU_MEMORY* pMem);
typedef GR_RESULT (GR_STDCALL *PFN_grFreeMemory)(GR_GPU_MEMORY mem);
typedef GR_RESULT (GR_STDCALL *PFN_grSetMemoryPriority)(GR_GPU_MEMORY mem, GR_ENUM priority);
typedef GR_RESULT (GR_STDCALL *PFN_grMapMemory)(GR_GPU_MEMORY mem, GR_FLAGS flags, GR_VOID** ppData);
typedef GR_RESULT (GR_STDCALL *PFN_grUnmapMemory)(GR_GPU_MEMORY mem);
typedef GR_RESULT (GR_STDCALL *PFN_grRemapVirtualMemoryPages)(GR_DEVICE device, GR_UINT rangeCount, const GR_VIRTUAL_MEMORY_REMAP_RANGE* pRanges, GR_UINT preWaitSemaphoreCount, const GR_QUEUE_SEMAPHORE* pPreWaitSemaphores, GR_UINT postSignalSemaphoreCount, const GR_QUEUE_SEMAPHORE* pPostSignalSemaphores);
typedef GR_RESULT (GR_STDCALL *PFN_grPinSystemMemory)(GR_DEVICE device, const GR_VOID* pSysMem, GR_SIZE memSize, GR_GPU_MEMORY* pMem);

// GENERIC API OBJECT MANAGEMENT FUNCTIONS
typedef GR_RESULT (GR_STDCALL *PFN_grDestroyObject)(GR_OBJECT object);
typedef GR_RESULT (GR_STDCALL *PFN_grGetObjectInfo)(GR_BASE_OBJECT object, GR_ENUM infoType, GR_SIZE* pDataSize, GR_VOID* pData);
typedef GR_RESULT (GR_STDCALL *PFN_grBindObjectMemory)(GR_OBJECT object, GR_GPU_MEMORY mem, GR_GPU_SIZE offset);

// IMAGE AND SAMPLER FUNCTIONS
typedef GR_RESULT (GR_STDCALL *PFN_grGetFormatInfo)(GR_DEVICE device, GR_FORMAT format, GR_ENUM infoType, GR_SIZE* pDataSize, GR_VOID* pData);
typedef GR_RESULT (GR_STDCALL *PFN_grCreateImage)(GR_DEVICE device, const GR_IMAGE_CREATE_INFO* pCreateInfo, GR_IMAGE* pImage);
typedef GR_RESULT (GR_STDCALL *PFN_grGetImageSubresourceInfo)(GR_IMAGE image, const GR_IMAGE_SUBRESOURCE* pSubresource, GR_ENUM infoType, GR_SIZE* pDataSize, GR_VOID* pData);
typedef GR_RESULT (GR_STDCALL *PFN_grCreateSampler)(GR_DEVICE device, const GR_SAMPLER_CREATE_INFO* pCreateInfo, GR_SAMPLER* pSampler);

// IMAGE VIEW FUNCTIONS
typedef GR_RESULT (GR_STDCALL *PFN_grCreateImageView)(GR_DEVICE device, const GR_IMAGE_VIEW_CREATE_INFO* pCreateInfo, GR_IMAGE_VIEW* pView);
typedef GR_RESULT (GR_STDCALL *PFN_grCreateColorTargetView)(GR_DEVICE device, const GR_COLOR_TARGET_VIEW_CREATE_INFO* pCreateInfo, GR_COLOR_TARGET_VIEW* pView);

// SHADER AND PIPELINE FUNCTIONS
typedef GR_RESULT (GR_STDCALL *PFN_grCreateShader)(GR_DEVICE device, const GR_SHADER_CREATE_INFO* pCreateInfo, GR_SHADER* pShader);
typedef GR_RESULT (GR_STDCALL *PFN_grCreateGraphicsPipeline)(GR_DEVICE device, const GR_GRAPHICS_PIPELINE_CREATE_INFO* pCreateInfo, GR_PIPELINE* pPipeline);
typedef GR_RESULT (GR_STDCALL *PFN_grCreateComputePipeline)(GR_DEVICE device, const GR_COMPUTE_PIPELINE_CREATE_INFO* pCreateInfo, GR_PIPELINE* pPipeline);
typedef GR_RESULT (GR_STDCALL *PFN_grStorePipeline)(GR_PIPELINE pipeline, GR_SIZE* pDataSize, GR_VOID* pData);
typedef GR_RESULT (GR_STDCALL *PFN_grLoadPipeline)(GR_DEVICE device, GR_SIZE dataSize, const GR_VOID* pData, GR_PIPELINE* pPipeline);

// DESCRIPTOR SET FUNCTIONS
typedef GR_RESULT (GR_STDCALL *PFN_grCreateDescriptorSet)(GR_DEVICE device, const GR_DESCRIPTOR_SET_CREATE_INFO* pCreateInfo, GR_DESCRIPTOR_SET* pDescriptorSet);
typedef GR_VOID (GR_STDCALL *PFN_grBeginDescriptorSetUpdate)(GR_DESCRIPTOR_SET descriptorSet);
typedef GR_VOID (GR_STDCALL *PFN_grEndDescriptorSetUpdate)(GR_DESCRIPTOR_SET descriptorSet);
typedef GR_VOID (GR_STDCALL *PFN_grAttachSamplerDescriptors)(GR_DESCRIPTOR_SET descriptorSet, GR_UINT startSlot, GR_UINT slotCount, const GR_SAMPLER* pSamplers);
typedef GR_VOID (GR_STDCALL *PFN_grAttachImageViewDescriptors)(GR_DESCRIPTOR_SET descriptorSet, GR_UINT startSlot, GR_UINT slotCount, const GR_IMAGE_VIEW_ATTACH_INFO* pImageViews);
typedef GR_VOID (GR_STDCALL *PFN_grAttachMemoryViewDescriptors)(GR_DESCRIPTOR_SET descriptorSet, GR_UINT startSlot, GR_UINT slotCount, const GR_MEMORY_VIEW_ATTACH_INFO* pMemViews);
typedef GR_VOID (GR_STDCALL *PFN_grAttachNestedDescriptors)(GR_DESCRIPTOR_SET descriptorSet, GR_UINT startSlot, GR_UINT slotCount, const GR_DESCRIPTOR_SET_ATTACH_INFO* pNestedDescriptorSets);
typedef GR_VOID (GR_STDCALL *PFN_grClearDescriptorSetSlots)(GR_DESCRIPTOR_SET descriptorSet, GR_UINT startSlot, GR_UINT slotCount);

// STATE OBJECT FUNCTIONS
typedef GR_RESULT (GR_STDCALL *PFN_grCreateViewportState)(GR_DEVICE device, const GR_VIEWPORT_STATE_CREATE_INFO* pCreateInfo, GR_VIEWPORT_STATE_OBJECT* pState);
typedef GR_RESULT (GR_STDCALL *PFN_grCreateRasterState)(GR_DEVICE device, const GR_RASTER_STATE_CREATE_INFO* pCreateInfo, GR_RASTER_STATE_OBJECT* pState);
typedef GR_RESULT (GR_STDCALL *PFN_grCreateColorBlendState)(GR_DEVICE device, const GR_COLOR_BLEND_STATE_CREATE_INFO* pCreateInfo, GR_COLOR_BLEND_STATE_OBJECT* pState);
typedef GR_RESULT (GR_STDCALL *PFN_grCreateDepthStencilState)(GR_DEVICE device, const GR_DEPTH_STENCIL_STATE_CREATE_INFO* pCreateInfo, GR_DEPTH_STENCIL_STATE_OBJECT* pState);
typedef GR_RESULT (GR_STDCALL *PFN_grCreateMsaaState)(GR_DEVICE device, const GR_MSAA_STATE_CREATE_INFO* pCreateInfo, GR_MSAA_STATE_OBJECT* pState);

// QUERY AND SYNCHRONIZATION FUNCTION
typedef GR_RESULT (GR_STDCALL *PFN_grCreateQueryPool)(GR_DEVICE device, const GR_QUERY_POOL_CREATE_INFO* pCreateInfo, GR_QUERY_POOL* pQueryPool);
typedef GR_RESULT (GR_STDCALL *PFN_grGetQueryPoolResults)(GR_QUERY_POOL queryPool, GR_UINT startQuery, GR_UINT queryCount, GR_SIZE* pDataSize, GR_VOID* pData);
typedef GR_RESULT (GR_STDCALL *PFN_grCreateFence)(GR_DEVICE device, const GR_FENCE_CREATE_INFO* pCreateInfo, GR_FENCE* pFence);
typedef GR_RESULT (GR_STDCALL *PFN_grGetFenceStatus)(GR_FENCE fence);
typedef GR_RESULT (GR_STDCALL *PFN_grWaitForFences)(GR_DEVICE device, GR_UINT fenceCount, const GR_FENCE* pFences, GR_BOOL waitAll, GR_FLOAT timeout);
typedef GR_RESULT (GR_STDCALL *PFN_grCreateQueueSemaphore)(GR_DEVICE device, const GR_QUEUE_SEMAPHORE_CREATE_INFO* pCreateInfo, GR_QUEUE_SEMAPHORE* pSemaphore);
typedef GR_RESULT (GR_STDCALL *PFN_grSignalQueueSemaphore)(GR_QUEUE queue, GR_QUEUE_SEMAPHORE semaphore);
typedef GR_RESULT (GR_STDCALL *PFN_grWaitQueueSemaphore)(GR_QUEUE queue, GR_QUEUE_SEMAPHORE semaphore);
typedef GR_RESULT (GR_STDCALL *PFN_grCreateEvent)(GR_DEVICE device, const GR_EVENT_CREATE_INFO* pCreateInfo, GR_EVENT* pEvent);
typedef GR_RESULT (GR_STDCALL *PFN_grGetEventStatus)(GR_EVENT event);
typedef GR_RESULT (GR_STDCALL *PFN_grSetEvent)(GR_EVENT event);
typedef GR_RESULT (GR_STDCALL *PFN_grResetEvent)(GR_EVENT event);

// MULTI-DEVICE MANAGEMENT FUNCTIONS
typedef GR_RESULT (GR_STDCALL *PFN_grGetMultiGpuCompatibility)(GR_PHYSICAL_GPU gpu0, GR_PHYSICAL_GPU gpu1, GR_GPU_COMPATIBILITY_INFO* pInfo);
typedef GR_RESULT (GR_STDCALL *PFN_grOpenSharedMemory)(GR_DEVICE device, const GR_MEMORY_OPEN_INFO* pOpenInfo, GR_GPU_MEMORY* pMem);
typedef GR_RESULT (GR_STDCALL *PFN_grOpenSharedQueueSemaphore)(GR_DEVICE device, const GR_QUEUE_SEMAPHORE_OPEN_INFO* pOpenInfo, GR_QUEUE_SEMAPHORE* pSemaphore);
typedef GR_RESULT (GR_STDCALL *PFN_grOpenPeerMemory)(GR_DEVICE device, const GR_PEER_MEMORY_OPEN_INFO* pOpenInfo, GR_GPU_MEMORY* pMem);
typedef GR_RESULT (GR_STDCALL *PFN_grOpenPeerImage)(GR_DEVICE device, const GR_PEER_IMAGE_OPEN_INFO* pOpenInfo, GR_IMAGE* pImage, GR_GPU_MEMORY* pMem);

// COMMAND BUFFER MANAGEMENT FUNCTIONS
typedef GR_RESULT (GR_STDCALL *PFN_grCreateCommandBuffer)(GR_DEVICE device, const GR_CMD_BUFFER_CREATE_INFO* pCreateInfo, GR_CMD_BUFFER* pCmdBuffer);
typedef GR_RESULT (GR_STDCALL *PFN_grBeginCommandBuffer)(GR_CMD_BUFFER cmdBuffer, GR_FLAGS flags);
typedef GR_RESULT (GR_STDCALL *PFN_grEndCommandBuffer)(GR_CMD_BUFFER cmdBuffer);
typedef GR_RESULT (GR_STDCALL *PFN_grResetCommandBuffer)(GR_CMD_BUFFER cmdBuffer);

// COMMAND BUFFER BUILDING FUNCTIONS
typedef GR_VOID (GR_STDCALL *PFN_grCmdBindPipeline)(GR_CMD_BUFFER cmdBuffer, GR_ENUM pipelineBindPoint, GR_PIPELINE pipeline);
typedef GR_VOID (GR_STDCALL *PFN_grCmdBindStateObject)(GR_CMD_BUFFER cmdBuffer, GR_ENUM stateBindPoint, GR_STATE_OBJECT state);
typedef GR_VOID (GR_STDCALL *PFN_grCmdBindDescriptorSet)(GR_CMD_BUFFER cmdBuffer, GR_ENUM pipelineBindPoint, GR_UINT index, GR_DESCRIPTOR_SET descriptorSet, GR_UINT slotOffset);
typedef GR_VOID (GR_STDCALL *PFN_grCmdBindDynamicMemoryView)(GR_CMD_BUFFER cmdBuffer, GR_ENUM pipelineBindPoint, const GR_MEMORY_VIEW_ATTACH_INFO* pMemView);
typedef GR_VOID (GR_STDCALL *PFN_grCmdBindIndexData)(GR_CMD_BUFFER cmdBuffer, GR_GPU_MEMORY mem, GR_GPU_SIZE offset, GR_ENUM indexType);
typedef GR_VOID (GR_STDCALL *PFN_grCmdBindTargets)(GR_CMD_BUFFER cmdBuffer, GR_UINT colorTargetCount, const GR_COLOR_TARGET_BIND_INFO* pColorTargets, const GR_DEPTH_STENCIL_BIND_INFO* pDepthTarget);
typedef GR_VOID (GR_STDCALL *PFN_grCmdPrepareMemoryRegions)(GR_CMD_BUFFER cmdBuffer, GR_UINT transitionCount, const GR_MEMORY_STATE_TRANSITION* pStateTransitions);
typedef GR_VOID (GR_STDCALL *PFN_grCmdPrepareImages)(GR_CMD_BUFFER cmdBuffer, GR_UINT transitionCount, const GR_IMAGE_STATE_TRANSITION* pStateTransitions);
typedef GR_VOID (GR_STDCALL *PFN_grCmdDraw)(GR_CMD_BUFFER cmdBuffer, GR_UINT firstVertex, GR_UINT vertexCount, GR_UINT firstInstance, GR_UINT instanceCount);
typedef GR_VOID (GR_STDCALL *PFN_grCmdDrawIndexed)(GR_CMD_BUFFER cmdBuffer, GR_UINT firstIndex, GR_UINT indexCount, GR_INT vertexOffset, GR_UINT firstInstance, GR_UINT instanceCount);
typedef GR_VOID (GR_STDCALL *PFN_grCmdDrawIndirect)(GR_CMD_BUFFER cmdBuffer, GR_GPU_MEMORY mem, GR_GPU_SIZE offset);
typedef GR_VOID (GR_STDCALL *PFN_grCmdDrawIndexedIndirect)(GR_CMD_BUFFER cmdBuffer, GR_GPU_MEMORY mem, GR_GPU_SIZE offset);
typedef GR_VOID (GR_STDCALL *PFN_grCmdDispatch)(GR_CMD_BUFFER cmdBuffer, GR_UINT x, GR_UINT y, GR_UINT z);
typedef GR_VOID (GR_STDCALL *PFN_grCmdDispatchIndirect)(GR_CMD_BUFFER cmdBuffer, GR_GPU_MEMORY mem, GR_GPU_SIZE offset);
typedef GR_VOID (GR_STDCALL *PFN_grCmdCopyMemory)(GR_CMD_BUFFER cmdBuffer, GR_GPU_MEMORY srcMem, GR_GPU_MEMORY destMem, GR_UINT regionCount, const GR_MEMORY_COPY* pRegions);
typedef GR_VOID (GR_STDCALL *PFN_grCmdCopyImage)(GR_CMD_BUFFER cmdBuffer, GR_IMAGE srcImage, GR_IMAGE destImage, GR_UINT regionCount, const GR_IMAGE_COPY* pRegions);
typedef GR_VOID (GR_STDCALL *PFN_grCmdCopyMemoryToImage)(GR_CMD_BUFFER cmdBuffer, GR_GPU_MEMORY srcMem, GR_IMAGE destImage, GR_UINT regionCount, const GR_MEMORY_IMAGE_COPY* pRegions);
typedef GR_VOID (GR_STDCALL *PFN_grCmdCopyImageToMemory)(GR_CMD_BUFFER cmdBuffer, GR_IMAGE srcImage, GR_GPU_MEMORY destMem, GR_UINT regionCount, const GR_MEMORY_IMAGE_COPY* pRegions);
typedef GR_VOID (GR_STDCALL *PFN_grCmdResolveImage)(GR_CMD_BUFFER cmdBuffer, GR_IMAGE srcImage, GR_IMAGE destImage, GR_UINT regionCount, const GR_IMAGE_RESOLVE* pRegions);
typedef GR_VOID (GR_STDCALL *PFN_grCmdCloneImageData)(GR_CMD_BUFFER cmdBuffer, GR_IMAGE srcImage, GR_ENUM srcImageState, GR_IMAGE destImage, GR_ENUM destImageState);
typedef GR_VOID (GR_STDCALL *PFN_grCmdUpdateMemory)(GR_CMD_BUFFER cmdBuffer, GR_GPU_MEMORY destMem, GR_GPU_SIZE destOffset, GR_GPU_SIZE dataSize, const GR_UINT32* pData);
typedef GR_VOID (GR_STDCALL *PFN_grCmdFillMemory)(GR_CMD_BUFFER cmdBuffer, GR_GPU_MEMORY destMem, GR_GPU_SIZE destOffset, GR_GPU_SIZE fillSize, GR_UINT32 data);
typedef GR_VOID (GR_STDCALL *PFN_grCmdClearColorImage)(GR_CMD_BUFFER cmdBuffer, GR_IMAGE image, const GR_FLOAT color[4], GR_UINT rangeCount, const GR_IMAGE_SUBRESOURCE_RANGE* pRanges);
typedef GR_VOID (GR_STDCALL *PFN_grCmdClearColorImageRaw)(GR_CMD_BUFFER cmdBuffer, GR_IMAGE image, const GR_UINT32 color[4], GR_UINT rangeCount, const GR_IMAGE_SUBRESOURCE_RANGE* pRanges);
typedef GR_VOID (GR_STDCALL *PFN_grCmdClearDepthStencil)(GR_CMD_BUFFER cmdBuffer, GR_IMAGE image, GR_FLOAT depth, GR_UINT8 stencil, GR_UINT rangeCount, const GR_IMAGE_SUBRESOURCE_RANGE* pRanges);
typedef GR_VOID (GR_STDCALL *PFN_grCmdSetEvent)(GR_CMD_BUFFER cmdBuffer, GR_EVENT event);
typedef GR_VOID (GR_STDCALL *PFN_grCmdResetEvent)(GR_CMD_BUFFER cmdBuffer, GR_EVENT event);
typedef GR_VOID (GR_STDCALL *PFN_grCmdMemoryAtomic)(GR_CMD_BUFFER cmdBuffer, GR_GPU_MEMORY destMem, GR_GPU_SIZE destOffset, GR_UINT64 srcData, GR_ENUM atomicOp);
typedef GR_VOID (GR_STDCALL *PFN_grCmdBeginQuery)(GR_CMD_BUFFER cmdBuffer, GR_QUERY_POOL queryPool, GR_UINT slot, GR_FLAGS flags);
typedef GR_VOID (GR_STDCALL *PFN_grCmdEndQuery)(GR_CMD_BUFFER cmdBuffer, GR_QUERY_POOL queryPool, GR_UINT slot);
typedef GR_VOID (GR_STDCALL *PFN_grCmdResetQueryPool)(GR_CMD_BUFFER cmdBuffer, GR_QUERY_POOL queryPool, GR_UINT startQuery, GR_UINT queryCount);
typedef GR_VOID (GR_STDCALL *PFN_grCmdWriteTimestamp)(GR_CMD_BUFFER cmdBuffer, GR_ENUM timestampType, GR_GPU_MEMORY destMem, GR_GPU_SIZE destOffset);
typedef GR_VOID (GR_STDCALL *PFN_grCmdInitAtomicCounters)(GR_CMD_BUFFER cmdBuffer, GR_ENUM pipelineBindPoint, GR_UINT startCounter, GR_UINT counterCount, const GR_UINT32* pData);
typedef GR_VOID (GR_STDCALL *PFN_grCmdLoadAtomicCounters)(GR_CMD_BUFFER cmdBuffer, GR_ENUM pipelineBindPoint, GR_UINT startCounter, GR_UINT counterCount, GR_GPU_MEMORY srcMem, GR_GPU_SIZE srcOffset);
typedef GR_VOID (GR_STDCALL *PFN_grCmdSaveAtomicCounters)(GR_CMD_BUFFER cmdBuffer, GR_ENUM pipelineBindPoint, GR_UINT startCounter, GR_UINT counterCount, GR_GPU_MEMORY destMem, GR_GPU_SIZE destOffset);

#if GR_PROTOTYPES
GR_RESULT GR_STDCALL grInitAndEnumerateGpus(const GR_APPLICATION_INFO* pAppInfo, const GR_ALLOC_CALLBACKS* pAllocCb, GR_UINT* pGpuCount, GR_PHYSICAL_GPU gpus[GR_MAX_PHYSICAL_GPUS]);
GR_RESULT GR_STDCALL grGetGpuInfo(GR_PHYSICAL_GPU gpu, GR_ENUM infoType, GR_SIZE* pDataSize, GR_VOID* pData);
GR_RESULT GR_STDCALL grCreateDevice(GR_PHYSICAL_GPU gpu, const GR_DEVICE_CREATE_INFO* pCreateInfo, GR_DEVICE* pDevice);
GR_RESULT GR_STDCALL grDestroyDevice(GR_DEVICE device);
GR_RESULT GR_STDCALL grGetExtensionSupport(GR_PHYSICAL_GPU gpu, const GR_CHAR* pExtName);
GR_RESULT GR_STDCALL grGetDeviceQueue(GR_DEVICE device, GR_ENUM queueType, GR_UINT queueId, GR_QUEUE* pQueue);
GR_RESULT GR_STDCALL grQueueWaitIdle(GR_QUEUE queue);
GR_RESULT GR_STDCALL grDeviceWaitIdle(GR_QUEUE queue);
GR_RESULT GR_STDCALL grQueueSubmit(GR_QUEUE queue, GR_UINT cmdBufferCount, const GR_CMD_BUFFER* pCmdBuffers, GR_UINT memRefCount, const GR_MEMORY_REF* pMemRefs, GR_FENCE fence);
GR_RESULT GR_STDCALL grQueueSetGlobalMemReferences(GR_QUEUE queue, GR_UINT memRefCount, const GR_MEMORY_REF* pMemRefs);
GR_RESULT GR_STDCALL grGetMemoryHeapCount(GR_DEVICE device, GR_UINT* pCount);
GR_RESULT GR_STDCALL grGetMemoryHeapInfo(GR_DEVICE device, GR_UINT heapId, GR_ENUM infoType, GR_SIZE* pDataSize, GR_VOID* pData);
GR_RESULT GR_STDCALL grAllocMemory(GR_DEVICE device, const GR_MEMORY_ALLOC_INFO* pAllocInfo, GR_GPU_MEMORY* pMem);
GR_RESULT GR_STDCALL grFreeMemory(GR_GPU_MEMORY mem);
GR_RESULT GR_STDCALL grSetMemoryPriority(GR_GPU_MEMORY mem, GR_ENUM priority);
GR_RESULT GR_STDCALL grMapMemory(GR_GPU_MEMORY mem, GR_FLAGS flags, GR_VOID** ppData);
GR_RESULT GR_STDCALL grUnmapMemory(GR_GPU_MEMORY mem);
GR_RESULT GR_STDCALL grRemapVirtualMemoryPages(GR_DEVICE device, GR_UINT rangeCount, const GR_VIRTUAL_MEMORY_REMAP_RANGE* pRanges, GR_UINT preWaitSemaphoreCount, const GR_QUEUE_SEMAPHORE* pPreWatSemaphores, GR_UINT postSignalSemaphoreCount, const GR_QUEUE_SEMAPHORE* pPostSignalSemaphores);
GR_RESULT GR_STDCALL grPinSystemMemory(GR_DEVICE device, const GR_VOID* pSysMem, GR_SIZE memSize, GR_GPU_MEMORY* pMem);
GR_RESULT GR_STDCALL grDestroyObject(GR_OBJECT object);
GR_RESULT GR_STDCALL grGetObjectInfo(GR_BASE_OBJECT object, GR_ENUM infoType, GR_SIZE* pDataSize, GR_VOID* pData);
GR_RESULT GR_STDCALL grBindObjectMemory(GR_OBJECT object, GR_GPU_MEMORY mem, GR_GPU_SIZE offset);
GR_RESULT GR_STDCALL grGetFormatInfo(GR_DEVICE device, GR_FORMAT format, GR_ENUM infoType, GR_SIZE* pDataSize, GR_VOID* pData);
GR_RESULT GR_STDCALL grCreateImage(GR_DEVICE device, const GR_IMAGE_CREATE_INFO* pCreateInfo, GR_IMAGE* pImage);
GR_RESULT GR_STDCALL grGetImageSubresourceInfo(GR_IMAGE image, const GR_IMAGE_SUBRESOURCE* pSubresource, GR_ENUM infoType, GR_SIZE* pDataSize, GR_VOID* pData);
GR_RESULT GR_STDCALL grCreateSampler(GR_DEVICE device, const GR_SAMPLER_CREATE_INFO* pCreateInfo, GR_SAMPLER* pSampler);
GR_RESULT GR_STDCALL grCreateImageView(GR_DEVICE device, const GR_IMAGE_VIEW_CREATE_INFO* pCreateInfo, GR_IMAGE_VIEW* pView);
GR_RESULT GR_STDCALL grCreateColorTargetView(GR_DEVICE device, const GR_COLOR_TARGET_VIEW_CREATE_INFO* pCreateInfo, GR_COLOR_TARGET_VIEW* pView);
GR_RESULT GR_STDCALL grCreateShader(GR_DEVICE device, const GR_SHADER_CREATE_INFO* pCreateInfo, GR_SHADER* pShader);
GR_RESULT GR_STDCALL grCreateGraphicsPipeline(GR_DEVICE device, const GR_GRAPHICS_PIPELINE_CREATE_INFO* pCreateInfo, GR_PIPELINE* pPipeline);
GR_RESULT GR_STDCALL grCreateComputePipeline(GR_DEVICE device, const GR_COMPUTE_PIPELINE_CREATE_INFO* pCreateInfo, GR_PIPELINE* pPipeline);
GR_RESULT GR_STDCALL grStorePipeline(GR_PIPELINE pipeline, GR_SIZE* pDataSize, GR_VOID* pData);
GR_RESULT GR_STDCALL grLoadPipeline(GR_DEVICE device, GR_SIZE dataSize, const GR_VOID* pData, GR_PIPELINE* pPipeline);
GR_RESULT GR_STDCALL grCreateDescriptorSet(GR_DEVICE device, const GR_DESCRIPTOR_SET_CREATE_INFO* pCreateInfo, GR_DESCRIPTOR_SET* pDescriptorSet);
GR_VOID GR_STDCALL grBeginDescriptorSetUpdate(GR_DESCRIPTOR_SET descriptorSet);
GR_VOID GR_STDCALL grEndDescriptorSetUpdate(GR_DESCRIPTOR_SET descriptorSet);
GR_VOID GR_STDCALL grAttachSamplerDescriptors(GR_DESCRIPTOR_SET descriptorSet, GR_UINT startSlot, GR_UINT slotCount, const GR_SAMPLER* pSamplers);
GR_VOID GR_STDCALL grAttachImageViewDescriptors(GR_DESCRIPTOR_SET descriptorSet, GR_UINT startSlot, GR_UINT slotCount, const GR_IMAGE_VIEW_ATTACH_INFO* pImageViews);
GR_VOID GR_STDCALL grAttachMemoryViewDescriptors(GR_DESCRIPTOR_SET descriptorSet, GR_UINT startSlot, GR_UINT slotCount, const GR_MEMORY_VIEW_ATTACH_INFO* pMemViews);
GR_VOID GR_STDCALL grAttachNestedDescriptors(GR_DESCRIPTOR_SET descriptorSet, GR_UINT startSlot, GR_UINT slotCount, const GR_DESCRIPTOR_SET_ATTACH_INFO* pNestedDescriptorSets);
GR_VOID GR_STDCALL grClearDescriptorSetSlots(GR_DESCRIPTOR_SET descriptorSet, GR_UINT startSlot, GR_UINT slotCount);
GR_RESULT GR_STDCALL grCreateViewportState(GR_DEVICE device, const GR_VIEWPORT_STATE_CREATE_INFO* pCreateInfo, GR_VIEWPORT_STATE_OBJECT* pState);
GR_RESULT GR_STDCALL grCreateRasterState(GR_DEVICE device, const GR_RASTER_STATE_CREATE_INFO* pCreateInfo, GR_RASTER_STATE_OBJECT* pState);
GR_RESULT GR_STDCALL grCreateColorBlendState(GR_DEVICE device, const GR_COLOR_BLEND_STATE_CREATE_INFO* pCreateInfo, GR_COLOR_BLEND_STATE_OBJECT* pState);
GR_RESULT GR_STDCALL grCreateDepthStencilState(GR_DEVICE device, const GR_DEPTH_STENCIL_STATE_CREATE_INFO* pCreateInfo, GR_DEPTH_STENCIL_STATE_OBJECT* pState);
GR_RESULT GR_STDCALL grCreateMsaaState(GR_DEVICE device, const GR_MSAA_STATE_CREATE_INFO* pCreateInfo, GR_MSAA_STATE_OBJECT* pState);
GR_RESULT GR_STDCALL grCreateQueryPool(GR_DEVICE device, const GR_QUERY_POOL_CREATE_INFO* pCreateInfo, GR_QUERY_POOL* pQueryPool);
GR_RESULT GR_STDCALL grGetQueryPoolResults(GR_QUERY_POOL queryPool, GR_UINT startQuery, GR_UINT queryCount, GR_SIZE* pDataSize, GR_VOID* pData);
GR_RESULT GR_STDCALL grCreateFence(GR_DEVICE device, const GR_FENCE_CREATE_INFO* pCreateInfo, GR_FENCE* pFence);
GR_RESULT GR_STDCALL grGetFenceStatus(GR_FENCE fence);
GR_RESULT GR_STDCALL grWaitForFences(GR_DEVICE device, GR_UINT fenceCount, const GR_FENCE* pFences, GR_BOOL waitAll, GR_FLOAT timeout);
GR_RESULT GR_STDCALL grCreateQueueSemaphore(GR_DEVICE device, const GR_QUEUE_SEMAPHORE_CREATE_INFO* pCreateInfo, GR_QUEUE_SEMAPHORE* pSemaphore);
GR_RESULT GR_STDCALL grSignalQueueSemaphore(GR_QUEUE queue, GR_QUEUE_SEMAPHORE semaphore);
GR_RESULT GR_STDCALL grWaitQueueSemaphore(GR_QUEUE queue, GR_QUEUE_SEMAPHORE semaphore);
GR_RESULT GR_STDCALL grCreateEvent(GR_DEVICE device, const GR_EVENT_CREATE_INFO* pCreateInfo, GR_EVENT* pEvent);
GR_RESULT GR_STDCALL grGetEventStatus(GR_EVENT event);
GR_RESULT GR_STDCALL grSetEvent(GR_EVENT event);
GR_RESULT GR_STDCALL grResetEvent(GR_EVENT event);
GR_RESULT GR_STDCALL grGetMultiGpuCompatibility(GR_PHYSICAL_GPU gpu0, GR_PHYSICAL_GPU gpu1, GR_GPU_COMPATIBILITY_INFO* pInfo);
GR_RESULT GR_STDCALL grOpenSharedMemory(GR_DEVICE device, const GR_MEMORY_OPEN_INFO* pOpenInfo, GR_GPU_MEMORY* pMem);
GR_RESULT GR_STDCALL grOpenSharedQueueSemaphore(GR_DEVICE device, const GR_QUEUE_SEMAPHORE_OPEN_INFO* pOpenInfo, GR_QUEUE_SEMAPHORE* pSemaphore);
GR_RESULT GR_STDCALL grOpenPeerMemory(GR_DEVICE device, const GR_PEER_MEMORY_OPEN_INFO* pOpenInfo, GR_GPU_MEMORY* pMem);
GR_RESULT GR_STDCALL grOpenPeerImage(GR_DEVICE device, const GR_PEER_IMAGE_OPEN_INFO* pOpenInfo, GR_IMAGE* pImage, GR_GPU_MEMORY* pMem);
GR_RESULT GR_STDCALL grCreateCommandBuffer(GR_DEVICE device, const GR_CMD_BUFFER_CREATE_INFO* pCreateInfo, GR_CMD_BUFFER* pCmdBuffer);
GR_RESULT GR_STDCALL grBeginCommandBuffer(GR_CMD_BUFFER cmdBuffer, GR_FLAGS flags);
GR_RESULT GR_STDCALL grEndCommandBuffer(GR_CMD_BUFFER cmdBuffer);
GR_RESULT GR_STDCALL grResetCommandBuffer(GR_CMD_BUFFER cmdBuffer);
GR_VOID GR_STDCALL grCmdBindPipeline(GR_CMD_BUFFER cmdBuffer, GR_ENUM pipelineBindPoint, GR_PIPELINE pipeline);
GR_VOID GR_STDCALL grCmdBindStateObject(GR_CMD_BUFFER cmdBuffer, GR_ENUM stateBindPoint, GR_STATE_OBJECT state);
GR_VOID GR_STDCALL grCmdBindDescriptorSet(GR_CMD_BUFFER cmdBuffer, GR_ENUM pipelineBindPoint, GR_UINT index, GR_DESCRIPTOR_SET descriptorSet, GR_UINT slotOffset);
GR_VOID GR_STDCALL grCmdBindDynamicMemoryView(GR_CMD_BUFFER cmdBuffer, GR_ENUM pipelineBindPoint, const GR_MEMORY_VIEW_ATTACH_INFO* pMemView);
GR_VOID GR_STDCALL grCmdBindIndexData(GR_CMD_BUFFER cmdBuffer, GR_GPU_MEMORY mem, GR_GPU_SIZE offset, GR_ENUM indexType);
GR_VOID GR_STDCALL grCmdBindTargets(GR_CMD_BUFFER cmdBuffer, GR_UINT colorTargetCount, const GR_COLOR_TARGET_BIND_INFO* pColorTargets, const GR_DEPTH_STENCIL_BIND_INFO* pDepthTarget);
GR_VOID GR_STDCALL grCmdPrepareMemoryRegions(GR_CMD_BUFFER cmdBuffer, GR_UINT transitionCount, const GR_MEMORY_STATE_TRANSITION* pStateTransitions);
GR_VOID GR_STDCALL grCmdPrepareImages(GR_CMD_BUFFER cmdBuffer, GR_UINT transitionCount, const GR_IMAGE_STATE_TRANSITION* pStateTransitions);
GR_VOID GR_STDCALL grCmdDraw(GR_CMD_BUFFER cmdBuffer, GR_UINT firstVertex, GR_UINT vertexCount, GR_UINT firstInstance, GR_UINT instanceCount);
GR_VOID GR_STDCALL grCmdDrawIndexed(GR_CMD_BUFFER cmdBuffer, GR_UINT firstIndex, GR_UINT indexCount, GR_INT vertexOffset, GR_UINT firstInstance, GR_UINT instanceCount);
GR_VOID GR_STDCALL grCmdDrawIndirect(GR_CMD_BUFFER cmdBuffer, GR_GPU_MEMORY mem, GR_GPU_SIZE offset);
GR_VOID GR_STDCALL grCmdDrawIndexedIndirect(GR_CMD_BUFFER cmdBuffer, GR_GPU_MEMORY mem, GR_GPU_SIZE offset);
GR_VOID GR_STDCALL grCmdDispatch(GR_CMD_BUFFER cmdBuffer, GR_UINT x, GR_UINT y, GR_UINT z);
GR_VOID GR_STDCALL grCmdDispatchIndirect(GR_CMD_BUFFER cmdBuffer, GR_GPU_MEMORY mem, GR_GPU_SIZE offset);
GR_VOID GR_STDCALL grCmdCopyMemory(GR_CMD_BUFFER cmdBuffer, GR_GPU_MEMORY srcMem, GR_GPU_MEMORY destMem, GR_UINT regionCount, const GR_MEMORY_COPY* pRegions);
GR_VOID GR_STDCALL grCmdCopyImage(GR_CMD_BUFFER cmdBuffer, GR_IMAGE srcImage, GR_IMAGE destImage, GR_UINT regionCount, const GR_IMAGE_COPY* pRegions);
GR_VOID GR_STDCALL grCmdCopyMemoryToImage(GR_CMD_BUFFER cmdBuffer, GR_GPU_MEMORY srcMem, GR_IMAGE destImage, GR_UINT regionCount, const GR_MEMORY_IMAGE_COPY* pRegions);
GR_VOID GR_STDCALL grCmdCopyImageToMemory(GR_CMD_BUFFER cmdBuffer, GR_IMAGE srcImage, GR_GPU_MEMORY destMem, GR_UINT regionCount, const GR_MEMORY_IMAGE_COPY* pRegions);
GR_VOID GR_STDCALL grCmdResolveImage(GR_CMD_BUFFER cmdBuffer, GR_IMAGE srcImage, GR_IMAGE destImage, GR_UINT regionCount, const GR_IMAGE_RESOLVE* pRegions);
GR_VOID GR_STDCALL grCmdCloneImageData(GR_CMD_BUFFER cmdBuffer, GR_IMAGE srcImage, GR_ENUM srcImageState, GR_IMAGE destImage, GR_ENUM destImageState);
GR_VOID GR_STDCALL grCmdUpdateMemory(GR_CMD_BUFFER cmdBuffer, GR_GPU_MEMORY destMem, GR_GPU_SIZE destOffset, GR_GPU_SIZE dataSize, const GR_UINT32* pData);
GR_VOID GR_STDCALL grCmdFillMemory(GR_CMD_BUFFER cmdBuffer, GR_GPU_MEMORY destMem, GR_GPU_SIZE destOffset, GR_GPU_SIZE fillSize, GR_UINT32 data);
GR_VOID GR_STDCALL grCmdClearColorImage(GR_CMD_BUFFER cmdBuffer, GR_IMAGE image, const GR_FLOAT color[4], GR_UINT rangeCount, const GR_IMAGE_SUBRESOURCE_RANGE* pRanges);
GR_VOID GR_STDCALL grCmdClearColorImageRaw(GR_CMD_BUFFER cmdBuffer, GR_IMAGE image, const GR_UINT32 color[4], GR_UINT rangeCount, const GR_IMAGE_SUBRESOURCE_RANGE* pRanges);
GR_VOID GR_STDCALL grCmdClearDepthStencil(GR_CMD_BUFFER cmdBuffer, GR_IMAGE image, GR_FLOAT depth, GR_UINT8 stencil, GR_UINT rangeCount, const GR_IMAGE_SUBRESOURCE_RANGE* pRanges);
GR_VOID GR_STDCALL grCmdSetEvent(GR_CMD_BUFFER cmdBuffer, GR_EVENT event);
GR_VOID GR_STDCALL grCmdResetEvent(GR_CMD_BUFFER cmdBuffer, GR_EVENT event);
GR_VOID GR_STDCALL grCmdMemoryAtomic(GR_CMD_BUFFER cmdBuffer, GR_GPU_MEMORY destMem, GR_GPU_SIZE destOffset, GR_UINT64 srcData, GR_ENUM atomicOp);
GR_VOID GR_STDCALL grCmdBeginQuery(GR_CMD_BUFFER cmdBuffer, GR_QUERY_POOL queryPool, GR_UINT slot, GR_FLAGS flags);
GR_VOID GR_STDCALL grCmdEndQuery(GR_CMD_BUFFER cmdBuffer, GR_QUERY_POOL queryPool, GR_UINT slot);
GR_VOID GR_STDCALL grCmdResetQueryPool(GR_CMD_BUFFER cmdBuffer, GR_QUERY_POOL queryPool, GR_UINT startQuery, GR_UINT queryCount);
GR_VOID GR_STDCALL grCmdWriteTimestamp(GR_CMD_BUFFER cmdBuffer, GR_ENUM timestampType, GR_GPU_MEMORY destMem, GR_GPU_SIZE destOffset);
GR_VOID GR_STDCALL grCmdInitAtomicCounters(GR_CMD_BUFFER cmdBuffer, GR_ENUM pipelineBindPoint, GR_UINT startCounter, GR_UINT counterCount, const GR_UINT32* pData);
GR_VOID GR_STDCALL grCmdLoadAtomicCounters(GR_CMD_BUFFER cmdBuffer, GR_ENUM pipelineBindPoint, GR_UINT startCounter, GR_UINT counterCount, GR_GPU_MEMORY srcMem, GR_GPU_SIZE srcOffset);
GR_VOID GR_STDCALL grCmdSaveAtomicCounters(GR_CMD_BUFFER cmdBuffer, GR_ENUM pipelineBindPoint, GR_UINT startCounter, GR_UINT counterCount, GR_GPU_MEMORY destMem, GR_GPU_SIZE destOffset);
#endif

#ifdef __cplusplus
}
#endif

#endif  // _MANTLE_H_
