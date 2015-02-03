#pragma once

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

struct AbsTrajectory 
{
	sf::VertexArray lines;
};

class WindowEventHandler;
class Grid;

class AbsModel
{
protected:
	///
	std::multimap<int, AbsTrajectory> m_traj;

public:
	///
	AbsModel(std::string const& _filename, std::shared_ptr<Grid> _grid);
	///
	void draw(std::shared_ptr<WindowEventHandler> _window);
};