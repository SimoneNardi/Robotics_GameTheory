#pragma once

#include "style.h"
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class WindowEventHandler;

class Grid
{
private:
	int m_col;
	int m_row;

	std::vector<sf::VertexArray> m_lattice;

public:
	/// constructor
	Grid() : m_col(0), m_row(0), m_lattice() {}

	/// constructor
	Grid(int _row, int _col) : m_row(_row), m_col(_col), m_lattice() {}

	/// Constructor
	Grid(std::shared_ptr<GridStyle> _style) : m_row( _style->getRow() ), m_col( _style->getCol() ), m_lattice() {}

	///
	void draw(std::shared_ptr<WindowEventHandler> _window);

	///
	bool init();

	int getCol() {return m_col;}
	int getRow() {return m_row;}
};