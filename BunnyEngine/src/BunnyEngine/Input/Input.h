#pragma once

#include "BunnyEngine/Input/KeyCodes.h"
#include "BunnyEngine/Input/MouseButtonCodes.h"

namespace BE
{
	enum class CursorMode
	{
		Normal,
		Hidden,
		Locked
	};

	class Input
	{
	protected:
		Input() = default;

	public:
		Input(const Input&) = delete;
		Input& operator= (const Input&) = delete;

	public:
		static bool IsKeyPressed(Key keyCode);

		static bool IsMouseButtonPressed(Mouse mouseButton);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

		static void SetShowCursor(bool bShow);
		static void SetCursorPos(double xPos, double yPos);

		static bool IsCursorVisible();
	};
}