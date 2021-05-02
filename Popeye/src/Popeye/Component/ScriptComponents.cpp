#include "ScriptComponents.h"

namespace Popeye
{
	void ScriptComponent::Init()	{}
	void ScriptComponent::Run()		{}
	void ScriptComponent::Exit()	{}


	void customscript::Init()
	{}

	void customscript::Run()
	{
		std::cout << "dddd" << std::endl;
	}

	void customscript::Exit()
	{}
}