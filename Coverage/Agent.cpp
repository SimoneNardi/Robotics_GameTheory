#include "Agent.h"
#include "Area.h"
#include "DiscretizedArea.h"
#include "Guard.h"
#include "Thief.h"

#include <memory>

#include "BaseGeometry/Point2D.h"
#include "BaseGeometry/Shape2D.h"
#include "BaseGeometry/Arc2D.h"

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
bool AgentPosition::communicable(std::shared_ptr<Agent> _otherAgent) const
{
	return m_point.distance( _otherAgent->getPosition().getPoint2D() ) < 2 * m_camera.getFarRadius() + IDSMath::TOLERANCE;
}

//////////////////////////////////////////////////////////////////////////
bool AgentPosition::visible(std::shared_ptr<Square> _area) const
{
	Shape2D sh = m_camera.getVisibleArea(m_point);
	if(sh.isValid())
		return sh.contains( _area->getBoundingBox().center() );
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////
double AgentPosition::computeCosts() const
{
	return m_camera.computeCosts();
}

//////////////////////////////////////////////////////////////////////////
std::vector<AreaCoordinate> AgentPosition::getCoverage(std::shared_ptr<DiscretizedArea> _space ) const
{
	AreaCoordinate l_center = _space->getCoordinate(m_point);
	return m_camera.getCoverage(l_center, _space);
}

//////////////////////////////////////////////////////////////////////////
IDS::BaseGeometry::Shape2D AgentPosition::getVisibleArea() const
{
	return m_camera.getVisibleArea(m_point);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
std::vector<AreaCoordinate> CameraPosition::getCoverage(AreaCoordinate _center, std::shared_ptr<DiscretizedArea> _area) const
{
	Point2D l_pt = _area->getPosition(_center);
	Shape2D l_sensorArea = this->getVisibleArea(l_pt);
	if(!l_sensorArea.isValid())
		return std::vector<AreaCoordinate>();

	int l_rowDelta = floor(m_farRadius / _area->getYStep()) + 1;
	int l_colDelta = floor(m_farRadius / _area->getXStep()) + 1;

	std::vector<AreaCoordinate> result;
	AreaCoordinate l_elem;
	for(int i = - l_rowDelta; i <= l_rowDelta; ++i)
	{
		int row = _center.row + i;
		if( row < 0 || row > _area->getNumRow() )
			continue;
		for(int j = - l_colDelta; j <= l_colDelta; ++j)
		{
			int col = _center.col + j;
			if( col < 0 || col > _area->getNumRow() )
				continue;

			SquarePtr l_square = _area->getSquare(row, col);
			if(!l_square)
				continue;

			Point2D l_center = l_square->getBoundingBox().center();
			if( l_sensorArea.contains(l_center) )
			{
				l_elem.row = row;
				l_elem.col = col;
				result.push_back(l_elem);
			}
		}
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
BaseGeometry::Shape2D CameraPosition::getVisibleArea(BaseGeometry::Point2D const& point) const
try
{
	if(fabs(m_farRadius) < IDSMath::TOLERANCE)
		return Shape2D();

	Curve2D l_external = makeCircle( point, m_farRadius, false);
	return Shape2D( std::vector<Curve2D>(1,l_external) );
}
catch (...)
{
	return Shape2D();
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
void Agent::setNextPosition()
{
	m_nextPosition = m_position;
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
void Agent::setCurrentPayoff(double _benefit)
{
	m_currentPayoff = _benefit;
}

//////////////////////////////////////////////////////////////////////////
void Agent::moveToNextPosition()
{
	m_memory.insert(m_memory.begin(), MemoryAgentPosition(m_position, m_currentPayoff));

	if(m_memory.size() > m_memorySpace)
		m_memory.erase(m_memory.begin()+m_memorySpace);

	m_currentPayoff = -IDSMath::Infinity;
	m_position = m_nextPosition;
	
	return;
}

//////////////////////////////////////////////////////////////////////////
std::vector<AgentPosition> Agent::getFeasibleActions( std::shared_ptr<DiscretizedArea> _space ) const
{
	AreaCoordinate l_currCoord = _space->getCoordinate( m_position.getPoint2D() );

	std::vector<AreaCoordinate> l_squares = _space->getStandardApproachableValidSquares(l_currCoord);

	std::vector<AgentPosition> l_result;
	for(size_t i = 0; i < l_squares.size(); ++i )
	{
		l_result.push_back( AgentPosition(_space->getPosition(l_squares[i]), m_position.m_camera) );
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
	std::set<std::shared_ptr<Square> > result;
	std::vector<AreaCoordinate> l_coord = m_position.getCoverage(_space);
	for(size_t i = 0; i < l_coord.size(); ++i)
	{
		result.insert(_space->getSquare(l_coord[i]));
	}
	return result;
	//return _space->getVisibleSquares(m_position);
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

//////////////////////////////////////////////////////////////////////////
double Agent::computeCosts() const
{
	return m_position.computeCosts();
}

//////////////////////////////////////////////////////////////////////////
void Agent::setNextPosition(std::vector< MemoryAgentPosition > const& memory)
{
	if( memory.empty() )
	{
		m_nextPosition = m_position;
	}
	else
	{
		double l_bestPayoff = memory.at(0).m_payoff;
		m_nextPosition = memory.at(0).m_action;
		for(size_t i= 1; i < memory.size(); ++i)
		{
			if(memory[i].m_payoff > l_bestPayoff)
			{
				m_nextPosition = memory[i].m_action;
				l_bestPayoff = memory[i].m_payoff;
			}
		}
	}	
	return;
}

//////////////////////////////////////////////////////////////////////////
std::shared_ptr<Thief> Agent::toThief()
{
	return std::dynamic_pointer_cast<Thief>(this->shared_from_this());
}

//////////////////////////////////////////////////////////////////////////
std::shared_ptr<Guard> Agent::toGuard()
{
	return dynamic_pointer_cast<Guard>(this->shared_from_this());
}

//////////////////////////////////////////////////////////////////////////
IDS::BaseGeometry::Shape2D Agent::getVisibleArea() const
{
	return m_position.getVisibleArea();
}
