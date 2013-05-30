#include "Agent.h"
#include "Area.h"
#include "DiscretizedArea.h"

#include "BaseGeometry/Point2D.h"

using namespace std;
using namespace Robotics;
using namespace Robotics::GameTheory;
using namespace IDS;
using namespace IDS::BaseGeometry;

//////////////////////////////////////////////////////////////////////////
void AgentPosition::updateCounter(std::shared_ptr<DiscretizedArea> area)
{
	AreaCoordinate l_coord = area->getCoordinate(m_point);

	area->getSquare( l_coord.row, l_coord.col )->increaseCounter();

	// Calcolo la copertura in base alla camera:
	// per ora conta solo il raggio massimo e non cala con la distanza!
	std::vector<AreaCoordinate> l_cover = m_camera.getCoverage(l_coord, area);

	for(size_t i = 0; i < l_cover.size(); ++i)
	{
		std::shared_ptr<Square> l_square = area->getSquare( l_cover[i].row, l_cover[i].col );
		if(l_square)
			l_square->increaseCounter();
	}
}

//////////////////////////////////////////////////////////////////////////
bool AgentPosition::communicable(std::shared_ptr<Agent> _other) const
{
	return m_point.distance( _other->getPosition()->getPoint2D() ) < 2 * m_camera.getFarRadius() + IDSMath::TOLERANCE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
std::vector<AreaCoordinate> CameraPosition::getCoverage(AreaCoordinate _center, std::shared_ptr<DiscretizedArea> _area)
{
	return std::vector<AreaCoordinate>(); //TODO
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
void Agent::setPosition(AgentPosition const& pos)
{
	m_position = pos;
}

//////////////////////////////////////////////////////////////////////////
void Agent::setNextPosition(AgentPosition const& pos)
{
	m_nextPosition = pos;
}

//////////////////////////////////////////////////////////////////////////
bool Agent::isActive()
{
	bool wakeUp = getStatus() == Agent::WAKEUP;
	if(wakeUp)
		setStatus(Agent::ACTIVE);

	return getStatus() == Agent::ACTIVE;
}

//////////////////////////////////////////////////////////////////////////
bool Agent::isOutOfInterest( std::shared_ptr<DiscretizedArea> space) const
{
	return space->isOut(m_position);
}

//////////////////////////////////////////////////////////////////////////
void Agent::sleep()
{
	return setStatus(Agent::SLEEP);
}

//////////////////////////////////////////////////////////////////////////
void Agent::wakeUp()
{
	return setStatus(Agent::WAKEUP);
}

//////////////////////////////////////////////////////////////////////////
Agent::Status Agent::getStatus() const
{
	return m_status;
}

//////////////////////////////////////////////////////////////////////////
void Agent::setStatus(Status stat)
{
	m_status = stat;
}

//////////////////////////////////////////////////////////////////////////
void Agent::moveToNextPosition()
{
	m_memory.insert(m_memory.begin(), MemoryAgentPosition(m_position, m_currentPayoff));

	m_currentPayoff = -IDSMath::Infinity;
	m_position = m_nextPosition;
	
	return;
}

//////////////////////////////////////////////////////////////////////////
std::vector<AgentPosition> Agent::getFeasibleActions( std::shared_ptr<DiscretizedArea> _space ) const
{
	std::vector<AgentPosition> l_result;

	AreaCoordinate l_currCoord = _space->getCoordinate( m_position.getPoint2D() );

	std::vector<AreaCoordinate> l_squares = _space->getStandardApproachableValidSquares();

	for(size_t i = 0; i < l_squares.size(); ++i )
	{
		l_result.push_back( _space->getPosition(l_squares[i]) );
	}

	return l_result;
}

//////////////////////////////////////////////////////////////////////////
std::set<std::shared_ptr<Agent> > Agent::getCommunicableAgents(std::set<std::shared_ptr<Agent> > const& _guards) const
{
	std::set<std::shared_ptr<Agent> > l_result;
	for(std::set<std::shared_ptr<Agent> >::const_iterator it = _guards.begin(); it != _guards.end(); ++it)
	{
		AgentPtr l_agent = *it;
		if( this->equals(l_agent) )
			continue;

		if( this->m_position.communicable( l_agent ) )
		{
			l_result.insert(l_agent);
		}
	}
	return l_result;
}

//////////////////////////////////////////////////////////////////////////
std::set<std::shared_ptr<Square> > Agent::getVisibleSquares( std::shared_ptr<DiscretizedArea> _space ) const
{
	return _space->getVisibleSquares(m_position);
}

//////////////////////////////////////////////////////////////////////////
bool Agent::equals(std::shared_ptr<Agent> _other) const
{
	return _other->m_id == m_id;
}

//////////////////////////////////////////////////////////////////////////
void Agent::receiveMessage( std::set<std::shared_ptr<Square> > const& _visible)
{
	// Ogni agente deve conoscere il numero di agenti che potenzialmente esaminano un riquadro:
	// in questo modo possono determinare la quantità n_q(s) per ogni q feasible
	// e quindi calcolare la propria utilità
}
