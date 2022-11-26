#pragma once
#include "PCH.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utilityfunctions.hpp"

template <class T>
void Animate(float time, T& property, std::map<float, T>& animation, std::function<float(float)> interpolation_behaviour)
{
	if (animation.size() == 0)
	{
		return;
	}
	assert(animation.size() > 1);
	if (time < animation.begin()->first)
	{
		return;
	}
	while (time > (++animation.begin())->first)
	{
		animation.erase(animation.begin());
		if (animation.size() == 1)
		{
			animation.clear();
			return;
		}
	}
	float t0 = animation.begin()->first;
	float t1 = (++animation.begin())->first;
	T v0 = animation.begin()->second;
	T v1 = (++animation.begin())->second;
	property = v0 + interpolation_behaviour((time - t0) / (t1 - t0)) * (v1 - v0);
}

class ScreenShakeSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		float screen_size_shake = 1;
		Animate(globals.time, screen_size_shake, level.screen_size_shake_animation, FakeSigmoid);
		globals.render_window.setView(sf::View(level.size / 2.f, level.size * screen_size_shake));
	}
};