workspace "Mortify"
    architecture "x86_64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution dir)
IncludeDir = {}
IncludeDir["GLFW"] = "Mortify/vendor/GLFW/include"

include "Mortify/vendor/GLFW"

project "Mortify"
    location "Mortify"
    kind "SharedLib"
    language "C++"
	staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "mtpch.h"
	pchsource "Mortify/src/mtpch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
		"%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
    }

	links {
		"GLFW"
	}

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "MT_PLATFORM_WINDOWS",
            "MT_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .."/Sandbox/\"")
        }

    filter "configurations:Debug"
        defines "MT_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "MT_RELEASE"
        symbols "On"

    filter "configurations:Dist"
        defines "MT_DIST"
        symbols "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
	staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "Mortify/src",
        "Mortify/vendor/spdlog/include"
    }

	links {
		"Mortify"
	}

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
    
        defines {
            "MT_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        defines "MT_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "MT_RELEASE"
        symbols "On"

    filter "configurations:Dist"
        defines "MT_DIST"
        symbols "On"

