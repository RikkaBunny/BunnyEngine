#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "BunnyEngine/Core/Timestep.h"

namespace BE
{
	class Event;

	enum class CameraProjectionMode : uint8_t
	{
		Perspective = 0,
		Orthographic = 1
	};

	class Camera
	{
	public:
		virtual ~Camera() = default;

		virtual const glm::mat4& GetProjection() const { return m_Projection; }

	protected:
		glm::mat4 m_Projection = glm::mat4(1.f);
	};

}