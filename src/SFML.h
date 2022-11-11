#include "Platform/Platform.hpp"
#include "timer.hpp"

class SFML
{
private:
public:
	SFML();
	void RunWindow(std::function<void(float)> update_func);
};