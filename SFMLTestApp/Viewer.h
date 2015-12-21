#pragma once

#include "Brain.h"
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
class Viewer : public Brain
{
public:
	///
	Viewer();
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