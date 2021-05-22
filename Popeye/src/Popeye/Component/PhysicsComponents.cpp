#include "PhysicsComponents.h"

namespace Popeye
{
	BoxCollider::BoxCollider()
	{
		SetValue();
	}

	void BoxCollider::SetValue()
	{
		width = 1.0f;
		length = 1.0f;
		height = 1.0f;
	}

	Rigidbody::Rigidbody()
	{
		SetValue();
	}
	
	void Rigidbody::SetValue()
	{
		gravity = true;
		weight = 10.0f;
	}
}