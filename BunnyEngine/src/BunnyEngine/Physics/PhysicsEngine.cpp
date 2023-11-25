#include "BEpch.h"
#include "PhysicsEngine.h"
#include "PhysXInternal.h"

namespace BE
{
	void PhysicsEngine::Init()
	{
		PhysXInternal::Init();
	}

	void PhysicsEngine::Shutdown()
	{
		PhysXInternal::Shutdown();
	}
}
