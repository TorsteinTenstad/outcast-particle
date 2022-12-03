#pragma once

class Button
{
public:
	std::function<void(void)> on_click;
};