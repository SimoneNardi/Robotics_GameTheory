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
	ss << "_" << timeinfo->tm_hour << "_" << timeinfo->tm_min << "_" << timeinfo->tm_sec;

	m_report = std::make_shared<Report>("result" + ss.str() +".dat");

	m_absModel = std::make_shared<AbsModel>("absModel.dat", m_board);

	this->init();
}

//////////////////////////////////////////////////////////////////////////
void Challenge::print()
{
	m_report->print();
}

//////////////////////////////////////////////////////////////////////////
bool Challenge::init()
{
	m_first_window = std::make_shared<WindowEventHandler>( "NoName_1", Identifier::getNext() );
#ifdef _SECOND_WINDOW
	m_second_window = std::make_shared<WindowEventHandler>( "NoName_2", Identifier::getNext() );
#endif

	return true;
}

//////////////////////////////////////////////////////////////////////////
void Challenge::run()
{
	// create a thread with func() as entry point
	sf::Thread l_thread1( &WindowEventHandler::EventLoop, m_first_window.get() );

#ifdef _SECOND_WINDOW
	// create a thread with func() as entry point
	sf::Thread l_thread2( &WindowEventHandler::EventLoop, m_second_window.get() );
#endif
	// run it
	l_thread1.launch();
#ifdef _SECOND_WINDOW
	l_thread2.launch();
#endif

	while( !m_first_window->m_created 
#ifdef _SECOND_WINDOW
		|| !m_second_window->m_created 
#endif
		)
		sf::sleep( sf::milliseconds(100) );

	m_first_window->setChallenge(this->shared_from_this());
#ifdef _SECOND_WINDOW
	m_second_window->setChallenge(this->shared_from_this());
#endif
	for(size_t i = 0; i < m_players.size(); ++i)
		m_players[i].player->bindTexture();

	if (m_treasure)
		m_treasure->bindTexture();

	initReport();

	m_timer->start();

	while( m_first_window->m_running
		//&& !isGameFinished()
#ifdef _SECOND_WINDOW
		&& m_second_window->m_running 
#endif
		)
		sf::sleep( sf::milliseconds(100) );
}

//////////////////////////////////////////////////////////////////////////
void Challenge::close()
{
	if(m_first_window)
		m_first_window->close();

	if(m_second_window)
		m_second_window->close();
}

//////////////////////////////////////////////////////////////////////////
void Challenge::draw(std::shared_ptr<WindowEventHandler> _window)
	/// draw order: board, timer, treasure, players...
{
	m_board->draw(_window);

	m_timer->draw(_window);

	//m_absModel->draw(_window);
	if(m_treasure)
		m_treasure->draw(_window);

	for(size_t i = 0; i < m_players.size(); ++i)
	{
		m_players[i].player->draw(_window);
	}
}

//////////////////////////////////////////////////////////////////////////
void Challenge::initReport()
{
	for(size_t i = 0; i < m_players.size(); ++i)
	{
		m_report->update(m_players[i].player);
	}
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
			//m_players[i].player->updatePosition();
			//m_report->update(m_players[i].player);
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

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
bool Equals(sf::Vector2f & first, sf::Vector2f & second)
{
	return fabs(first.x - second.x) < 1.e-2 && fabs(first.y - second.y) < 1.e-2;
}

//////////////////////////////////////////////////////////////////////////
bool Equals(sf::Vector2i & first, sf::Vector2i & second)
{
	return abs(first.x - second.x) < 1.e-2 && abs(first.y - second.y) < 1.e-2;
}