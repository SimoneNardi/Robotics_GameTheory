#include "AbsModel.h"
#include "WindowEventHandler.h"
#include "ConfigurationFile.h"
#include <iostream>
#include <fstream>
#include "Utility.h"
#include "Grid.h"
#include <memory>
#include <SFML/Graphics.hpp>

//////////////////////////////////////////////////////////////////////////
AbsModel::AbsModel(std::string const& _filename, std::shared_ptr<Grid> _grid)
{
	std::ifstream file;
	file.open(g_configFolder + _filename);

	int l_gridStepCol(1), l_gridStepRow(1);

	std::string l_sep("\t");
	if(file.is_open())
	{
		std::string l_line;
		while (std::getline(file, l_line))
		{
			std::vector<std::string> l_token;
			tokenize(l_line, l_sep, l_token);
			if(l_token.empty())
				continue;

			l_gridStepCol = _WINDOW_DIM / _grid->getCol();
			l_gridStepRow = _WINDOW_DIM / _grid->getRow();

			int prev( atoi( l_token[0].c_str() ) );
			AbsTrajectory l_trj;
			int l_num_vertex = (l_token.size()-1) / 2;
			l_trj.lines =sf::VertexArray( sf::LinesStrip, l_num_vertex );
			sf::Color l_color((rand()%155), (rand()% 155), (rand()%155));
			for(int i = 0; i < l_num_vertex; ++i)
			{
				l_trj.lines[i] = sf::Vector2f(atof( l_token[2*i + 1].c_str()) * l_gridStepRow + l_gridStepRow/2., atof(l_token[2*i + 2].c_str() ) * l_gridStepCol + l_gridStepCol/2.);
				l_trj.lines[i].color = l_color;
			}
			m_traj.insert( std::make_pair(prev, l_trj) );
		}
	}
	file.close();
}

//////////////////////////////////////////////////////////////////////////
void AbsModel::draw(std::shared_ptr<WindowEventHandler> _window)
{
#if 1
	for(std::multimap<int, AbsTrajectory>::iterator it = m_traj.begin(); it != m_traj.end(); ++it)
	{
		sf::VertexArray l_array = it->second.lines;
		_window->m_window->draw(l_array);
	}
#endif
}