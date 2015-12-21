#pragma once

#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "InputControl.h"
#include "Identifier.h"

class PlayerStyle;
class WindowEventHandler;
class Treasure;
<<<<<<< HEAD
class Brain;
=======
class Challenge;
>>>>>>> origin/Pareto_Efficient

class Player : public std::enable_shared_from_this<Player>
{
protected:
	/// Symbol
	std::shared_ptr<sf::CircleShape> m_symbol;

	///
	sf::Vector2i m_currentPos;
	///
	sf::Vector2i m_oldPos;

	///
	std::shared_ptr<PlayerStyle> m_style;

	///
	int m_xStep;

	///
	int m_yStep;
	
	///
	bool m_winner;

	///
	Identifier m_id;

	///
	bool m_available;
public:

	///
	Player(std::shared_ptr<PlayerStyle> _style);

	///
	void bindTexture();

	///
	void draw(std::shared_ptr<WindowEventHandler> _window);

	///
	bool updatePosition();

	///
	void move( Direction _direction );

	///
	InputControlStyle getControl();

	///
<<<<<<< HEAD
	bool goalAchieved(std::shared_ptr<Brain> _game);
=======
	bool goalAchieved(std::shared_ptr<Challenge> _game);
>>>>>>> origin/Pareto_Efficient

	///
	void setWinner(bool _winner);

	///
	bool isThief();

	///
	sf::Vector2i serializePosition();

public:
	///
	bool checkThief(std::shared_ptr<Player> _target);

	///
	bool checkTreasure(std::shared_ptr<Treasure> _target);

	///
	sf::Vector2f getPosition();

	///
	Identifier getID();

	///
	bool isWinner();

	///
	void setAvailable(bool _available);
};