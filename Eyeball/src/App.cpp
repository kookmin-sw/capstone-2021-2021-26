#include <Popeye.h>

class Game : public Popeye::Application
{
public:
	Game()
	{}

	~Game()
	{
	}
};

Popeye::Application* Popeye::CreateApplication()
{
	return new Game();
}