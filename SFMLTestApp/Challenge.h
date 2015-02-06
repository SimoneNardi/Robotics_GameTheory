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
class Challenge : public Brain
{

public:
	///
	Challenge();
};