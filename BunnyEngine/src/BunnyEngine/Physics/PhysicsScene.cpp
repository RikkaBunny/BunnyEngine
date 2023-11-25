#include "BEpch.h"
#include "PhysicsScene.h"
#include "PhysicsEngine.h"
#include "PhysicsSettings.h"
#include "PhysXDebugger.h"
#include "PhysXInternal.h"
#include "ContactListener.h"
#include "BunnyEngine/Core/Project.h"

namespace BE
{
    static ContactListener s_ContactListener;

    PhysicsScene::PhysicsScene(const PhysicsSettings& settings)
    : m_Settings(settings)
    , m_SubstepSize(settings.FixedTimeStep)
    {
        physx::PxSceneDesc sceneDesc(PhysXInternal::GetPhysics().getTolerancesScale());
        sceneDesc.dynamicTreeRebuildRateHint *= 5;
        sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD | physx::PxSceneFlag::eENABLE_PCM;
        sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ENHANCED_DETERMINISM;
        sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS;
        sceneDesc.gravity = PhysXUtils::ToPhysXVector(settings.Gravity);
        sceneDesc.broadPhaseType = PhysXUtils::ToPhysXBroadphaseType(settings.BroadphaseAlgorithm);
        sceneDesc.cpuDispatcher = PhysXInternal::GetCPUDispatcher();
        sceneDesc.filterShader = m_Settings.EditorScene ? (physx::PxSimulationFilterShader)PhysXInternal::EditorFilterShader :(physx::PxSimulationFilterShader)PhysXInternal::FilterShader;
        sceneDesc.simulationEventCallback = &s_ContactListener;
        sceneDesc.frictionType = PhysXUtils::ToPhysXFrictionType(settings.FrictionModel);

        BE_CORE_ASSERT(sceneDesc.isValid(), "Invalid scene desc");

        m_Scene = PhysXInternal::GetPhysics().createScene(sceneDesc);
        BE_CORE_ASSERT(m_Scene, "Invalid scene");
        m_Scene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.f);
        m_Scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 2.f);

        CreateRegions();

        #ifdef BE_DEBUG
        if (settings.DebugOnPlay && !PhysXDebugger::IsDebugging())
            PhysXDebugger::StartDebugging(Project::GetSavedPath() / "PhysXDebugInfo", settings.DebugType == DebugType::Live);
        #endif
    }
    
    void PhysicsScene::ConstructFromScene(Scene* scene)
    {
        for (auto& it : scene->GetAliveEntities())
        {
            Entity& entity = it.second;
            CreatePhysicsActor(entity);
        }
    }

    void PhysicsScene::Simulate(Timestep ts, bool bFixedUpdate)
    {
        if (bFixedUpdate)
        {
            for (auto& actor : m_Actors)
                actor.second->OnFixedUpdate(ts);
        }

        bool bAdvanced = Advance(ts);
        if (bAdvanced)
        {
            uint32_t nActiveActors;
            physx::PxActor** activeActors = m_Scene->getActiveActors(nActiveActors);
            for (uint32_t i = 0; i < nActiveActors; ++i)
            {
                PhysicsActor* activeActor = (PhysicsActor*)activeActors[i]->userData;
                activeActor->SynchronizeTransform();
            }
        }
    }
    
    Ref<PhysicsActor>& PhysicsScene::GetPhysicsActor(const Entity& entity)
    {
        Ref<PhysicsActor> s_InvalidPhysicsActor = nullptr;
        auto it = m_Actors.find(entity.GetGUID());
        return it != m_Actors.end() ? it->second : s_InvalidPhysicsActor;
    }

    const Ref<PhysicsActor>& PhysicsScene::GetPhysicsActor(const Entity& entity) const
    {
        static Ref<PhysicsActor> s_InvalidPhysicsActor = nullptr;
        auto it = m_Actors.find(entity.GetGUID());
        return it != m_Actors.end() ? it->second : s_InvalidPhysicsActor;
    }
    
    Ref<PhysicsActor> PhysicsScene::CreatePhysicsActor(Entity& entity)
    {
        static const Ref<PhysicsActor> s_InvalidActor;
        const bool bHasRigidBody = entity.HasComponent<RigidBodyComponent>();
        const bool bHasAnyCollider = entity.HasAny<BoxColliderComponent, SphereColliderComponent, CapsuleColliderComponent, MeshColliderComponent>();

        if (!bHasAnyCollider && !bHasRigidBody)
            return s_InvalidActor;

        if (!bHasRigidBody)
            entity.AddComponent<RigidBodyComponent>();

        Ref<PhysicsActor> actor = MakeRef<PhysicsActor>(entity, m_Settings);
        m_Actors[entity.GetGUID()] = actor;
        m_Scene->addActor(*actor->m_RigidActor);

        actor->SetSimulationData();
        
        return actor;
    }
    
    void PhysicsScene::RemovePhysicsActor(const Ref<PhysicsActor>& physicsActor)
    {
        if (!physicsActor)
            return;

        physicsActor->RemoveAllColliders();
        m_Scene->removeActor(*physicsActor->m_RigidActor);
        physicsActor->m_RigidActor->release();
        physicsActor->m_RigidActor = nullptr;
        m_Actors.erase(physicsActor->GetEntity().GetGUID());
    }
    
    bool PhysicsScene::Raycast(const glm::vec3& origin, const glm::vec3& dir, float maxDistance, RaycastHit* outHit)
    {
        physx::PxRaycastBuffer hitInfo;
        bool bResult = m_Scene->raycast(PhysXUtils::ToPhysXVector(origin), PhysXUtils::ToPhysXVector(dir), maxDistance, hitInfo);

        if (bResult)
        {
            PhysicsActor* actor = (PhysicsActor*)hitInfo.block.actor->userData;
            outHit->HitEntity = actor->GetEntity().GetGUID();
            outHit->Position = PhysXUtils::FromPhysXVector(hitInfo.block.position);
            outHit->Normal = PhysXUtils::FromPhysXVector(hitInfo.block.normal);
            outHit->Distance = hitInfo.block.distance;
        }

        return bResult;
    }
    
    bool PhysicsScene::OverlapBox(const glm::vec3& origin, const glm::vec3& halfSize, std::array<physx::PxOverlapHit, OVERLAP_MAX_COLLIDERS>& buffer, uint32_t& count)
    {
        return OverlapGeometry(origin, physx::PxBoxGeometry(halfSize.x, halfSize.y, halfSize.z), buffer, count);
    }
    
    bool PhysicsScene::OverlapCapsule(const glm::vec3& origin, float radius, float halfHeight, std::array<physx::PxOverlapHit, OVERLAP_MAX_COLLIDERS>& buffer, uint32_t& count)
    {
        return OverlapGeometry(origin, physx::PxCapsuleGeometry(radius, halfHeight), buffer, count);
    }
    
    bool PhysicsScene::OverlapSphere(const glm::vec3& origin, float radius, std::array<physx::PxOverlapHit, OVERLAP_MAX_COLLIDERS>& buffer, uint32_t& count)
    {
        return OverlapGeometry(origin, physx::PxSphereGeometry(radius), buffer, count);
    }
    
    void PhysicsScene::CreateRegions()
    {
        const PhysicsSettings& settings = m_Settings;

        if (settings.BroadphaseAlgorithm == BroadphaseType::AutomaticBoxPrune)
            return;
        
        physx::PxBounds3* regionBounds = new physx::PxBounds3[(uint64_t)settings.WorldBoundsSubdivisions * settings.WorldBoundsSubdivisions];
        physx::PxBounds3 globalBounds(PhysXUtils::ToPhysXVector(settings.WorldBoundsMin), PhysXUtils::ToPhysXVector(settings.WorldBoundsMax));
        uint32_t regionCount = physx::PxBroadPhaseExt::createRegionsFromWorldBounds(regionBounds, globalBounds, settings.WorldBoundsSubdivisions);

        for (uint32_t i = 0; i < regionCount; ++i)
        {
            physx::PxBroadPhaseRegion region;
            region.bounds = regionBounds[i];
            m_Scene->addBroadPhaseRegion(region);
        }
    }
    
    bool PhysicsScene::Advance(Timestep ts)
    {
        SubstepStrategy(ts);

        for (uint32_t i = 0; i < m_NumSubsteps; ++i)
        {
            m_Scene->simulate(m_SubstepSize);
            m_Scene->fetchResults(true);
        }
        return m_NumSubsteps != 0;
    }
    
    void PhysicsScene::SubstepStrategy(Timestep ts)
    {
        if (m_Accumulator > m_SubstepSize)
            m_Accumulator = 0.f;

        m_Accumulator += ts;
        if (m_Accumulator < m_SubstepSize)
        {
            m_NumSubsteps = 0;
            return;
        }

        m_NumSubsteps = glm::min((uint32_t)(m_Accumulator / m_SubstepSize), c_MaxSubsteps);
        m_Accumulator -= m_NumSubsteps * m_SubstepSize;
    }
    
    void PhysicsScene::Clear()
    {
        if (m_Scene)
        {
            while (m_Actors.size())
                RemovePhysicsActor(m_Actors.begin()->second);

            m_Actors.clear(); //Just in case
        }
    }

    void PhysicsScene::Reset()
    {
        Clear();
        m_Accumulator = 0.f;
        m_NumSubsteps = 0;
    }

    void PhysicsScene::Destroy()
    {
        if (m_Scene)
        {
        #ifdef BE_DEBUG
            if (m_Settings.DebugOnPlay && PhysXDebugger::IsDebugging())
                PhysXDebugger::StopDebugging();
        #endif
            while(m_Actors.size())
                RemovePhysicsActor(m_Actors.begin()->second);

            m_Actors.clear(); //Just in case
            m_Scene->release();
            m_Scene = nullptr;
        }
    }
    
    bool PhysicsScene::OverlapGeometry(const glm::vec3& origin, const physx::PxGeometry& geometry, std::array<physx::PxOverlapHit, OVERLAP_MAX_COLLIDERS>& buffer, uint32_t& count)
    {
        physx::PxOverlapBuffer buf(buffer.data(), OVERLAP_MAX_COLLIDERS);
        physx::PxTransform pose = PhysXUtils::ToPhysXTranform(glm::translate(glm::mat4(1.f), origin));

        bool bResult = m_Scene->overlap(geometry, pose, buf);

        if (bResult)
        {
            memcpy(buffer.data(), buf.touches, buf.nbTouches * sizeof(physx::PxOverlapHit));
            count = buf.nbTouches;
        }

        return bResult;
    }
}