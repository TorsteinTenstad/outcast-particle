#include "utils.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "constants.hpp"
#include <assert.h>
#include <math.h>

bool IsMenu(const std::string& level_id)
{
	return level_id == MAIN_MENU || level_id == LEVEL_MENU || level_id == OPTIONS_MENU;
}

float FakeSigmoid(float x)
{
	assert(x >= 0);
	assert(x <= 1);
	if (x < 0.5)
	{
		return pow(2 * x, 3) / 2;
	}
	else
	{
		return 1 - abs(pow(2 * x - 2, 3)) / 2;
	}
}

float Magnitude(sf::Vector2f v)
{
	return sqrt((v.x * v.x) + (v.y * v.y));
}

sf::Vector2f Normalized(sf::Vector2f v)
{
	return v / Magnitude(v);
}

sf::Vector2f Abs(sf::Vector2f v)
{
	return sf::Vector2f(abs(v.x), abs(v.y));
}

sf::Vector2f GetQuarterTurnRotation(sf::Vector2f v)
{
	return sf::Vector2f(-v.y, v.x);
}

float Dot(sf::Vector2f u, sf::Vector2f v)
{
	return u.x * v.x + u.y * v.y;
}

std::vector<sf::Vector2f> GridHelper(int n_elements, int n_columns, float eliment_w, float eliment_h, float spacing)
{
	std::vector<sf::Vector2f> grid;
	int n_rows = ceil(((float)n_elements) / n_columns);
	int c = 0;
	int r = 0;
	float total_w = (n_columns - 1) * (eliment_w + spacing);
	float total_h = (n_rows - 1) * (eliment_h + spacing);
	for (int i = 0; i < n_elements; ++i)
	{
		float x = (eliment_w + spacing) * c - total_w / 2;
		float y = (eliment_h + spacing) * r - total_h / 2;
		grid.push_back(sf::Vector2f(x, y));
		c++;
		if (c == n_columns)
		{
			r++;
			c = 0;
		}
	}
	return grid;
}

std::string HumanName(sf::Keyboard::Key key)
{
	if (sf::Keyboard::A <= key && key <= sf::Keyboard::Z)
	{
		return std::string(1, 'A' + key);
	}
	if (sf::Keyboard::Num0 <= key && key <= sf::Keyboard::Num9)
	{
		return std::string(1, '0' + key);
	}
	switch (key)
	{
		case sf::Keyboard::Escape:
			return "Escape";
		case sf::Keyboard::LControl:
			return "LControl";
		case sf::Keyboard::LShift:
			return "LShift";
		case sf::Keyboard::LAlt:
			return "LAlt";
		case sf::Keyboard::RControl:
			return "RControl";
		case sf::Keyboard::RShift:
			return "RShift";
		case sf::Keyboard::RAlt:
			return "RAlt";
		case sf::Keyboard::Space:
			return "Space";
		case sf::Keyboard::Enter:
			return "Enter";
		case sf::Keyboard::Backspace:
			return "Backspace";
		case sf::Keyboard::Tab:
			return "Tab";
		case sf::Keyboard::Delete:
			return "Delete";
		case sf::Keyboard::Comma:
			return "Comma";
		case sf::Keyboard::Period:
			return "Period";

		default:
			return "Unknown key";
	}
}