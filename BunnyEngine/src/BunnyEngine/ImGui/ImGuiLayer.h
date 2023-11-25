#pragma once

#include "BunnyEngine/Core/Layer.h"
#include "BunnyEngine/Events/ApplicationEvent.h"
#include "BunnyEngine/Events/KeyEvent.h"
#include "BunnyEngine/Events/MouseEvent.h"

namespace BE {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		//virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();
		
		void BlockEvents(bool blockEvents) { m_BlockEvents = blockEvents; }
	private:
		void SetDarkThemeColors();
		void SetDarkTheme1Colors();
		void SetLightThemeColors();
	private:
		float m_Time = 0.0f;
		bool m_BlockEvents = false;
	};

}


