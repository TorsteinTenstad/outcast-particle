#include "components/draw_info.hpp"
#include "components/physics.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "entity_creation.hpp"
#include "make_fade_into_level.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/math.hpp"

#define MINIMUM_PLAYER_VELOCITY_REQUIRED_FOR_FACE_MOVEMENT 10

void FaceSystem::Update(Level& level, float dt)
{
	for (auto& [entity, face, children, draw_info, draw_priority, radius, position] : level.GetEntitiesWith<Face, Children, DrawInfo, DrawPriority, Radius, Position>())
	{

		std::function<Entity(Level&)> create_face = [active_level_id = active_level_id_, face = face, draw_priority = draw_priority, radius = radius, position = position](Level& level) {
			Entity entity = level.CreateEntity();
			level.AddComponent<DrawInfo>(entity);
			level.AddComponent<DrawPriority>(entity)->draw_priority = draw_priority->draw_priority + 1;
			level.AddComponent<Radius>(entity)->radius = radius->radius;
			level.AddComponent<Position>(entity)->position = position->position;
			MakeFadeIntoLevel(level, entity, active_level_id);
			return entity;
		};

		Entity child = GetSingletonChildId<Face>(level, entity, create_face);
		level.GetComponent<DrawInfo>(child)->image_path = face->image_path;
		level.GetComponent<Radius>(child)->radius = radius->radius;
		if (FillColor* fill_color = level.RawGetComponent<FillColor>(entity))
		{
			level.EnsureExistenceOfComponent<FillColor>(child)->color = fill_color->color;
		}

		sf::Vector2f& pos = position->position;
		Velocity* velocity = level.RawGetComponent<Velocity>(entity);
		if (!velocity)
		{
			return;
		}
		sf::Vector2f& vel = velocity->velocity;
		sf::Vector2f& child_pos = level.GetComponent<Position>(child)->position;

		if (level.GetMode() != PLAY_MODE)
		{
			child_pos = pos;
			return;
		}
		child_pos += vel * dt;

		sf::Vector2f offset = child_pos - pos;
		sf::Vector2f target_offset = sf::Vector2f(0, 0);
		if (Magnitude(vel) > MINIMUM_PLAYER_VELOCITY_REQUIRED_FOR_FACE_MOVEMENT)
		{
			target_offset = 20.f * Normalized(vel);
		}
		float snap = 0.05f * dt * radius->radius;
		sf::Vector2f next_offset = (1 - snap) * offset + snap * target_offset;
		child_pos = pos + next_offset;
	}
}