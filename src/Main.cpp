#include "SFML.h"
#include "game.hpp"
#include "globals.hpp"

static Globals globals_;
Globals& globals = globals_;

class A
{
public:
	int x = 1;
	int y = 2;
};
class B
{
public:
	int x = 3;
	int z = 4;
};

int main()
{

	std::vector<std::type_index> a;
	a.push_back(typeid(B));
	std::variant<A, B> v;
	B t;
	v = t;
	std::visit([](auto& var) { if(var.x==1){std::cout << var.x << "\n"; } }, v);

	SFML sfml_app = SFML();
	Game game = Game();
	sfml_app.RunWindow(game);
	return 0;
}
