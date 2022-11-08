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
	std::string to_string()
	{
		return std::to_string(x) + " " + std::to_string(y);
	}
};
class B

{

public:
	int x = 3;
	std::string to_string()
	{
		return std::to_string(x);
	}
};

int main()
{
	std::map<const char*, std::variant<std::map<int, A>, std::map<int, B>, std::map<int, Position>>> components;
	std::map<int, A> a_map;
	A a;
	a.x = 53;
	a_map[0] = a;
	a_map[1];
	std::map<int, B> b_map;
	B b;
	b.x = 11;
	b_map[0] = b;
	b_map[1];
	std::map<int, Position> p_map;
	p_map[0] = { sf::Vector2f(23, 23) };
	components[typeid(A).name()] = a_map;
	components[typeid(B).name()] = b_map;
	components[typeid(Position).name()] = p_map;
	for (auto& [component_typeid_name_, component_map_variant] : components)
	{
		const char* component_typeid_name = component_typeid_name_;
		std::visit([component_typeid_name](auto& component_map) {
			for (const auto& [entity_id, component] : component_map)
			{
				std::string unique_id = "content/levels/" + std::to_string(entity_id) + "-" + std::string(component_typeid_name);
				SaveToBinaryFile(component, unique_id.c_str());
			}
		},
			component_map_variant);
	}

	std::map<const char*, std::variant<std::map<int, A>, std::map<int, B>>> components2;
	std::map<int, A> a_map2;
	std::map<int, B> b_map2;
	std::map<int, Position> p_map2;
	components[typeid(A).name()] = a_map2;
	components[typeid(B).name()] = b_map2;
	components[typeid(Position).name()] = p_map2;

	for (const auto& entry : std::experimental::filesystem::directory_iterator("content/levels/"))
	{
		std::string filename = entry.path().filename().string();
		size_t separator_index = filename.find("-");
		if (separator_index != std::string::npos)
		{
			int entity_id = stoi(filename.substr(0, separator_index));
			std::string component_typeid_name = filename.substr(separator_index + 1, filename.length());
			std::cout << entity_id << "," << component_typeid_name << "\n";
			std::visit([entry, entity_id, component_typeid_name](auto& component_map) {
				LoadFromBinaryFile(component_map[entity_id], entry.path().string().c_str());
			},
				components2[component_typeid_name.c_str()]);
		}
	}

	for (auto& [component_typeid_name_, component_map_variant] : components2)
	{
		const char* component_typeid_name = component_typeid_name_;
		std::visit([component_typeid_name](auto& component_map) {
			for (auto& [entity_id, component] : component_map)
			{

				std::cout << entity_id << ":" << component.to_string() << "\n";
			}
		},
			component_map_variant);
	}
	return 0;
	SFML sfml_app = SFML();
	Game game = Game();
	sfml_app.RunWindow(game);
	return 0;
}
