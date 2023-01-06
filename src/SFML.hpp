#include "timer.hpp"
#include <functional>

class SFML
{
private:
public:
	SFML();
	void RunWindow(std::function<void(float)> update_func);
};