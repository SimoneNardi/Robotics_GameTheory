#include "InputControl.h"

#include <iostream>

int g_available_joystick  = 0;
int g_available_keyboard = 0;

bool initGlobalInputControl()
{
	g_available_joystick = 0;
	g_available_keyboard = 0;

	sf::Joystick::update();

	if (sf::Joystick::isConnected(0))
	{
		// joystick number 0 is connected
		g_available_joystick ++;
	}
	else
	{
		g_available_keyboard++;
	}

	if (sf::Joystick::isConnected(1))
	{
		// joystick number 1 is connected
		g_available_joystick ++;
	}
	else
	{
		g_available_keyboard++;
	}
	return true;
}

bool g_inputControlInitialized;

//////////////////////////////////////////////////////////////////////////
InputControlStyle::InputControlStyle(int _num_of_player)
	// First of all assign the joystick to player and then the keyboard
{
	if(!g_inputControlInitialized)
		throw std::exception("Input Controller not initialized!");

	static int l_numOfJoyUsed = 0;

	if( g_available_joystick - l_numOfJoyUsed > 0 )
	{
		l_numOfJoyUsed++;
		if(l_numOfJoyUsed == 1)
			m_type = Joystick1;
		else
			m_type = Joystick2;
	}
	else if(l_numOfJoyUsed > 0)
	{
		m_type = Arrow_Keyboard;
	}
	else
	{
		if(_num_of_player == 1)
			m_type = Arrow_Keyboard;
		else
			m_type = ASDW_Keyboard;
	}
}


//////////////////////////////////////////////////////////////////////////
Direction InputControlStyle::getDirection(sf::Event _event)
{
	if (_event.type == sf::Event::KeyReleased)
	{
		switch(_event.key.code)
		{
		case sf::Keyboard::Escape:
			std::cout << "The Help key has been pressed:" << std::endl;

			std::cout << "Thief Control:" << (g_available_joystick > 1 ? "First Joystick" : (g_available_joystick > 0 ? "Joystick" : "Keyboard Arrows") ) << "."<< std::endl;
			std::cout << "Guard Control:" << (g_available_joystick > 1 ? "Second Joystick" : (g_available_joystick > 0 ? "Joystick" : "Keyboard A (left) S (down) D (right) W (up)") ) << "." << std::endl;

			break;
			//////////////////////////////////////////////////////////////////////////
		case	sf::Keyboard::Left:         ///< Left arrow
			if(m_type == Arrow_Keyboard)
				return Left;
			break;
		case	sf::Keyboard::Right:        ///< Right arrow
			if(m_type == Arrow_Keyboard)
				return Right;
			break;
		case	sf::Keyboard::Up:           ///< Up arrow
			if(m_type == Arrow_Keyboard)
				return Up;
			break;
		case	sf::Keyboard::Down:         ///< Down arrow
			if(m_type == Arrow_Keyboard)
				return Down;
			break;
			//////////////////////////////////////////////////////////////////////////
		case	sf::Keyboard::A:         ///< Left arrow
			if(m_type == ASDW_Keyboard)
				return Left;
			break;
		case	sf::Keyboard::D:        ///< Right arrow
			if(m_type == ASDW_Keyboard)
				return Right;
			break;
		case	sf::Keyboard::W:           ///< Up arrow
			if(m_type == ASDW_Keyboard)
				return Up;
			break;
		case	sf::Keyboard::S:         ///< Down arrow
			if(m_type == ASDW_Keyboard)
				return Down;
			break;
		}

	}
	else if (_event.type == sf::Event::JoystickMoved)
	{
		if (_event.joystickMove.axis == sf::Joystick::X)
		{
			if(	(g_available_joystick == 1 && m_type == Joystick1 ) ||
				(g_available_joystick > 1 &&
				(_event.joystickMove.joystickId == 0 && m_type == Joystick1 ||
				_event.joystickMove.joystickId == 1 && m_type == Joystick2) ))
			{

				if(_event.joystickMove.position < -99)
					return Left;
				if(_event.joystickMove.position > 99)
					return Right;
#if 0
				std::cout << "X axis moved!" << std::endl;
				std::cout << "joystick id: " << _event.joystickMove.joystickId << std::endl;
				std::cout << "new position: " << _event.joystickMove.position << std::endl;
#endif
			}

		}
		else if (_event.joystickMove.axis == sf::Joystick::Y)
		{
			if((g_available_joystick == 1 && m_type == Joystick1 ) ||
				(g_available_joystick > 1 &&
				(_event.joystickMove.joystickId == 0 && m_type == Joystick1 ||
				_event.joystickMove.joystickId == 1 && m_type == Joystick2) ))
			{
				if(_event.joystickMove.position > 99)
					return Down;
				if(_event.joystickMove.position < -99)
					return Up;
#if 0
				std::cout << "Y axis moved!" << std::endl;
				std::cout << "joystick id: " << _event.joystickMove.joystickId << std::endl;
				std::cout << "new position: " << _event.joystickMove.position << std::endl;
#endif
			}
		}
	}
	else if(_event.type == sf::Event::JoystickButtonReleased)
	{
		if((g_available_joystick == 1 && m_type == Joystick1 ) ||
			(g_available_joystick > 1 &&
			(_event.joystickMove.joystickId == 0 && m_type == Joystick1 ||
			_event.joystickMove.joystickId == 1 && m_type == Joystick2) ))
		{
			if(_event.joystickButton.button == 0)
			{
				return Left;
			}
			else if(_event.joystickButton.button == 1)
			{
				return Up;
			}
			else if(_event.joystickButton.button == 2)
			{
				return Down;
			}
			else if(_event.joystickButton.button == 3)
			{
				return Right;
			}
		}
	}
	return NotSet;
}
