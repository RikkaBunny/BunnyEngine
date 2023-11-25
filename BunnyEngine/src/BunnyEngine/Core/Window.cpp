#include "BEpch.h"
#include "Window.h"

#ifdef BE_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace BE
{
	Ref<Window> Window::Create(const WindowProps& props)
	{
		#ifdef BE_PLATFORM_WINDOWS
			return MakeRef<WindowsWindow>(props);
		#else
			BE_CORE_ASSERT(false, "Unknown platform!");
			return nullptr;
		#endif
	}
}