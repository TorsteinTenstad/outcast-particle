class KeyboardForceSystem
{
public:
	void Update(std::map<int, Player>& player_map, std::map<int, ReceivedForces>& received_forces_map)
	{
		for (auto& [entity_id, player] : player_map)
		{
			int x_direction = 0;
			if (player.moving_left)
			{
				x_direction -= 1;
			}
			if (player.moving_right)
			{
				x_direction += 1;
			}
			int y_direction = 0;
			if (player.moving_up)
			{
				y_direction -= 1;
			}
			if (player.moving_down)
			{
				y_direction += 1;
			}
			received_forces_map[entity_id].keyboard_force.x = x_direction * player.move_force;
			received_forces_map[entity_id].keyboard_force.y = y_direction * player.move_force;
		}
	}
};