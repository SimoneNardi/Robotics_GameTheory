#pragma once

#include "Brain.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Identifier.h"

<<<<<<< HEAD
=======
class Grid;
class Player;
class Timer;
class Treasure;
class WindowEventHandler;
class Report;
class AbsModel;


>>>>>>> origin/Pareto_Efficient
//////////////////////////////////////////////////////////////////////////
class Challenge : public Brain
{

public:
	///
	Challenge();
<<<<<<< HEAD

	///
	virtual void update(Identifier _windows_id, sf::Event _event);

	///
	virtual bool guardGoalAchieved(std::shared_ptr<Player> _guard);

	///
	virtual bool thiefGoalAchieved(std::shared_ptr<Player> _thief);

	///
	virtual bool goalAchieved();

	///
	virtual bool isGameFinished();
=======
>>>>>>> origin/Pareto_Efficient
};