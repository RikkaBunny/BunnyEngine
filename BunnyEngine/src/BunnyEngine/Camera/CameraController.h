#pragma once

#include "BunnyEngine/Core/ScriptableEntity.h"

namespace BE
{
	class MouseScrolledEvent;

	class CameraController : public ScriptableEntity
	{
	protected:
		virtual void OnCreate() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& e) override;

	protected:
		glm::vec3 m_EulerRotation = glm::vec3(0.f);
		float m_MouseX = 0.f;
		float m_MouseY = 0.f;

		float m_MoveSpeed = 16.75f;
		float m_MouseRotationSpeed = 0.032f;
	};
}