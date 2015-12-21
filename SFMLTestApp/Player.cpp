#include "Player.h"
#include "Style.h"
#include "WindowEventHandler.h"
#include "ConfigurationFile.h"
#include "Treasure.h"
#include "Challenge.h"
 
//////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
Player::Player(std::shared_ptr<PlayerStyle> _style)
=======
Player::Player(std::shared_ptr<PlayerStyle> _style) 
>>>>>>> origin/Pareto_Efficient
	: m_style(_style), m_symbol(nullptr), m_currentPos(), m_oldPos(), m_winner(false), m_id( Identifier::getNext() ), m_available(true)
{
	if(!m_style)
		throw std::exception("Not enough player styles in the configuration file!");
	
	m_style->setOwner( m_id );

	m_symbol = m_style->getSymbol();

	// set start position:
	std::shared_ptr<sf::Vector2i> l_absCurrPos = m_style->getPosition();

#if 0
	l_absCurrPos->x = rand() % g_configurationFile->getGridStyle()->getCol();
	l_absCurrPos->y = rand() % g_configurationFile->getGridStyle()->getRow();
#endif

	sf::Vector2i l_step = m_style->getStep();
	m_currentPos = sf::Vector2i( l_absCurrPos->x * l_step.x, l_absCurrPos->y * l_step.y);
	m_oldPos = m_currentPos; 
}

//////////////////////////////////////////////////////////////////////////
void Player::bindTexture()
{
<<<<<<< HEAD
	m_style->bindPlayerTexture( m_style->getPlayerType() );
=======
	m_style->bindTexture( m_style->getPlayerType() );
>>>>>>> origin/Pareto_Efficient
	m_symbol = m_style->getSymbol();

	this->updatePosition();
}

//////////////////////////////////////////////////////////////////////////
bool Player::updatePosition()
{
	m_symbol->setPosition(m_currentPos.x, m_currentPos.y);
	return !Equals(m_currentPos, m_oldPos);
}

//////////////////////////////////////////////////////////////////////////
void Player::draw(std::shared_ptr<WindowEventHandler> _window)
{
	sf::CircleShape l_circle = *m_symbol.get();
	if(!m_available)
		l_circle.setFillColor(sf::Color(255, 255, 255, 128)); // half transparent

	_window->m_window->draw(l_circle);
}

//////////////////////////////////////////////////////////////////////////
void Player::move( Direction _direction )
{
	sf::Vector2i l_step = m_style->getStep();
	int newx(m_currentPos.x), newy(m_currentPos.y);
	switch(_direction)
	{
	case Left:
		newx -= l_step.x;
		break;
	case Right:
		newx += l_step.x;
		break;
	case Up:
		newy -= l_step.y;
		break;
	case Down:
		newy += l_step.y;
		break;
	default:
		break;
	}

	if(newx < 0 || newx >= _WINDOW_DIM)
		newx = m_currentPos.x;
	if(newy < 0 || newy >= _WINDOW_DIM)
		newy = m_currentPos.y;

	m_oldPos = m_currentPos;
	m_currentPos = sf::Vector2i(newx, newy);
}

//////////////////////////////////////////////////////////////////////////
void Player::setAvailable(bool _available)
{
	m_available = _available;
}

//////////////////////////////////////////////////////////////////////////
InputControlStyle Player::getControl()
{
	 return m_style->getControl();
}

//////////////////////////////////////////////////////////////////////////
bool Player::checkTreasure(std::shared_ptr<Treasure> _target)
{
	return Equals( this->getPosition(), _target->getPosition() ); 
}

//////////////////////////////////////////////////////////////////////////
bool Player::checkThief(std::shared_ptr<Player> _target)
{
	return Equals( this->getPosition(), _target->getPosition() ); 
}

//////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
bool Player::goalAchieved(std::shared_ptr<Brain> _game)
=======
bool Player::goalAchieved(std::shared_ptr<Challenge> _game)
>>>>>>> origin/Pareto_Efficient
{
	switch(m_style->getPlayerType())
	{
	case Guard:
			return _game->guardGoalAchieved( this->shared_from_this() );
		break;
	case Thief:
		return _game->thiefGoalAchieved( this->shared_from_this() );
		break;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool Player::isWinner()
{
	return m_winner;
}

//////////////////////////////////////////////////////////////////////////
void Player::setWinner(bool _winner)
{
	m_winner = _winner;
	if(m_winner)
		m_style->setWinner();
	else
		m_style->resetWinner();

	m_symbol = m_style->getSymbol();
}

//////////////////////////////////////////////////////////////////////////
bool Player::isThief()
{
	return m_style->getPlayerType() == Thief;
}

//////////////////////////////////////////////////////////////////////////
sf::Vector2f Player::getPosition()
{
	return m_symbol->getPosition();
}

//////////////////////////////////////////////////////////////////////////
Identifier Player::getID()
{
	return m_id;
}

//////////////////////////////////////////////////////////////////////////
sf::Vector2i Player::serializePosition()
{
	sf::Vector2i l_step = m_style->getStep();
	return sf::Vector2i( m_currentPos.x / l_step.x + 0.5, m_currentPos.y / l_step.y + 0.5 );
}