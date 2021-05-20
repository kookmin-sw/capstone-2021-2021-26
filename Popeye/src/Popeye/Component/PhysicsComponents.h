#pragma once
namespace Popeye {
	//-------------------------------------------
	//TODO :: Cube Collider
	//-------------------------------------------
	struct Cube_Collider {
		float W;
		float L; 
		float H;
		float pos;

		//Cube_Collider();

		//float CubeVolume(float width, float length, float height);
		//bool IsCollision(float position);

	};

	//-------------------------------------------
	//TODO :: Rigidbody
	//-------------------------------------------
	struct Rigidbody {
		bool gravity;
		float weight;

		//Rigidbody();

		//int PhysicCal(??);
		//bool IsGravity(??);

	};
}
