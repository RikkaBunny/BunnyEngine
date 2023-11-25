#pragma once

#include <memory>

#include "PlatformDetection.h"
#include "DelayCall.h"

#define BE_VERSION "0.5"
#define BE_VERSION_MAJOR 0
#define BE_VERSION_MINOR 5
#define BE_VERSION_PATCH 0
#define BE_EDITOR 1

#define BE_HOVER_THRESHOLD 0.5f

#ifdef BE_PLATFORM_WINDOWS
#if BE_DYNAMIC_LINK
	#ifdef BE_BUILD_DLL
		#define BE_API __declspec(dllexport)
	#else
		#define BE_API __declspec(dllimport)
	#endif // BE_BUILD_DLL
#else
	#define BE_API
#endif
#else 
	#error BunnyEngine only support Windows!
#endif

#ifdef BE_ENABLE_ASSERTS
	#define BE_ASSERT(x,...) {if(!(x)) { BE_ERROR("Assertion Failed: {0}", __VA_VRGS__); __debugbreak();} }
	#define BE_CORE_ASSERT(x,...) { if(!(x)) { BE_CORE_ERROR("Assertion Failed: {0}", __VA_VRGS__); __debugbreak();} }
#else
	#define BE_ASSERT(x,...)
	#define BE_CORE_ASSERT(x,...)
#endif

#define BIT(x) (1 << x)

#define BE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define BE_BIND_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define BE_SET_TIMER_BY_FUNC(fn, ms, ...) (new ::BE::DelayCall(std::bind(&fn, this, __VA_ARGS__), ms))

namespace BE
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename Type, class... Args>
	constexpr Scope<Type> MakeScope(Args&&... args)
	{
		return std::make_unique<Type>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename Type, class... Args>
	constexpr Ref<Type> MakeRef(Args&&... args)
	{
		return std::make_shared<Type>(std::forward<Args>(args)...);
	}

	template<typename ToType, typename FromType>
	constexpr Ref<ToType> Cast(const Ref<FromType>& ref)
	{
		return std::dynamic_pointer_cast<ToType>(ref);
	}
}
