#include "BEpch.h"
#include "Framebuffer.h"

#include "BunnyEngine/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace BE
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				BE_CORE_ASSERT(false, "RendererAPI::None currently is not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return MakeRef<OpenGLFramebuffer>(spec);
		}
		BE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}