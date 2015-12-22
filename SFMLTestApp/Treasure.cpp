#include "Treasure.h"
#include "Style.h"
#include "WindowEventHandler.h"

//////////////////////////////////////////////////////////////////////////
Treasure::Treasure( std::shared_ptr<TargetStyle> _style ) : m_style(_style), m_symbol(nullptr)
{
	if(m_style)
	{
		m_symbol = m_style->getSymbol();

		// set start position:
		std::shared_ptr<sf::Vector2i> l_absCurrPos = m_style->getPosition();
		sf::Vector2i l_step = m_style->getStep();
		m_symbol->setPosition(l_absCurrPos->x * l_step.x, l_absCurrPos->y * l_step.y);

		m_style->setOwner( Identifier::getNext() );
	}
}

//////////////////////////////////////////////////////////////////////////
void Treasure::bindTexture()
{
	m_style->bindTargetTexture();
	m_symbol = m_style->getSymbol();
}

//////////////////////////////////////////////////////////////////////////
void Treasure::draw(std::shared_ptr<WindowEventHandler> _window)
{
	_window->m_window->draw(*m_symbol.get());
}

//////////////////////////////////////////////////////////////////////////
sf::Vector2f Treasure::getPosition()
{
	return m_symbol->getPosition();
}
