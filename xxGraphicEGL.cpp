//==============================================================================
// xxGraphic : OpenGL EGL Source
//
// Copyright (c) 2019 TAiGA
// https://github.com/metarutaiga/xxGraphic
//==============================================================================
#include "xxGraphicInternal.h"
#include "xxGraphicGL.h"
#include "xxGraphicEGL.h"

#if defined(xxANDROID)
#   include <dlfcn.h>
#endif

#define EGL_EGL_PROTOTYPES 0
#include "gl/egl.h"
static void*                                    g_eglLibrary = nullptr;
static void*                                    g_glLibrary = nullptr;
static EGLConfig                                g_eglConfig = nullptr;
static EGLDisplay                               g_eglDisplay = EGL_NO_DISPLAY;
static PFNEGLCHOOSECONFIGPROC                   eglChooseConfig;
static PFNEGLCREATECONTEXTPROC                  eglCreateContext;
static PFNEGLCREATEPBUFFERSURFACEPROC           eglCreatePbufferSurface;
static PFNEGLCREATEWINDOWSURFACEPROC            eglCreateWindowSurface;
static PFNEGLDESTROYCONTEXTPROC                 eglDestroyContext;
static PFNEGLDESTROYSURFACEPROC                 eglDestroySurface;
static PFNEGLGETCURRENTCONTEXTPROC              eglGetCurrentContext;
static PFNEGLGETDISPLAYPROC                     eglGetDisplay;
static PFNEGLGETPROCADDRESSPROC                 eglGetProcAddress;
static PFNEGLINITIALIZEPROC                     eglInitialize;
static PFNEGLMAKECURRENTPROC                    eglMakeCurrent;
static PFNEGLSWAPBUFFERSPROC                    eglSwapBuffers;
static PFNGLGENVERTEXARRAYSPROC                 glGenVertexArrays;
static PFNGLDELETEVERTEXARRAYSPROC              glDeleteVertexArrays;
static PFNGLBINDVERTEXARRAYPROC                 glBindVertexArray;

//==============================================================================
//  Initialize - EGL
//==============================================================================
static bool eglSymbolFailed = false;
static void* GL_APIENTRY eglSymbol(const char* name, bool* failed)
{
    void* ptr = nullptr;

    if (ptr == nullptr && eglGetProcAddress)
        ptr = (void*)eglGetProcAddress(name);

#if defined(xxANDROID)
    if (ptr == nullptr && g_glLibrary)
        ptr = dlsym(g_glLibrary, name);

    if (ptr == nullptr && g_eglLibrary)
        ptr = dlsym(g_eglLibrary, name);
#elif defined(xxWINDOWS)
    if (ptr == nullptr && g_glLibrary)
        ptr = GetProcAddress((HMODULE)g_glLibrary, name);

    if (ptr == nullptr && g_eglLibrary)
        ptr = GetProcAddress((HMODULE)g_eglLibrary, name);
#endif

    if (ptr == nullptr)
        xxLog("EGL", "%s is not found", name);

    eglSymbolFailed |= (ptr == nullptr);
    if (failed)
        (*failed) |= (ptr == nullptr);

    return ptr;
}
#define eglSymbol(var) (void*&)var = eglSymbol(#var, nullptr);
//------------------------------------------------------------------------------
uint64_t glCreateContextEGL(uint64_t instance, void* view, void** surface)
{
    EGLContext eglShareContext = reinterpret_cast<EGLContext>(instance);
    EGLDisplay eglDisplay = g_eglDisplay;
    EGLConfig eglConfig = g_eglConfig;

    EGLSurface eglSurface = EGL_NO_SURFACE;
    if (view)
    {
        eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, (EGLNativeWindowType)view, nullptr);
    }
    else
    {
        EGLint surfaceAttributes[] =
        {
            EGL_WIDTH,  1,
            EGL_HEIGHT, 1,
            EGL_NONE
        };

        eglSurface = eglCreatePbufferSurface(eglDisplay, eglConfig, surfaceAttributes);
    }
    if (eglSurface == EGL_NO_SURFACE)
        return 0;

    EGLint contextAttibutes[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    EGLContext eglContext = eglCreateContext(eglDisplay, eglConfig, eglShareContext, contextAttibutes);
    if (eglContext == EGL_NO_CONTEXT)
        return 0;
 
    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);

    eglSymbol(glGetIntegerv);
    eglSymbol(glGenVertexArrays);
    eglSymbol(glDeleteVertexArrays);
    eglSymbol(glBindVertexArray);
    if (glGenVertexArrays && glBindVertexArray)
    {
        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
    }

    if (surface)
    {
        (*surface) = eglSurface;
    }
    else
    {
        eglDestroySurface(eglDisplay, eglSurface);
    }

    return reinterpret_cast<uint64_t>(eglContext);
}
//------------------------------------------------------------------------------
void glDestroyContextEGL(uint64_t context, void* view, void* surface)
{
    EGLContext eglContext = reinterpret_cast<EGLContext>(context);
    if (eglContext == EGL_NO_CONTEXT)
        return;
    EGLDisplay eglDisplay = g_eglDisplay;
    EGLSurface eglSurface = reinterpret_cast<EGLSurface>(surface);

    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);

    if (glGetIntegerv && glDeleteVertexArrays && glBindVertexArray)
    {
        GLuint vao = 0;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&vao);
        glDeleteVertexArrays(1, &vao);
        glBindVertexArray(0);
    }

    eglMakeCurrent(eglDisplay, nullptr, nullptr, nullptr);
    eglDestroyContext(eglDisplay, eglContext);
    eglDestroySurface(eglDisplay, eglSurface);
}
//------------------------------------------------------------------------------
void glMakeCurrentContextEGL(uint64_t context, void* surface)
{
    EGLContext eglContext = reinterpret_cast<EGLContext>(context);
    EGLDisplay eglDisplay = g_eglDisplay;
    EGLSurface eglSurface = reinterpret_cast<EGLSurface>(surface);

    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
}
//------------------------------------------------------------------------------
void glPresentContextEGL(uint64_t context, void* surface)
{
    EGLDisplay eglDisplay = g_eglDisplay;
    EGLSurface eglSurface = reinterpret_cast<EGLSurface>(surface);

    eglSwapBuffers(eglDisplay, eglSurface);
}
//------------------------------------------------------------------------------
void glGetViewSizeEGL(void* view, unsigned int* width, unsigned int* height)
{
}
//------------------------------------------------------------------------------
uint64_t xxGraphicCreateEGL()
{
#if defined(xxANDROID)
    if (g_eglLibrary == nullptr)
        g_eglLibrary = dlopen("libEGL.so", RTLD_LAZY);
#elif defined(xxWINDOWS)
    if (g_eglLibrary == nullptr)
        g_eglLibrary = LoadLibraryW(L"libEGL.dll");
#endif
    if (g_eglLibrary == nullptr)
        return 0;

#if defined(xxANDROID)
    if (g_glLibrary == nullptr)
        g_glLibrary = dlopen("libGLESv2.so", RTLD_LAZY);
#elif defined(xxWINDOWS)
    if (g_glLibrary == nullptr)
        g_glLibrary = LoadLibraryW(L"libGLESv2.dll");
#endif
    if (g_glLibrary == nullptr)
        return 0;

    eglSymbolFailed = false;
    eglSymbol(eglChooseConfig);
    eglSymbol(eglCreateContext);
    eglSymbol(eglCreatePbufferSurface);
    eglSymbol(eglCreateWindowSurface);
    eglSymbol(eglDestroyContext);
    eglSymbol(eglDestroySurface);
    eglSymbol(eglGetCurrentContext);
    eglSymbol(eglGetDisplay);
    eglSymbol(eglGetProcAddress);
    eglSymbol(eglInitialize);
    eglSymbol(eglMakeCurrent);
    eglSymbol(eglSwapBuffers);
    if (eglSymbolFailed)
        return 0;

    EGLDisplay eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (eglDisplay == EGL_NO_DISPLAY)
        return 0;
    g_eglDisplay = eglDisplay;

    EGLint majorVersion = 0;
    EGLint minorVersion = 0;
    if (eglInitialize(eglDisplay, &majorVersion, &minorVersion) == EGL_FALSE)
        return 0;

    EGLint configs = 0;
    EGLint configAttributes[] =
    {
        EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
        EGL_DEPTH_SIZE,         24,
        EGL_RED_SIZE,           8,
        EGL_GREEN_SIZE,         8,
        EGL_BLUE_SIZE,          8,
        EGL_NONE
    };

    EGLConfig eglConfig = nullptr;
    if (eglChooseConfig(eglDisplay, configAttributes, &eglConfig, 1, &configs) == EGL_FALSE)
        return 0;
    g_eglConfig = eglConfig;

    uint64_t context = glCreateContextEGL(0, nullptr, nullptr);
    if (context == 0)
        return 0;

    if (xxGraphicCreateGL(eglSymbol) == false)
    {
        xxGraphicDestroyEGL(context);
        return 0;
    }

    glCreateContext = glCreateContextEGL;
    glDestroyContext = glDestroyContextEGL;
    glMakeCurrentContext = glMakeCurrentContextEGL;
    glPresentContext = glPresentContextEGL;
    glGetViewSize = glGetViewSizeEGL;

    return context;
}
//------------------------------------------------------------------------------
void xxGraphicDestroyEGL(uint64_t context)
{
    glDestroyContextEGL(context, nullptr, nullptr);

    if (g_glLibrary)
    {
#if defined(xxANDROID)
        dlclose(g_glLibrary);
#elif defined(xxWINDOWS)
        FreeLibrary((HMODULE)g_glLibrary);
#endif
        g_glLibrary = nullptr;
    }

    if (g_eglLibrary)
    {
#if defined(xxANDROID)
        dlclose(g_eglLibrary);
#elif defined(xxWINDOWS)
        FreeLibrary((HMODULE)g_eglLibrary);
#endif
        g_eglLibrary = nullptr;
    }

    g_eglConfig = nullptr;
    g_eglDisplay = EGL_NO_DISPLAY;

    xxGraphicDestroyGL();
}
//==============================================================================
