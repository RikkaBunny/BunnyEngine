#include "BunnyEngineEditorApp.h"
#include "EditorLayer.h"

#include <BunnyEngine/Core/Core.h>
#include <BunnyEngine/Core/EntryPoint.h>

namespace BE
{
	BunnyEngineEditor::BunnyEngineEditor() : Application("BunnyEngine Editor")
	{
		PushLayer(new EditorLayer());
	}

	void BunnyEngineEditor::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BE_BIND_FN(BunnyEngineEditor::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BE_BIND_FN(BunnyEngineEditor::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;

			(*it)->OnEvent(e);
		}
	}

	bool BunnyEngineEditor::OnWindowResize(WindowResizeEvent& e)
	{
		const uint32_t width = e.GetWidth();
		const uint32_t height = e.GetHeight();

		if (width == 0 || height == 0)
		{
			m_Minimized = true;
		}
		else
		{
			m_Minimized = false;
		}

		//Disabled so EditorLayer can control vieportsize
		//Renderer::WindowResized(width, height);

		return false;
	}

	Application* CreateApplication()
	{
		return new BunnyEngineEditor();
	}
}
