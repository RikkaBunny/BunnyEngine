#pragma once

#include "BunnyEngine.h"

namespace BE
{
	class BunnyEngineEditor : public Application
	{
	public:
		BunnyEngineEditor();

	protected:
		virtual void OnEvent(Event& e) override;
		virtual bool OnWindowResize(WindowResizeEvent& e) override;
	};
}
