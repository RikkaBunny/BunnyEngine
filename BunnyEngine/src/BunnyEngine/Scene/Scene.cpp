#include "BEpch.h"
#include "Scene.h"

#include <glm/glm.hpp>

#include "BunnyEngine/Renderer/Renderer2D.h"
#include "BunnyEngine/Renderer/UniformBuffer.h"
#include "BunnyEngine/Renderer/RenderCommand.h"
#include "Entity.h"
#include "Components.h"
#include "BunnyEngine/Renderer/Light.h"
#include <glm/gtx/rotate_vector.hpp>

namespace BE {

	static void DoMath(const glm::mat4& transform) {

	}

	static void OnTransformConstruct(entt::registry& registry, entt::entity entity) {

	}

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& entityName = entity.AddComponent<NameComponent>();
		entityName.Name = name;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdateEditor(EditorCamera& camera)
	{
		//Renderer2D::InitQuad();
		Renderer2D::BeginScene(camera);
		LightDataManage();
		
		{
			auto view = m_Registry.view<MeshComponent, MaterialComponent>();
			for (auto entity : view) {
				auto [mesh, material] = view.get<MeshComponent, MaterialComponent>(entity);
				TransformComponent transfrom = m_Registry.get<TransformComponent>(entity);
				Ref<Shader> m_Shader = material.Mat.GetShader();
				
				if (!m_Shader)
					continue;

				material.Mat.BindShader();
				glm::mat4 viewProj = camera.GetViewProjection();
				m_Shader->Bind();
				m_Shader->SetFloat("u_Emissive", 2.0f);
				m_Shader->SetMat4("u_WorldTransform", transfrom.GetTransform());
				m_Shader->SetMat4("u_ViewProjection", viewProj);
				RenderCommand::DrawIndexed(mesh.MeshSource.GetMeshSource());
				
			}
		}
		Renderer2D::EndScene();
		
	}

	void Scene::OnUpdateEditorSelect(EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto view = m_Registry.view<MeshComponent, TransformComponent>();
		for (auto entity : view) {
			auto [mesh, transfrom] = view.get<MeshComponent, TransformComponent>(entity);
			//TransformComponent transfrom = m_Registry.get<TransformComponent>(entity);
			Ref<Shader> m_Shader = ShaderLibray::Get("GameEditorSelect", "Assets/shaders/GameEditorSelect.glsl");

			if (!m_Shader)
				continue;

			
			glm::mat4 viewProj = camera.GetViewProjection();
			m_Shader->Bind();
			m_Shader->SetMat4("u_WorldTransform", transfrom.GetTransform());
			m_Shader->SetMat4("u_ViewProjection", viewProj);
			m_Shader->SetInt("u_EntityID", (uint32_t)entity);
			RenderCommand::DrawIndexed(mesh.MeshSource.GetMeshSource());

		}
	}



	void Scene::OnUpdateRuntime()
	{
		//Update scripts
		{
			m_Registry.view<ScriptComponent>().each([=](auto entity, auto& scriptComponent) {
				//TODO: Move to Scene::OnScenePlay
				if (!scriptComponent.Instance) {
					scriptComponent.Instance = scriptComponent.InstantiateScript();
					scriptComponent.Instance->m_Entity = Entity{ entity, this };
					scriptComponent.Instance->OnCreate();
				}				
				scriptComponent.Instance->OnUpdate();
				});
		}

		//Render Scene
		SceneCamera* mainCamera = nullptr;
		glm::mat4 mainCamerTransform ;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view) {
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Main) {
					mainCamera = &camera.Camera;
					mainCamerTransform = transform.GetTransform();
					break;
				}
			}
		}
		if (!mainCamera)
			return;

//////////////////////////////////////////////////if mainCamera null, return;
//////////////////////Scence Actor Render;

		Renderer2D::BeginScene(*mainCamera, mainCamerTransform);

		LightDataManage();

		{
			auto view = m_Registry.view<MeshComponent, MaterialComponent>();
			for (auto entity : view) {
				auto [mesh, material] = view.get<MeshComponent, MaterialComponent>(entity);
				TransformComponent transfrom = m_Registry.get<TransformComponent>(entity);
				Ref<Shader> m_Shader = material.Mat.GetShader();

				if (!m_Shader)
					continue;

				material.Mat.BindShader();
				glm::mat4 viewProj = mainCamera->GetProjection() * glm::inverse(mainCamerTransform);
				m_Shader->Bind();
				m_Shader->SetFloat("u_Emissive", 2.0f);
				m_Shader->SetMat4("u_WorldTransform", transfrom.GetTransform());
				m_Shader->SetMat4("u_ViewProjection", viewProj);
				RenderCommand::DrawIndexed(mesh.MeshSource.GetMeshSource());

			}
		}

		Renderer2D::EndScene();
		
	}


	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio) {
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	Entity Scene::GetMainCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Main)
				return Entity{ entity, this };
		}
		return {};
	}

	void Scene::LightDataManage()
	{

		///////////////////////////////////////////DirctionLightComponent

		auto view = m_Registry.view<TransformComponent, DirctionLightComponent>();
		for (auto entity : view) {
			auto [transform, dirctionLight] = view.get<TransformComponent, DirctionLightComponent>(entity);
			LightDirection lightDir;
			lightDir.lightColor = glm::vec4(dirctionLight.DirctionColor, 1.0);
			lightDir.lightDir = glm::vec4(transform.Rotation, 1.0);
			lightDir.lightDirPos = glm::vec4(transform.Translation, 1.0);
			glm::vec3 dirction = glm::vec3(0.0f, 0.0f, 1.0f);
			dirction = glm::rotateX(dirction, transform.Rotation.x);
			dirction = glm::rotateY(dirction, transform.Rotation.y);
			dirction = glm::rotateZ(dirction, transform.Rotation.z);
			//dirction = -1.0f * dirction;
			lightDir.lightDir = glm::vec4(dirction, 1.0);

			UniformBuffer::CreateUniformBuffer<LightDirection>("LightDirection", lightDir);
		}
	}


}