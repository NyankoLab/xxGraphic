//==============================================================================
// xxGraphic : OpenGL ES 3.2 Source
//
// Copyright (c) 2019-2025 TAiGA
// https://github.com/metarutaiga/xxGraphic
//==============================================================================
#undef  GL_ES_VERSION_2_0
#undef  GL_ES_VERSION_3_0
#undef  GL_ES_VERSION_3_1
#undef  GL_ES_VERSION_3_2
#include "internal/xxGraphicInternalGL.h"
#include "xxGraphicGLES2.h"
#include "xxGraphicGLES3.h"
#include "xxGraphicGLES31.h"
#include "xxGraphicGLES32.h"

#if defined(xxANDROID)
#   include "xxGraphicEGL.h"
#endif

#if defined(xxIOS) && !defined(xxMACCATALYST)
#   include "xxGraphicEAGL.h"
#endif

#if defined(xxMACOS)
#   include "xxGraphicCGL.h"
#endif

#if defined(xxWINDOWS)
#   include "xxGraphicWGL.h"
#endif

//==============================================================================
//  Instance
//==============================================================================
uint64_t xxCreateInstanceGLES32()
{
    uint64_t instance = 0;

#if defined(xxANDROID)
    instance = xxGraphicCreateEGL(320);
#endif

#if defined(xxMACOS)
    instance = xxGraphicCreateCGL(320);
#endif

#if defined(xxIOS) && !defined(xxMACCATALYST)
    instance = xxGraphicCreateEAGL(320);
#endif

#if defined(xxWINDOWS)
    instance = xxGraphicCreateWGL(320);
#endif

    if (instance == 0)
        return 0;

    xxRegisterFunction(GLES2);

    xxRegisterFunctionSingle(xxCreateInstance, xxCreateInstanceGLES32);
    xxRegisterFunctionSingle(xxGetInstanceName, xxGetInstanceNameGLES32);

    xxRegisterFunctionSingle(xxCreateIndexBuffer, xxCreateIndexBufferGLES3);
    xxRegisterFunctionSingle(xxCreateVertexBuffer, xxCreateVertexBufferGLES3);
    xxRegisterFunctionSingle(xxMapBuffer, xxMapBufferGLES3);
    xxRegisterFunctionSingle(xxUnmapBuffer, xxUnmapBufferGLES3);

    xxRegisterFunctionSingle(xxSetVertexBuffers, xxSetVertexBuffersGLES32);
    xxRegisterFunctionSingle(xxDraw, xxDrawGLES32);
    xxRegisterFunctionSingle(xxDrawIndexed, xxDrawIndexedGLES32);

    return instance;
}
//==============================================================================
//  Command
//==============================================================================
void xxSetVertexBuffersGLES32(uint64_t commandEncoder, int count, const uint64_t* buffers, uint64_t vertexAttribute)
{
    GLVERTEXATTRIBUTE* glVertexAttribute = reinterpret_cast<GLVERTEXATTRIBUTE*>(vertexAttribute);
    GLVERTEXATTRIBUTE::Attribute* attributes = glVertexAttribute->attributes;

    int currentStream = -1;
    for (int i = 0; i < glVertexAttribute->count; ++i)
    {
        GLVERTEXATTRIBUTE::Attribute& attribute = attributes[i];
        if (currentStream != attribute.stream)
        {
            currentStream = attribute.stream;
            GLBUFFER* glBuffer = reinterpret_cast<GLBUFFER*>(buffers[attribute.stream]);

            glBindBuffer(GL_ARRAY_BUFFER, glBuffer->buffer);
        }
        glVertexAttribPointer(attribute.index, attribute.size, attribute.type, attribute.normalized, attribute.stride, attribute.pointer);
    }
}
//------------------------------------------------------------------------------
void xxDrawGLES32(uint64_t commandEncoder, int vertexCount, int instanceCount, int firstVertex, int firstInstance)
{
    glDrawArraysInstanced(GL_TRIANGLES, firstVertex, vertexCount, instanceCount);
}
//------------------------------------------------------------------------------
void xxDrawIndexedGLES32(uint64_t commandEncoder, uint64_t indexBuffer, int indexCount, int vertexCount, int instanceCount, int firstIndex, int vertexOffset, int firstInstance)
{
    GLenum indexType = vertexCount < 65536 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
    glDrawElementsInstancedBaseVertex(GL_TRIANGLES, indexCount, indexType, (char*)nullptr + firstIndex * (vertexCount < 65536 ? 2 : 4), instanceCount, vertexOffset);
}
//==============================================================================
