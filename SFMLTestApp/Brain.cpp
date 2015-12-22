#include "Brain.h"
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
Brain::Brain() : m_players(), m_board(nullptr), m_timer(nullptr), m_report(nullptr), m_absModel(nullptr)
{}

//////////////////////////////////////////////////////////////////////////
bool Brain::init()
{
	m_first_window = std::make_shared<WindowEventHandler>( "NoName_1", Identifier::getNext() );
#ifdef _SECOND_WINDOW
	m_second_window = std::make_shared<WindowEventHandler>( "NoName_2", Identifier::getNext() );
#endif

	return true;
}

//////////////////////////////////////////////////////////////////////////
void Brain::print()
{
	m_report->print();
}

//////////////////////////////////////////////////////////////////////////
void Brain::run()
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

	m_first_window->setBrain(this->shared_from_this());
#ifdef _SECOND_WINDOW
	m_second_window->setBrain(this->shared_from_this());
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
void Brain::close()
{
	if(m_first_window)
		m_first_window->close();

	if(m_second_window)
		m_second_window->close();
}

//////////////////////////////////////////////////////////////////////////
void Brain::draw(std::shared_ptr<WindowEventHandler> _window)
	/// draw order: board, timer, treasure, players...
{
	m_board->draw(_window);

	m_timer->draw(_window);

	if(m_treasure)
		m_treasure->draw(_window);

	for(size_t i = 0; i < m_players.size(); ++i)
	{
		m_players[i].player->draw(_window);
	}
}

//////////////////////////////////////////////////////////////////////////
void Brain::initReport()
{
	for(size_t i = 0; i < m_players.size(); ++i)
	{
		m_report->update(m_players[i].player);
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