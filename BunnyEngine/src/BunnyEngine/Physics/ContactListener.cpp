#include "BEpch.h"
#include "ContactListener.h"
#include "PhysicsActor.h"
//#include "BunnyEngine/Script/ScriptEngine.h"

namespace BE
{
	void ContactListener::onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32)
	{
	}
	
	void ContactListener::onWake(physx::PxActor** actors, physx::PxU32 count)
	{
		for (uint32_t i = 0; i < count; ++i)
		{
			physx::PxActor& physxActor = *actors[i];
			PhysicsActor* actor = (PhysicsActor*)physxActor.userData;
			const Entity& entity = actor->GetEntity();
			BE_CORE_INFO("[Physics Engine] Physics Actor is waking up. Name {0}", entity.GetComponent<EntitySceneNameComponent>().Name);
		}
	}
	
	void ContactListener::onSleep(physx::PxActor** actors, physx::PxU32 count)
	{
		for (uint32_t i = 0; i < count; ++i)
		{
			physx::PxActor& physxActor = *actors[i];
			PhysicsActor* actor = (PhysicsActor*)physxActor.userData;
			const Entity& entity = actor->GetEntity();
			BE_CORE_INFO("[Physics Engine] Physics Actor is going to sleep. Name {0}", entity.GetComponent<EntitySceneNameComponent>().Name);
		}
	}
	
	void ContactListener::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
	{
		if (!Scene::GetCurrentScene()->IsPlaying())
			return;

		auto removedActorA = pairHeader.flags & physx::PxContactPairHeaderFlag::eREMOVED_ACTOR_0;
		auto removedActorB = pairHeader.flags & physx::PxContactPairHeaderFlag::eREMOVED_ACTOR_1;

		if (removedActorA || removedActorB)
			return;

		PhysicsActor* actorA = (PhysicsActor*)pairHeader.actors[0]->userData;
		PhysicsActor* actorB = (PhysicsActor*)pairHeader.actors[1]->userData;

	}
	
	void ContactListener::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
		if (!Scene::GetCurrentScene()->IsPlaying())
			return;

		for (uint32_t i = 0; i < count; ++i)
		{
			if (pairs[i].flags & (physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
				continue;
			
			PhysicsActor* triggerActor = (PhysicsActor*)pairs[i].triggerActor->userData;
			PhysicsActor* otherActor = (PhysicsActor*)pairs[i].otherActor->userData;

			
		}
	}
	
	void ContactListener::onAdvance(const physx::PxRigidBody* const*, const physx::PxTransform*, const physx::PxU32)
	{
	}
}
