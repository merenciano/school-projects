
newoption {
    trigger = "graphics-api",
    value = "API",
    description = "Choose the 3D graphics backend",
    default = "OPENGL",
    allowed = {
        { "OPENGL", "OpenGL 3.3" },
        { "OPENGL_ES",  "OpenGL for Embedded Systems 3.0" }
    }
}

workspace "Leep"
    configurations {"Debug", "Release", "Trace" }
    location "build"
    architecture "x86_64"

project "server"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "bin/"
    objdir "build/"
    defines { "KILLBALL_SERVER" }

    files {
        "./server/server.cpp",
    }

    configuration "vs2019"
        defines {"_CRT_SECURE_NO_WARNINGS", "_GLFW_WIN32", "LEEP_WIN", }

    filter "configurations:Debug"
        symbols "On"
    
    filter "configurations:Release"
        optimize "On"

project "killball"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "bin/"
    objdir "build/"
    includedirs {
        "include/",
        "src/",
        "extern/include/",
    }

    defines { "LEEP_" .. _OPTIONS["graphics-api"] }

    files {
        "src/killball.cpp",
        "include/*.h",
        "src/*.h",
        "src/*.cpp",
        "src/*/*.h",
        "src/*/*.cpp",
        "src/*/*/*.h",
        "src/*/*/*.cpp",

        --GLFW
        "./extern/src/GLFW/internal.h",
        "./extern/src/GLFW/mappings.h",
        "./extern/src/GLFW/egl_context.h",
        "./extern/src/GLFW/osmesa_context.h",
        "./extern/src/GLFW/context.c",
        "./extern/src/GLFW/init.c",
        "./extern/src/GLFW/input.c",
        "./extern/src/GLFW/monitor.c",
        "./extern/src/GLFW/vulkan.c",
        "./extern/src/GLFW/window.c",
        "./extern/src/GLFW/egl_context.c",
        "./extern/src/GLFW/osmesa_context.c",

        --glad
        "./extern/src/glad.c",

        --minitrace
        "./extern/src/minitrace.c",

        --imgui
        "./extern/src/imgui/*.cpp",

        --lua
        "./extern/src/lua/**",


    }
    
    configuration "vs2019"
        defines {"_CRT_SECURE_NO_WARNINGS", "_GLFW_WIN32", "LEEP_WIN", }

        files {
            "./extern/src/GLFW/win32_platform.h",
            "./extern/src/GLFW/win32_joystick.h",
            "./extern/src/GLFW/wgl_context.h",
            "./extern/src/GLFW/win32_init.c",
            "./extern/src/GLFW/win32_joystick.c",
            "./extern/src/GLFW/win32_monitor.c",
            "./extern/src/GLFW/win32_time.c",
            "./extern/src/GLFW/win32_thread.c",
            "./extern/src/GLFW/win32_window.c",
            "./extern/src/GLFW/wgl_context.c",
        }

    configuration "gmake"
        defines { "_GLFW_X11", "LUA_USE_POSIX", "LEEP_LINUX", }
        links { "X11", "dl", "pthread"  }
        buildoptions { "-Wall",  }

        files {
            "./extern/src/GLFW/x11_platform.h",
            "./extern/src/GLFW/xkb_unicode.h",
            "./extern/src/GLFW/posix_time.h",
            "./extern/src/GLFW/posix_thread.h",
            "./extern/src/GLFW/glx_context.h",
            "./extern/src/GLFW/linux_joystick.h",
            "./extern/src/GLFW/x11_init.c",
            "./extern/src/GLFW/x11_monitor.c",
            "./extern/src/GLFW/x11_window.c",
            "./extern/src/GLFW/xkb_unicode.c",
            "./extern/src/GLFW/posix_time.c",
            "./extern/src/GLFW/posix_thread.c",
            "./extern/src/GLFW/glx_context.c",
            "./extern/src/GLFW/linux_joystick.c",
        }

    filter "configurations:Debug"
        defines { "LEEP_DEBUG" }
        symbols "On"
    
    filter "configurations:Release"
        defines "LEEP_RELEASE"
        optimize "On"

    filter "configurations:Trace"
        defines { "LEEP_RELEASE", "MTR_ENABLED" }
        optimize "On"
