#pragma once
namespace Popeye {
	//-------------------------------------------
	// Physics Component : Box Collider 
	//-------------------------------------------
	struct BoxCollider {
		float width;
		float length; 
		float height;


		BoxCollider();
		void SetValue();
		//float pos;

		//Cube_Collider();

		//float CubeVolume(float width, float length, float height);
		//bool IsCollision(float position);

	};

	//-------------------------------------------
	// Physics Component : Rigidbody
	//-------------------------------------------
	struct Rigidbody {
		bool gravity;
		float weight;

		Rigidbody();
		void SetValue();

		//Rigidbody();

		//int PhysicCal(??);
		//bool IsGravity(??);

	};
}
