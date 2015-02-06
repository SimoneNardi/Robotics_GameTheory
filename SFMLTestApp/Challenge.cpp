#include "Challenge.h"
#include "WindowEventHandler.h"
#include "ConfigurationFile.h"
#include "Player.h"
#include "Grid.h"
#include "Timer.h"
#include "Treasure.h"
#include "Report.h"
#include "AbsModel.h"
#include "Order.h"
#include <sstream>

#include <SFML/System.hpp>

const int g_NumberOfPlayer = 2;

PlayerOrder g_playerOrder;

//////////////////////////////////////////////////////////////////////////
Challenge::Challenge() : m_players(), m_board(nullptr), m_timer(nullptr), m_report(nullptr), m_absModel(nullptr)
{
	/// read player from configuration file:
	int l_numberOfPlayer = g_configurationFile->getNumberPlayerStyle();
	for(int i = 0; i < l_numberOfPlayer; ++i)
	{
		std::shared_ptr<Player> l_player_ptr = std::make_shared<Player>( g_configurationFile->getPlayerStyle() );
		PlayerOnBoard l_player_struct;
		l_player_struct.player = l_player_ptr;
		l_player_struct.active = true;

		m_players.push_back( l_player_struct );
	}

	std::shared_ptr<TargetStyle> l_style = g_configurationFile->getTargetStyle();
	if(l_style)
		m_treasure = std::make_shared<Treasure>( l_style );

	/// create board from configuration file:
	m_board = std::make_shared<Grid>( g_configurationFile->getGridStyle() );
	if(m_board)
		m_board->init();
	/// Initialize timer:
	m_timer = std::make_shared<Timer>();

	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];

	time (&rawtime);
	timeinfo = localtime (&rawtime);
	std::stringstream ss;
	ss << "_" << (1900 + timeinfo->tm_year) << "_" << (timeinfo->tm_mon+1) << "_" << timeinfo->tm_mday << "_" << timeinfo->tm_hour << "_" << timeinfo->tm_min << "_" << timeinfo->tm_sec;

	m_report = std::make_shared<Report>("Result/result" + ss.str() +".dat");

	m_absModel = std::make_shared<AbsModel>("absModel.dat", m_board);

	this->init();
}

//////////////////////////////////////////////////////////////////////////
bool Challenge::isGameFinished()
{
	for(size_t i = 0; i < m_players.size(); ++i)
	{
		if(m_players[i].player->isWinner())
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
void Challenge::update(Identifier _windows_id, sf::Event _event)
{
	for(size_t i = 0; i < m_players.size(); ++i)
	{
		m_players[i].player->setAvailable(true);
		Direction l_dir = m_players[i].player->getControl().getDirection(_event);
		if( l_dir != NotSet && g_playerOrder.isAble(m_players[i].player) )
		{		
			m_players[i].player->move(l_dir);
		}
	}

	g_playerOrder.reset(m_players.size(), m_report);

	g_playerOrder.showUnavailable();

	goalAchieved();

}

//////////////////////////////////////////////////////////////////////////
bool Challenge::goalAchieved()
{
	bool res = false;
	for(size_t i = 0; i < m_players.size(); ++i)
	{
		if( m_players[i].player->goalAchieved( this->shared_from_this() ) )
			res = true;
	}
	return res;
}

//////////////////////////////////////////////////////////////////////////
bool Challenge::guardGoalAchieved(std::shared_ptr<Player> _guard)
{
	for(size_t i = 0; i < m_players.size(); ++i)
	{
		if( m_players[i].player->isThief() )
		{
			if( _guard->checkThief(m_players[i].player) )
			{
				_guard->setWinner(true);
				return true;
			}
			else
			{
				_guard->setWinner(false);
			}
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool Challenge::thiefGoalAchieved(std::shared_ptr<Player> _thief)
{
	if(m_treasure)
	{
		if(_thief->checkTreasure(m_treasure))
		{
			_thief->setWinner(true);
			return true;
		}
		else
		{
			_thief->setWinner(false);
			return false;
		}
	}
	else
	{
		_thief->setWinner(false);
		return false;
	}
}