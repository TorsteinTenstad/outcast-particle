#include "_pure_DO_systems.hpp"
#include "entity_creation.hpp"
#include "utils.hpp"

#define MINIMUM_PLAYER_VELOCITY_REQUIRED_FOR_FACE_MOVEMENT 10

void FaceSystem::Update(Level& level, float dt)
{
	for (auto& [entity_id, face, children, draw_info, draw_priority, radius, position] : level.GetEntitiesWith<Face, Children, DrawInfo, DrawPriority, Radius, Position>())
	{

		std::function<int(void)> create_face = [&level, face = face, draw_priority = draw_priority, radius = radius, position = position]() {
			int face_id = level.CreateEntityId();
			level.AddComponent<DrawInfo>(face_id);
			level.AddComponent<DrawPriority>(face_id)->draw_priority = draw_priority->draw_priority;
			level.AddComponent<Radius>(face_id)->radius = radius->radius;
			level.AddComponent<Position>(face_id)->position = position->position;
			level.AddComponent<FillColor>(face_id);
			AnimatedOpacity* animated_opacity = level.AddComponent<AnimatedOpacity>(face_id);
			animated_opacity->animation_func = [](float t) {{ return sf::Uint8(255 * Smoothstep(2*t-1)); }; };
			animated_opacity->start_time = globals.time_of_last_level_enter;
			return face_id;
		};

		int child_id = EnsureExistanceOfChildEntity<Face>(children, create_face);
		level.GetComponent<DrawInfo>(child_id)->image_path = face->image_path;
		if (!level.HasComponents<Velocity>(entity_id))
		{
			continue;
		}
		sf::Vector2f& pos = position->position;
		sf::Vector2f& vel = level.GetComponent<Velocity>(entity_id)->velocity;
		sf::Vector2f& child_pos = level.GetComponent<Position>(child_id)->position;

		if (level.GetMode() == PLAY_MODE)
		{
			child_pos += vel * dt;
		}

		sf::Vector2f offset = child_pos - pos;
		sf::Vector2f target_offset = sf::Vector2f(0, 0);
		if (Magnitude(vel) > MINIMUM_PLAYER_VELOCITY_REQUIRED_FOR_FACE_MOVEMENT)
		{
			target_offset = 20.f * Normalized(vel);
		}
		float snap = 0.01f * dt * 600;
		sf::Vector2f next_offset = (1 - snap) * offset + snap * target_offset;
		child_pos = pos + next_offset;
	}
}