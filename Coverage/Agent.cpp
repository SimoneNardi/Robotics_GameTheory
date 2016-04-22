#include "Agent.h"
#include "Area.h"
#include "DiscretizedArea.h"
#include "Guard.h"
#include "Thief.h"
#include "Sink.h"
#include "Probability.h"

#include <memory>

#include "BaseGeometry/Point2D.h"
#include "BaseGeometry/Shape2D.h"
#include "BaseGeometry/Arc2D.h"

using namespace std;
using namespace Robotics;
using namespace Robotics::GameTheory;
using namespace IDS;
using namespace IDS::BaseGeometry;

const int g_period = 5;

//////////////////////////////////////////////////////////////////////////
bool Agent::getRandomFeasibleAction(std::vector<AgentPosition> const& _feasible, AgentPosition & r_pos) const
{
	int l_tot = _feasible.size();
	if(l_tot = 0)
		return false;

	int l_rand = getRandomValue(l_tot);
	r_pos = _feasible[l_rand];

	return true;
}

//////////////////////////////////////////////////////////////////////////
void Agent::setCurrentPosition(AgentPosition const& pos)
{
	m_currentPosition = pos;
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
	return space->isOut(m_currentPosition);
}

//////////////////////////////////////////////////////////////////////////
void Agent::sleep()
{
	return setStatus(Agent::STANDBY);
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
std::vector<AgentPosition> Agent::getFeasibleActions( std::shared_ptr<DiscretizedArea> _space ) const
{
	AreaCoordinate l_currCoord = _space->getCoordinate( m_currentPosition.getPoint2D() ); 

	std::vector<AreaCoordinate> l_squares = _space->getStandardApproachableValidSquares(l_currCoord); // prende 8 punti adiacenti e li mette in l_aquare

	std::vector<AgentPosition> l_result;
	for( size_t i = 0; i < l_squares.size(); ++i )
	{
		// AgentPosition takes point and cameraPosition and put them in l_result
		l_result.push_back( AgentPosition(_space->getPosition(l_squares[i]), m_currentPosition.m_camera) );
	}

	return l_result; // return vector of all AgentPosition possible
}

//In this function one among all FeasibleAction is selected 
//////////////////////////////////////////////////////////////////////////
AgentPosition Agent::selectRandomFeasibleAction(std::shared_ptr<DiscretizedArea> _space)
{
	std::vector<AgentPosition> l_feasible = this->getFeasibleActions(_space); // tutte le posizioni possibili in base a dove mi trovo
	if(l_feasible.empty())
		return m_currentPosition;
	else
	{
		//this->removeBestTrajectoryFromFeasible(l_feasible);

		int l_value = getRandomValue( int( l_feasible.size() ) ); // ne prende una a caso
		return l_feasible[l_value];
	}
}

//////////////////////////////////////////////////////////////////////////
bool Agent::equals(std::shared_ptr<Agent> _other) const
{
	return _other->m_id == m_id;
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
std::shared_ptr<Sink> Agent::toSink()
{
	return dynamic_pointer_cast<Sink>(this->shared_from_this());
}

//////////////////////////////////////////////////////////////////////////
void Agent::moveToNextPosition()
{
	m_currentPosition = m_nextPosition;
}
