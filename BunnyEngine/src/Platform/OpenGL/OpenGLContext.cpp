#include "BEpch.h"

#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace BE
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		BE_CORE_ASSERT(m_WindowHandle, "OpenGL WindowHandle is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		BE_CORE_ASSERT(status, "Failed to initialize Glad!");

		BE_CORE_INFO("OpenGL Info:");
		BE_CORE_INFO("\tVendor: {0}", glGetString(GL_VENDOR));
		BE_CORE_INFO("\tRenderer: {0}", glGetString(GL_RENDERER));
		BE_CORE_INFO("\tVersion: {0}", glGetString(GL_VERSION));

		#ifdef BE_ENABLE_ASSERTS
			int versionMajor;
			int versionMinor;
			glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
			glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

			BE_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "BunnyEngine requires at least OpenGL version 4.5!");
		#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}