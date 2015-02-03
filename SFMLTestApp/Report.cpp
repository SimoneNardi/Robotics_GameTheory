#include "Report.h"
#include "ConfigurationFile.h"
#include "Player.h"
#include "AbsModel.h"

#include <iostream>

//////////////////////////////////////////////////////////////////////////
Report::Report(std::string const &_filename) : m_filename( g_configFolder + _filename), m_trajectory()
{}

//////////////////////////////////////////////////////////////////////////
void Report::update(std::shared_ptr<Player> _player)
{
	Identifier l_id = _player->getID();
	std::map<Identifier, Trajectory>::iterator it = m_trajectory.find( l_id );
	if( it == m_trajectory.end() )
		// traiettoria non ancora registrata:
	{
		Trajectory l_trj;
		l_trj.positions.push_back(_player->serializePosition());
		m_trajectory[l_id] = l_trj;
	}
	else
		m_trajectory[l_id].positions.push_back(_player->serializePosition());
}

//////////////////////////////////////////////////////////////////////////
void Report::print()
{
	std::ofstream l_output;
	l_output.open(m_filename);

	for(std::map<Identifier, Trajectory>::iterator it = m_trajectory.begin(); it != m_trajectory.end(); ++it)
	{
		l_output << it->first;
		Trajectory l_trj = it->second;
		for(size_t i = 0; i < l_trj.positions.size(); ++i)
		{
			l_output << "\t" << l_trj.positions[i].x << "\t" << l_trj.positions[i].y;
		}
		l_output << std::endl;
		
		l_output.flush();
	}
	l_output.close();

	std::cout << "Report has been written." << std::endl;
}