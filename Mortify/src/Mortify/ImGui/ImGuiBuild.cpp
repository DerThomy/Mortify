#include "mtpch.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#ifdef MT_PLATFORM_WINDOWS
    #include <examples/imgui_impl_opengl3.cpp>
    #include <examples/imgui_impl_win32.cpp>
#elif defined(MT_PLATFORM_LINUX)
    #include <examples/imgui_impl_opengl3.cpp>
    #include <examples/imgui_impl_glfw.cpp>
#endif