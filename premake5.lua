workspace "BunnyEngine"
	architecture "x64"
	startproject "BunnyEngineEditor"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

	flags{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/BunnyEngine/SDK/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/BunnyEngine/SDK/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/BunnyEngine/SDK/imgui"
IncludeDir["glm"] = "%{wks.location}/BunnyEngine/SDK/glm"
IncludeDir["stb_image"] = "%{wks.location}/BunnyEngine/SDK/stb_image"
IncludeDir["entt"] = "%{wks.location}/BunnyEngine/SDK/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/BunnyEngine/SDK/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/BunnyEngine/SDK/ImGuizmo"
IncludeDir["imgui_node_editor"] = "%{wks.location}/BunnyEngine/SDK/imgui-node-editor"
IncludeDir["assimp"] = "%{wks.location}/BunnyEngine/SDK/assimp/include"
IncludeDir["mono"] = "%{wks.location}/BunnyEngine/SDK/mono/include"
IncludeDir["PhysX"] = "%{wks.location}/BunnyEngine/SDK/PhysX/include"
IncludeDir["fmod"] = "%{wks.location}/BunnyEngine/SDK/fmod/inc"

LibDir = {}
LibDir["PhysXDebug"] = "%{wks.location}/BunnyEngine/SDK/PhysX/lib/Debug/"
LibDir["PhysXRelease"] = "%{wks.location}/BunnyEngine/SDK/PhysX/lib/Release/"
LibDir["fmodDebug"] = "%{wks.location}/BunnyEngine/SDK/fmod/lib/Debug"
LibDir["fmodRelease"] = "%{wks.location}/BunnyEngine/SDK/fmod/lib/Release"

LibFiles = {}
LibFiles["monoDebug"] = "%{wks.location}/BunnyEngine/SDK/mono/lib/Debug/mono-2.0-sgen.lib"
LibFiles["monoRelease"] = "%{wks.location}/BunnyEngine/SDK/mono/lib/Release/mono-2.0-sgen.lib"

LibFiles["PhysXDebug"] = "%{LibDir.PhysXDebug}/PhysX_static_64.lib"
LibFiles["PhysXCharacterKinematicDebug"] = "%{LibDir.PhysXDebug}PhysXCharacterKinematic_static_64.lib"
LibFiles["PhysXCommonDebug"] = "%{LibDir.PhysXDebug}/PhysXCommon_static_64.lib"
LibFiles["PhysXCookingDebug"] = "%{LibDir.PhysXDebug}/PhysXCooking_static_64.lib"
LibFiles["PhysXExtensionsDebug"] = "%{LibDir.PhysXDebug}/PhysXExtensions_static_64.lib"
LibFiles["PhysXFoundationDebug"] = "%{LibDir.PhysXDebug}/PhysXFoundation_static_64.lib"
LibFiles["PhysXPvdSDKDebug"] = "%{LibDir.PhysXDebug}/PhysXPvdSDK_static_64.lib"
LibFiles["PhysXVehicleDebug"] = "%{LibDir.PhysXDebug}/PhysXVehicle_static_64.lib"

LibFiles["PhysXRelease"] = "%{LibDir.PhysXRelease}/PhysX_static_64.lib"
LibFiles["PhysXCharacterKinematicRelease"] = "%{LibDir.PhysXRelease}PhysXCharacterKinematic_static_64.lib"
LibFiles["PhysXCommonRelease"] = "%{LibDir.PhysXRelease}/PhysXCommon_static_64.lib"
LibFiles["PhysXCookingRelease"] = "%{LibDir.PhysXRelease}/PhysXCooking_static_64.lib"
LibFiles["PhysXExtensionsRelease"] = "%{LibDir.PhysXRelease}/PhysXExtensions_static_64.lib"
LibFiles["PhysXFoundationRelease"] = "%{LibDir.PhysXRelease}/PhysXFoundation_static_64.lib"
LibFiles["PhysXPvdSDKRelease"] = "%{LibDir.PhysXRelease}/PhysXPvdSDK_static_64.lib"
LibFiles["PhysXVehicleRelease"] = "%{LibDir.PhysXRelease}/PhysXVehicle_static_64.lib"

LibFiles["fmodDebug"] = "%{LibDir.fmodDebug}/fmodL_vc.lib"
LibFiles["fmodRelease"] = "%{LibDir.fmodRelease}/fmod_vc.lib"


group "Dependencise"
	include "BunnyEngine/SDK/GLFW"
	include "BunnyEngine/SDK/Glad"
	include "BunnyEngine/SDK/imgui"
	include "BunnyEngine/SDK/assimp"
	include "BunnyEngine/SDK/yaml-cpp"
	include "BunnyEngine/SDK/imgui-node-editor"
group ""

project "BunnyEngine"
	location "BunnyEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"


	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "BEpch.h"
	pchsource "BunnyEngine/src/BEpch.cpp"

	buildoptions "/MTd"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

		"%{prj.name}/SDK/stb_image/**.h",
		"%{prj.name}/SDK/stb_image/**.cpp",

		"%{prj.name}/SDK/glm/glm/**.hpp",
		"%{prj.name}/SDK/glm/glm/**.inl",

		"%{prj.name}/SDK/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/SDK/ImGuizmo/ImGuizmo.cpp"
	}

	defines{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"PX_PHYSX_STATIC_LIB"
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/SDK/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.imgui_node_editor}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.mono}",
		"%{IncludeDir.PhysX}",
		"%{IncludeDir.fmod}"
	}

	links{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"imgui-node-editor",
		"assimp",
		"opengl32.lib"
	}

	filter "files:BunnyEngine/SDK/ImGuizmo/**.cpp"
		flags { "NoPCH"	}
	filter "system:windows"
		systemversion "latest"

		defines{
			"BE_PLATFORM_WINDOWS",
			"BE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}




	filter "configurations:Debug"
		defines "EG_DEBUG"
		runtime "Debug"
		symbols "on"
		libdirs
		{
			"%{LibDir.PhysXDebug}",
			"%{LibDir.fmodDebug}"
		}
		links
		{
			"%{LibFiles.PhysXDebug}",
			"%{LibFiles.PhysXCharacterKinematicDebug}",
			"%{LibFiles.PhysXCommonDebug}",
			"%{LibFiles.PhysXCookingDebug}", 
			"%{LibFiles.PhysXExtensionsDebug}",
			"%{LibFiles.PhysXFoundationDebug}",
			"%{LibFiles.PhysXPvdSDKDebug}",
			"%{LibFiles.PhysXVehicleDebug}",
			"%{LibFiles.fmodDebug}",
			"%{LibFiles.monoDebug}"
		}

	filter "configurations:Release"
		defines 
		{
			"EG_RELEASE",
			"NDEBUG"
		}
		libdirs
		{
			"%{LibDir.PhysXRelease}",
			"%{LibDir.fmodRelease}"
		}
		links
		{
			"%{LibFiles.PhysXRelease}",
			"%{LibFiles.PhysXCharacterKinematicRelease}",
			"%{LibFiles.PhysXCommonRelease}",
			"%{LibFiles.PhysXCookingRelease}", 
			"%{LibFiles.PhysXExtensionsRelease}",
			"%{LibFiles.PhysXFoundationRelease}",
			"%{LibFiles.PhysXPvdSDKRelease}",
			"%{LibFiles.PhysXVehicleRelease}",
			"%{LibFiles.fmodRelease}",
			"%{LibFiles.monoRelease}"
		}
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines 
		{
			"EG_DIST",
			"NDEBUG"
		}
		libdirs
		{
			"%{LibDir.PhysXRelease}",
			"%{LibDir.fmodRelease}"
		}
		links
		{
			"%{LibFiles.PhysXRelease}",
			"%{LibFiles.PhysXCharacterKinematicRelease}",
			"%{LibFiles.PhysXCommonRelease}",
			"%{LibFiles.PhysXCookingRelease}", 
			"%{LibFiles.PhysXExtensionsRelease}",
			"%{LibFiles.PhysXFoundationRelease}",
			"%{LibFiles.PhysXPvdSDKRelease}",
			"%{LibFiles.PhysXVehicleRelease}",
			"%{LibFiles.fmodRelease}",
			"%{LibFiles.monoRelease}"
		}
		runtime "Release"
		optimize "on"


project "BunnyEngineEditor"
	location "BunnyEngineEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs{
		"BunnyEngine/SDK/spdlog/include",
		"BunnyEngine/src",
		"BunnyEngine/SDK",
		"%{IncludeDir.glm}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGui}"
	}

	links{
		"BunnyEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines{
			"BE_PLATFORM_WINDOWS",
			"BE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}
		
	filter "configurations:Debug"
		defines "EG_DEBUG"
		runtime "Debug"
		symbols "on"

		postbuildcommands 
		{
			'{COPY} "../BunnyEngine/SDK/mono/bin/Debug/mono-2.0-sgen.dll" "%{cfg.targetdir}"',
			'{COPY} "../BunnyEngine/SDK/fmod/lib/Debug/fmodL.dll" "%{cfg.targetdir}"'
		}

	filter "configurations:Release"
		defines 
		{
			"EG_RELEASE",
			"NDEBUG"
		}
		runtime "Release"
		optimize "on"

		postbuildcommands 
		{
			'{COPY} "../BunnyEngine/SDK/mono/bin/Release/mono-2.0-sgen.dll" "%{cfg.targetdir}"',
			'{COPY} "../BunnyEngine/SDK/fmod/lib/Release/fmod.dll" "%{cfg.targetdir}"'
		}

	filter "configurations:Dist"
		defines 
		{
			"EG_DIST",
			"NDEBUG"
		}
		runtime "Release"
		optimize "on"

		postbuildcommands 
		{
			'{COPY} "../BunnyEngine/SDK/mono/bin/Release/mono-2.0-sgen.dll" "%{cfg.targetdir}"',
			'{COPY} "../BunnyEngine/SDK/fmod/lib/Release/fmod.dll" "%{cfg.targetdir}"'
		}