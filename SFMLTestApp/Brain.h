#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Identifier.h"

class Grid;
class Player;
class Timer;
class Treasure;
class WindowEventHandler;
class Report;
class AbsModel;

//////////////////////////////////////////////////////////////////////////
class PlayerOnBoard
{
public:
	std::shared_ptr<Player> player;

	sf::Vector2i position;

	bool active;
};

//////////////////////////////////////////////////////////////////////////
class Brain : public std::enable_shared_from_this<Brain>
{
protected:
	std::shared_ptr<Grid> m_board;

	std::vector<PlayerOnBoard> m_players;

	std::shared_ptr<Timer> m_timer;

	std::shared_ptr<Treasure> m_treasure;

	std::shared_ptr<WindowEventHandler> m_first_window;
	std::shared_ptr<WindowEventHandler> m_second_window;

	std::shared_ptr<Report> m_report;

	std::shared_ptr<AbsModel> m_absModel;

public:
	///
	Brain();

	///
	void moveNext();

	///
	void waitInput();

	///
	void draw(std::shared_ptr<WindowEventHandler> _window);

	///
	bool init();

	///
	void run();

	///
	void update(Identifier _windows_id, sf::Event _event);

	///
	bool guardGoalAchieved(std::shared_ptr<Player> _guard);

	///
	bool thiefGoalAchieved(std::shared_ptr<Player> _thief);

	///
	bool goalAchieved();
	void print();
	void initReport();
	bool isGameFinished();
	void close();
};

bool Equals(sf::Vector2f & first, sf::Vector2f & second);
bool Equals(sf::Vector2i & first, sf::Vector2i & second);
