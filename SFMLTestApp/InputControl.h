#pragma once

#include <SFML/Graphics.hpp>

//////////////////////////////////////////////////////////////////////////
enum Direction
{
	Up,
	Down,
	Left,
	Right,
	NotSet
};

//////////////////////////////////////////////////////////////////////////
class InputControlStyle
{
public:
	enum InputControl
	{
		Arrow_Keyboard,
		ASDW_Keyboard,
		Joystick1,
		Joystick2,
		Undefined
	} m_type;

	InputControlStyle(int _num_of_player);

	Direction getDirection(sf::Event _event);
};