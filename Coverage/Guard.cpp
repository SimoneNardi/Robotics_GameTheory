#include "Guard.h"
#include "DiscretizedArea.h"
#include "Probability.h"
#include "CoverageUtility.h"

#include <utility>

using namespace Robotics::GameTheory;
using namespace IDS::BaseGeometry;

Guard::Guard( int _teamID, int _id, AgentPosition _position, int _trajectoryLength, int _memorySpace ) 
	: Agent(_id, _position)
	, m_teamID(_teamID)
	, m_currentTrajectoryPayoff(0.)
	, m_currentPayoff(0.)
	, m_currentTrajectory()
	, m_maxTrajectoryLength(_trajectoryLength)
	, m_memory(_memorySpace)
	, m_coverage()
	, m_oldCoverage()
	, m_currentMood(Content)
	, m_current_battery(MAXIMUM_BATTERY)
	, m_minimum_battery(MINIMUM_BATTERY)
	, m_maximum_battery(MAXIMUM_BATTERY)
{}

Guard::~Guard()
{}

//////////////////////////////////////////////////////////////////////////
std::vector<AgentPosition> Guard::getFeasibleActions( std::shared_ptr<DiscretizedArea> _space ) const
{
	AreaCoordinate l_currCoord = _space->getCoordinate( m_currentPosition.getPoint2D() );

	std::vector<AreaCoordinate> l_squares = _space->getStandardApproachableValidSquares(l_currCoord);
	_space->addSpecialApproachableValidSquares(l_currCoord, l_squares);

	std::vector<AgentPosition> l_result;
	for(size_t i = 0; i < l_squares.size(); ++i )
	{
		//int l_distance = _space->getDistance(l_currCoord, l_squares[i]);
		l_result.push_back( AgentPosition(_space->getPosition(l_squares[i]), m_currentPosition.m_camera) );
	}

	return l_result;
}

//////////////////////////////////////////////////////////////////////////
int Guard::getBestTrajectoryIndex(bool _best)
{
	//m_memory.computeBestWorstTrajectories();
	return _best ? m_memory.m_best : m_memory.m_worst;
}

//////////////////////////////////////////////////////////////////////////
void RemoveBestPositionFromFeasible(std::vector<AgentPosition> & _feasible, AgentPosition const& _bestPosition)
{
	Point2D l_posBest = _bestPosition.getPoint2D();

	if(l_posBest.isValid())
	{
		for(size_t j = 0; j < _feasible.size();)
		{
			Point2D l_posFeas = _feasible[j].getPoint2D();
			bool found = false;
			if(l_posFeas.distance(l_posBest) < 1.e-1)
			{
				found = true;
				break;
			}
			if(found)
				_feasible.erase(_feasible.begin()+j);
			else
				++j;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void Guard::removeBestPositionFromFeasible(std::vector<AgentPosition> & _feasible)
{
	MemoryGuardTrajectory l_best;
	if( !m_memory.bestTrajectory(l_best))
		return;

	if(!(m_currentTrajectory==l_best.m_memTrajectory))
		return;

	if( m_currentTrajectory.isLast(m_maxTrajectoryLength) )
	{
		RemoveBestPositionFromFeasible(_feasible, l_best.m_memTrajectory.getLastPosition());
	}
}

//////////////////////////////////////////////////////////////////////////
std::set<std::shared_ptr<Guard> > Guard::getCommunicableAgents(std::set< std::shared_ptr<Guard> > const& _guards) const
{
	std::set<std::shared_ptr<Guard> > l_result;
	for(std::set<std::shared_ptr<Guard> >::const_iterator it = _guards.begin(); it != _guards.end(); ++it)
	{
		std::shared_ptr<Guard> l_agent = *it;
		if( this->equals(l_agent) )
			continue;

		if( this->m_currentPosition.communicable( l_agent ) )
		{
			l_result.insert(l_agent);
		}
	}
	return l_result;
}

//////////////////////////////////////////////////////////////////////////
double Guard::computeCurrentCosts() const
{
	return m_currentPosition.computeCosts();
}

//////////////////////////////////////////////////////////////////////////
void Guard::updateMemoryTrajectories()
{
	m_memory.add(MemoryGuardTrajectory(m_currentTrajectory, m_currentTrajectoryPayoff, m_currentMood));
	m_memory.computeBestWorstTrajectories();
}

#pragma region PARETO

Mood Guard::computeMood(double _explorationRate)
{
	int l_index = getBestTrajectoryIndex(true);
	
	if( l_index >= 0 &&
		m_memory.m_elems[l_index].equals(m_currentTrajectory, m_currentTrajectoryPayoff) && 
		m_memory.m_elems[l_index].m_mood == Content)
	{
		return Content;
	}

	double l_explorationRate = pow( _explorationRate, 1 - m_currentTrajectoryPayoff );
	bool l_agentHasToExperiments = agentHasToExperiments(l_explorationRate);
	if(l_agentHasToExperiments)
		return Content;
	else
		return Discontent;
}

#pragma endregion


#pragma region DISLALGORITHM

///
bool Guard::isRunning() const
{
	const int len = m_currentTrajectory.size();
	return len > 0 && len < m_maxTrajectoryLength;
	//return m_currentTrajectory.size() < m_maxTrajectoryLength; 
}

///
void Guard::reset(double _explorationRate)
{
	if( !(m_currentTrajectory.size() == 0) )
	{
		Mood l_mood = computeMood(_explorationRate);

		updateMemoryTrajectories();

		setCurrentMood(l_mood);
	}
	m_currentTrajectory.clear();
	m_currentTrajectoryPayoff = 0.;

	m_oldCoverage = m_coverage;
	m_coverage.clear();

	int l_period = computePeriod();
	if(l_period != m_maxTrajectoryLength)
	{
		resetMemory();
		updatePeriod(l_period);
	}

}

///
void Guard::startExperiment(double _explorationRate)
{
	reset(_explorationRate);
	m_exploring = -1;
}

///
void Guard::followBestTrajectory(double _explorationRate, bool best)
{
	reset(_explorationRate);
	m_exploring = getBestTrajectoryIndex(best);
}

///
void Guard::selectNextAction(std::shared_ptr<DiscretizedArea> _space)
{
	switch(m_exploring)
	{
	case -1:
		this->setNextPosition( selectNextFeasiblePosition(_space) );
		break;
	default:
		this->setNextPosition( m_memory.getNextPosition(m_exploring, m_currentTrajectory.size()) );
	}
}

///
void Guard::moveToNextPosition()
{
	m_currentPayoff = 0.;
	Agent::moveToNextPosition();

	updateBattery(-LOSTBATTERY_PERSTEP);
}

//////////////////////////////////////////////////////////////////////////
std::set<std::shared_ptr<Square> > Guard::getVisibleSquares( std::shared_ptr<DiscretizedArea> _space )
{
	std::set<std::shared_ptr<Square> > result;
	std::vector<AreaCoordinate> l_coord = m_currentPosition.getCoverage(_space);
	for(size_t i = 0; i < l_coord.size(); ++i)
	{
		result.insert(_space->getSquare(l_coord[i]));
	}
	collectVisitedSquare(result);
	return result;
}

//////////////////////////////////////////////////////////////////////////
AgentPosition Guard::selectNextFeasiblePositionWithoutConstraint(std::shared_ptr<DiscretizedArea> _space)
{
	return Agent::selectRandomFeasibleAction(_space);
}

//////////////////////////////////////////////////////////////////////////
AgentPosition Guard::selectNextFeasiblePositionWithoutConstraint(std::shared_ptr<DiscretizedArea> _space, std::set<int> &_alreadyTested)
{
	std::vector<AgentPosition> l_feasible = this->getFeasibleActions(_space);

	std::vector< std::pair<AgentPosition, int> > l_notControlledFeasibleActions;
	for(size_t i = 0; i < l_feasible.size(); ++i)
	{
		if( _alreadyTested.find(i) != _alreadyTested.end() )
			continue;

		if( m_currentTrajectory.contains(l_feasible[i]) )
		{
			//std::cout << "An action already chosen has been selected!" << std::endl;
			continue;
		}

		l_notControlledFeasibleActions.push_back( std::make_pair<AgentPosition, int>(l_feasible[i], i) );
	}

	if(l_notControlledFeasibleActions.empty())
		return m_currentPosition;
	else if(_space->isThereASink())
	{
		std::vector<int> l_distanceNearestSink = _space->distanceFromNearestSink(l_notControlledFeasibleActions);

		double l_mindist = IDSMath::Infinity;
		int l_mindistIndex = -1;

		for(size_t i = 0; i < l_distanceNearestSink.size(); ++i)
		{
			if ( l_distanceNearestSink[i] < l_mindist )
			{
				l_mindist = l_distanceNearestSink[i]; 
				l_mindistIndex = i;
			}
		}

		_alreadyTested.insert(l_notControlledFeasibleActions[l_mindistIndex].second);
		return l_notControlledFeasibleActions[l_mindistIndex].first;
	}
	else
	{
		//this->removeBestTrajectoryFromFeasible(l_feasible);

		int l_value = getRandomValue( int( l_notControlledFeasibleActions.size() ) );
		_alreadyTested.insert(l_notControlledFeasibleActions[l_value].second);
		return l_notControlledFeasibleActions[l_value].first;
	}
}

//////////////////////////////////////////////////////////////////////////
AgentPosition Guard::selectNextFeasiblePosition(std::shared_ptr<DiscretizedArea> _space)
{
	// Feasible action è un azione di una traiettoria ancora da definire ma che deve rispettare i vincoli degli algoritmi dinamici.
	// Pesco il riquadro finchè non trovo un elemento accettabile!
	if(!this->isRunning())
	{
		AgentPosition l_selectedPosition = this->selectNextFeasiblePositionWithoutConstraint(_space);
		return l_selectedPosition;
	}

	AgentPosition l_selectedPosition = this->getCurrentPosition();
	SquarePtr l_source = _space->getSquare(m_currentTrajectory.getPosition(0).getPoint2D());

	int l_nodeDistance = 0;
	int k = 0;
	std::set<int> l_alreadyTested;
	do
	{
		l_selectedPosition = this->selectNextFeasiblePositionWithoutConstraint(_space, l_alreadyTested);
		
		//if( (k < 8) && (m_currentTrajectory.contains(l_selectedPosition)) )
		//	continue;
		
		SquarePtr l_selected = _space->getSquare( l_selectedPosition.getPoint2D() );

		l_nodeDistance = _space->getDistance(l_source, l_selected);
		k++;
	} 
	while(m_maxTrajectoryLength - m_currentTrajectory.size() < l_nodeDistance && k < 9);

	return l_selectedPosition;
}

void Guard::receiveMessage( std::set<std::shared_ptr<Square> > const& _visible)
{
	return; // CONTROLLARE
}

//////////////////////////////////////////////////////////////////////////
void Guard::setCurrentPayoff(double _benefit)
{
	m_currentPayoff = _benefit;

	m_currentTrajectory.add(m_currentPosition);
	m_currentTrajectoryPayoff += m_currentPayoff;
}

//////////////////////////////////////////////////////////////////////////
void Guard::setCurrentMood(Mood _state)
{
	m_currentMood = _state;
}

//////////////////////////////////////////////////////////////////////////
int Guard::actualActionIndex()
{
	return m_currentTrajectory.size();
}

//////////////////////////////////////////////////////////////////////////
int Guard::totalActions()
{
	return m_maxTrajectoryLength;
}

//////////////////////////////////////////////////////////////////////////
std::set< std::shared_ptr<Square> > Guard::getTrajectoryCoverage() const
{
	return m_oldCoverage;
}

//////////////////////////////////////////////////////////////////////////
void Guard::collectVisitedSquare(std::set<SquarePtr>const& _squares)
{
	m_coverage.insert(_squares.begin(), _squares.end());
}

//////////////////////////////////////////////////////////////////////////
void Guard::updateBattery(double value)
{
	m_current_battery += value;

	if(m_current_battery < 0)
		m_current_battery = 0;

	if(m_current_battery > m_maximum_battery)
		m_current_battery = m_maximum_battery;
}

//////////////////////////////////////////////////////////////////////////
void Guard::updatePeriod(int value)
{
	m_maxTrajectoryLength = value;
}

const double MAXIMUM_PERIOD = std::max(double(Robotics::GameTheory::DISCRETIZATION_COL), double(Robotics::GameTheory::DISCRETIZATION_ROW));

//////////////////////////////////////////////////////////////////////////
int Guard::computePeriod()
{
	int l_period = (m_minimum_battery - m_current_battery) / m_minimum_battery * MAXIMUM_PERIOD;
	if ( l_period < 1 )
		l_period = 1;
	
	return l_period;
}

//////////////////////////////////////////////////////////////////////////
double Guard::computeBatteryCosts(std::shared_ptr<DiscretizedArea> _space)
{
	const double l_gain = 1.;
	double l_distance = _space->getDistanceFromNearestSink(m_currentPosition.getPoint2D());
	
	double l_param = l_distance / std::max(double(Robotics::GameTheory::DISCRETIZATION_COL), double(Robotics::GameTheory::DISCRETIZATION_ROW));

	return l_gain * ( m_maxTrajectoryLength - 1 ) * l_param;
}

//////////////////////////////////////////////////////////////////////////
void Guard::resetMemory()
{
	m_exploring=-1;
	m_memory.reset();
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
AgentPosition MemoryGuardTrajectories::getNextPosition(int _indexBest, int _indexNext)
{
	return m_elems.at(_indexBest).m_memTrajectory.getPosition(_indexNext);
}

//////////////////////////////////////////////////////////////////////////
double MemoryGuardTrajectories::getDeltaMemoryBenefit()
{
	if(m_elems.size() == 0)
		return 0.;

	//computeBestWorstTrajectories();

	return (m_best > m_worst ? -1. : 1.) * (m_elems[m_best].m_payoff - m_elems[m_worst].m_payoff);
}

//////////////////////////////////////////////////////////////////////////
void MemoryGuardTrajectories::reset()
{
	m_best = -1;
	m_worst = -1;
	m_elems.clear();
}

#pragma endregion