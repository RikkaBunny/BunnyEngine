#pragma once

#include "BunnyEngine/Core/Notifications.h"

namespace BE
{
	class Object
	{
	public:
		Object() = default;
		Object(const Object&) = default;
		Object(Object&&) = default;

		Object& operator=(const Object&) = default;
		Object& operator=(Object&&) = default;
		virtual ~Object() = default;

		virtual void OnNotify(Notification notification) {}
	};
}
