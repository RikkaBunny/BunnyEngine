#pragma once


#define BE_MOUSE_BUTTON_1         0
#define BE_MOUSE_BUTTON_2         1
#define BE_MOUSE_BUTTON_3         2
#define BE_MOUSE_BUTTON_4         3
#define BE_MOUSE_BUTTON_5         4
#define BE_MOUSE_BUTTON_6         5
#define BE_MOUSE_BUTTON_7         6
#define BE_MOUSE_BUTTON_8         7
#define BE_MOUSE_BUTTON_LAST      BE_MOUSE_BUTTON_8
#define BE_MOUSE_BUTTON_LEFT      BE_MOUSE_BUTTON_1
#define BE_MOUSE_BUTTON_RIGHT     BE_MOUSE_BUTTON_2
#define BE_MOUSE_BUTTON_MIDDLE    BE_MOUSE_BUTTON_3

namespace BE
{
	using uint16_t = unsigned short int;

	enum Mouse : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	};
}