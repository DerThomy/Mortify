workspace "Mortify"
architecture "x86_64"
startproject "Sandbox"

configurations { 
    "Debug",
    "Release",
    "Dist" 
}

flags
{
    "MultiProcessorCompile"
}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution dir)
IncludeDir = {}
IncludeDir["GLFW"] = "Mortify/vendor/GLFW/include"
IncludeDir["Glad"] = "Mortify/vendor/Glad/include"
IncludeDir["ImGui"] = "Mortify/vendor/ImGUI"
IncludeDir["glm"] = "Mortify/vendor/glm"
IncludeDir["stb_image"] = "Mortify/vendor/stb_image"
IncludeDir["assimp"] = "Mortify/vendor/assimp/include"

include "Mortify/vendor/GLFW"
include "Mortify/vendor/Glad"
include "Mortify/vendor/ImGUI"

project "Mortify"
    location "Mortify"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "mtpch.h"
	pchsource "Mortify/src/mtpch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp"
    }

    removefiles {
        "%{prj.name}/src/Platform/**.h",
        "%{prj.name}/src/Platform/**.cpp"
    }

    defines {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
    }

    includedirs {
		"%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.assimp}"
    }

	links {
		"GLFW",
		"Glad",
        "ImGui"
	}

    filter "system:windows"
        systemversion "latest"

        files { 
            "%{prj.name}/src/Platform/Windows/**.h",
            "%{prj.name}/src/Platform/Windows/**.cpp",
            "%{prj.name}/src/Platform/OpenGL/**.h",
            "%{prj.name}/src/Platform/OpenGL/**.cpp"
        }

    filter "system:linux"
        pic "on"
        systemversion "latest"

        files { 
            "%{prj.name}/src/Platform/Linux/**.h",
            "%{prj.name}/src/Platform/Linux/**.cpp",
            "%{prj.name}/src/Platform/OpenGL/**.h",
            "%{prj.name}/src/Platform/OpenGL/**.cpp"
        }

    filter "configurations:Debug"
        defines "MT_DEBUG"
		runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "MT_RELEASE"
		runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "MT_DIST"
		runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
	staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{wks.location}/Mortify/vendor/spdlog/include",
		"%{wks.location}/Mortify/src",
		"%{wks.location}/Mortify/vendor",
		"%{IncludeDir.glm}"
    }

	links {
        "Mortify",
        "GLFW",
		"Glad",
        "ImGui"
    }
    
    filter "system:linux"
        pic "on"

        links {
            "Xrandr",
            "Xi",
            "X11",
            "dl",
            "pthread",
            "stdc++fs"
        }

    filter "configurations:Debug"
        defines "MT_DEBUG"
		runtime "Debug"
        symbols "on"

        --links {
		--	"Hazel/vendor/assimp/bin/Debug/assimp-vc141-mtd.lib"
		--}

    filter "configurations:Release"
        defines "MT_RELEASE"
		runtime "Release"
        optimize "on"

        --links {
		--	"Hazel/vendor/assimp/bin/Release/assimp-vc141-mt.lib"
		--}

    filter "configurations:Dist"
        defines "MT_DIST"
		runtime "Release"
        optimize "on"

        --links {
		--	"Hazel/vendor/assimp/bin/Release/assimp-vc141-mt.lib"
		--}
