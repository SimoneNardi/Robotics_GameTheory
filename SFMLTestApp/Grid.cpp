#include "Grid.h"
#include "WindowEventHandler.h"
#include "ConfigurationFile.h"

bool Grid::init()
{
	if( !(m_col > 0 && m_row > 0) )
		return false;

	/// dato il numero di righe e di colonne calcolo le righe del grigliato:
	int l_stepCol = _WINDOW_DIM / m_col;
	int l_stepRow = _WINDOW_DIM / m_row;

	/// create the list of line for col
	for(int i = 0; i < m_row-1; ++i)
	{
		sf::VertexArray l_row(sf::LinesStrip, m_col+1);
		l_row.setPrimitiveType(sf::LinesStrip);
		
		for(int j = 0; j < m_col+1; ++j)
			l_row[j].position = sf::Vector2f(l_stepRow * (i+1), l_stepCol * j);
		
		m_lattice.push_back(l_row);
	}

	/// create the list of line for row
	for(int i = 0; i < m_col-1; ++i)
	{
		sf::VertexArray l_col(sf::LinesStrip, m_row+1);
		l_col.setPrimitiveType(sf::LinesStrip);

		for(int j = 0; j < m_row+1; ++j)
			l_col[j].position = sf::Vector2f(l_stepRow * j, l_stepCol * (i+1));

		m_lattice.push_back(l_col);
	}

	return true;
}

void Grid::draw(std::shared_ptr<WindowEventHandler> _window)
{
	for(size_t i = 0; i < m_lattice.size(); ++i)
		_window->m_window->draw(m_lattice[i]);
}