#include "BEpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace BE
{
	Ref<RendererAPI> RenderCommand::s_RendererAPI = MakeRef<OpenGLRendererAPI>();
}
