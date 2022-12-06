#pragma once
#include "PCH.hpp"

class KeyConfig
{
public:
	sf::Keyboard::Key PLAYER_MOVE_LEFT = sf::Keyboard::A;
	sf::Keyboard::Key PLAYER_MOVE_RIGHT = sf::Keyboard::D;
	sf::Keyboard::Key PLAYER_MOVE_UP = sf::Keyboard::W;
	sf::Keyboard::Key PLAYER_MOVE_DOWN = sf::Keyboard::S;
	sf::Keyboard::Key PLAYER_SWITCH_CHARGE = sf::Keyboard::Space;
	sf::Keyboard::Key PLAYER_GO_NEUTRAL = sf::Keyboard::Enter;
	sf::Keyboard::Key MENU = sf::Keyboard::Escape;
	sf::Keyboard::Key EDIT_MODE = sf::Keyboard::Tab;
	sf::Keyboard::Key INCREASE_LEVEL_SIZE = sf::Keyboard::Up;
	sf::Keyboard::Key DECREASE_LEVEL_SIZE = sf::Keyboard::Down;
	sf::Keyboard::Key EDIT_MODE_SWITCH_CHARGE = sf::Keyboard::Space;
	sf::Keyboard::Key EDIT_MODE_SWITCH_MAGNETIC_FIELD_DIRECTION = sf::Keyboard::Space;
	sf::Keyboard::Key SELECT_MULTIPLE_ENTITIES = sf::Keyboard::LControl;
	sf::Keyboard::Key COPY_ENTITY = sf::Keyboard::LAlt;
	sf::Keyboard::Key DELETE_ENTITY = sf::Keyboard::Delete;
	sf::Keyboard::Key SNAP_TO_GRID = sf::Keyboard::LShift;
	sf::Keyboard::Key ALT_SENSITIVITY = sf::Keyboard::RShift;
	sf::Keyboard::Key ROTATE_ENTITY = sf::Keyboard::R;
	sf::Keyboard::Key INCREMENT_HEIGHT = sf::Keyboard::W;
	sf::Keyboard::Key DECREMENT_HEIGHT = sf::Keyboard::S;
	sf::Keyboard::Key DECREMENT_WIDTH = sf::Keyboard::A;
	sf::Keyboard::Key INCREMENT_WIDTH = sf::Keyboard::D;
	sf::Keyboard::Key INCREMENT_VELOCITY = sf::Keyboard::W;
	sf::Keyboard::Key DECREMENT_VELOCITY = sf::Keyboard::S;
	sf::Keyboard::Key DECREMENT_VELOCITY_ANGLE = sf::Keyboard::A;
	sf::Keyboard::Key INCREMENT_VELOCITY_ANGLE = sf::Keyboard::D;
};