#include "utils.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "constants.hpp"
#include "string_parsing_utils.hpp"
#include <algorithm>
#include <assert.h>
#include <filesystem>
#include <iostream>
#include <math.h>

float Smoothstep(float a, float b, float x)
{
	float t = Clamp((x - a) / (b - a), 0.f, 1.f);
	return t * t * (3.0 - 2.0 * t);
}

float Ease(float x, float undershoot, float overshoot)
{
	x = Clamp(x, 0.f, 1.f);
	float a = overshoot * x * x;
	float b = 1 - undershoot * (x - 1) * (x - 1);
	float s = Smoothstep(0, 1, x);
	float result = a * (1 - s) + b * s;
	assert(0 <= result);
	return result;
}

std::string GetGroupNameFromId(const std::string& level_id)
{
	std::filesystem::path path { level_id };
	return (++path.remove_filename().begin())->string();
}

std::string GetGroupDisplayNameFromGroupName(const std::string& group_name)
{
	return SplitString(group_name, "_").back();
}

std::string GetGroupDisplayNameFromId(const std::string& level_id)
{
	return GetGroupDisplayNameFromGroupName(GetGroupNameFromId(level_id));
}

std::string GetLevelDisplayNameFromId(const std::string& level_id)
{
	const std::filesystem::path path { level_id };
	return SplitString(path.filename().stem().string(), "_").back();
}

bool IsMenu(const std::string& level_id)
{
	return level_id == MAIN_MENU || level_id == LEVEL_MENU || level_id == OPTIONS_MENU || level_id == KEY_CONFIG_MENU;
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

std::array<int, 2> Vector2iToArray(sf::Vector2i v)
{
	return { v.x, v.y };
};

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

std::vector<sf::Vector2f> GridHelper(int n_elements, int n_columns, float spacing_w, float spacing_h)
{
	sf::Vector2f standard_button_size = sf::Vector2f(10, 2) * 120.f;
	float w = standard_button_size.x;
	float h = standard_button_size.y;
	std::vector<sf::Vector2f> grid;
	int n_rows = ceil(((float)n_elements) / n_columns);
	int c = 0;
	int r = 0;
	float total_w = (n_columns - 1) * (w + spacing_w);
	float total_h = (n_rows - 1) * (h + spacing_h);
	for (int i = 0; i < n_elements; ++i)
	{
		float x = (w + spacing_w) * c - total_w / 2;
		float y = (h + spacing_h) * r - total_h / 2;
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
			return ",";
		case sf::Keyboard::Period:
			return "Period";

		default:
			return "Unknown key";
	}
}

std::string OptionsButtonTextCreator(std::string options_name, std::string current_option)
{
	std::string button_string = options_name;
	float button_text_width = 0.5 * current_option.size();
	if (current_option.size() % 2 != 0)
	{
		button_text_width += 0.5;
	}
	int space_number = 30;
	assert(options_name.size() + button_text_width < space_number);
	button_string.append(space_number - options_name.size() - button_text_width, ' ');
	button_string += current_option;
	button_string.append(space_number, ' ');
	return (button_string);
}