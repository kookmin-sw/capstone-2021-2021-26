#pragma once
#include "Popeye/Component/ScriptComponents.h"
namespace Popeye
{
	class customscript : public ScriptComponent
	{
	public:
		void Init();
		void Run();
		void Exit();
	};
}

