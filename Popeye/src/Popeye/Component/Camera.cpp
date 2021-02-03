#include "pch.h"
#include "Camera.h"

namespace Popeye
{
	Cam::Cam(){}

	std::unordered_map<int, Cam> Camera::cameras;

	void Camera::ComponentAdded(int id)
	{
		if (cameras.find(id) == cameras.end())
		{
			cameras[id] = Cam();
		}
	}
}