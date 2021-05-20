#pragma once

namespace Popeye
{
	struct ScriptComponent
	{
		void Init();
		void Run();
		void Exit();
	};

	class customscript : public ScriptComponent
	{
	public:
		void Init();
		void Run();
		void Exit();
	};
}