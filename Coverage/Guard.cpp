#include "Guard.h"
#include "DiscretizedArea.h"
#include "Probability.h"
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
	, m_currentMood(Mood::C)
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

////////////////////////////////////////////////////////////////////////////
//std::vector<AgentPosition> Agent::getFeasibleActions( std::shared_ptr<DiscretizedArea> _space, AreaCoordinate _currCoord, int time ) const
//{
//	std::vector<AreaCoordinate> l_squares = _space->getStandardApproachableValidSquares(_currCoord);
//
//	std::vector<AgentPosition> l_result;
//	for(size_t i = 0; i < l_squares.size(); ++i )
//	{
//		l_result.push_back( AgentPosition(_space->getPosition(l_squares[i]), m_position.m_camera) );
//	}
//
//	return l_result;
//}

////////////////////////////////////////////////////////////////////////////
//void Agent::selectRandomFeasibleTrajectory(std::shared_ptr<DiscretizedArea> _space)
//{
//	std::vector<AgentPosition> l_feasible = this->getFeasibleTrajectories(_space);
//	if(l_feasible.empty())
//		this->setNextTrajectory();
//	else
//	{
//		this->removeBestTrajectoryFromFeasible(l_feasible);
//
//		int l_value = getRandomValue( int( l_feasible.size() ) );
//		this->setNextTrajectory(l_feasible[l_value]);
//	}
//	return;
//}

////////////////////////////////////////////////////////////////////////////
//std::set<std::shared_ptr<Agent> > Agent::getCommunicableAgents(std::set<std::shared_ptr<Agent> > const& _guards) const
//{
//	std::set<std::shared_ptr<Agent> > l_result;
//	for(std::set<std::shared_ptr<Agent> >::const_iterator it = _guards.begin(); it != _guards.end(); ++it)
//	{
//		AgentPtr l_agent = *it;
//		if( this->equals(l_agent) )
//			continue;
//
//		if( this->m_position.communicable( l_agent ) )
//		{
//			l_result.insert(l_agent);
//		}
//	}
//	return l_result;
//}

////////////////////////////////////////////////////////////////////////////
//void Agent::receiveMessage( std::set<std::shared_ptr<Square> > const& _visible)
//{
//	// Ogni agente deve conoscere il numero di agenti che potenzialmente esaminano un riquadro:
//	// in questo modo possono determinare la quantità n_q(s) per ogni q feasible
//	// e quindi calcolare la propria utilità
//}

////////////////////////////////////////////////////////////////////////////
//double Agent::computeCosts() const
//{
//	return m_position.computeCosts();
//}

////////////////////////////////////////////////////////////////////////////
//void Agent::removeBestPositionFromFeasible(std::vector<AgentPosition> & _feasible)
//{
//	Point2D l_posBest = m_bestPosition.getPoint2D();
//
//	if(l_posBest.isValid())
//	{
//		for(size_t j = 0; j < _feasible.size();)
//		{
//			Point2D l_posFeas = _feasible[j].getPoint2D();
//			bool found = false;
//			if(l_posFeas.distance(l_posBest) < 1.e-1)
//			{
//				found = true;
//				break;
//			}
//			if(found)
//				_feasible.erase(_feasible.begin()+j);
//			else
//				++j;
//		}
//	}
//}

////////////////////////////////////////////////////////////////////////////
//void Agent::setBestPosition(std::vector< MemoryAgentPosition > const& memory, std::vector<AgentPosition> const& _feasibleposition, bool _best)
//{
//	std::vector< MemoryAgentPosition > l_memory = memory;
//	// verifica che le azioni in memory siano feasible!
//	for(size_t j = 0; j < l_memory.size();)
//	{
//		Point2D l_posMem = l_memory[j].m_action.getPoint2D();
//		bool found = false;
//		for(size_t i = 0; i < _feasibleposition.size(); ++i)
//		{
//			Point2D l_posFeas = _feasibleposition[i].getPoint2D();
//			if(l_posFeas.distance(l_posMem) < 1.e-1)
//			{
//				found = true;
//				break;
//			}
//		}
//		if(found || m_position.getPoint2D().distance(l_posMem) < 1.e-1)
//			++j;
//		else
//			l_memory.erase(l_memory.begin()+j);
//	}
//
//	if( l_memory.empty() )
//	{
//		m_bestPosition = m_position;
//	}
//	else
//	{
//		double l_bestPayoff = l_memory.at(0).m_payoff;
//		m_bestPosition = l_memory.at(0).m_action;
//		for(size_t i= 1; i < l_memory.size(); ++i)
//		{
//			bool l_update = false;
//			if(_best)
//			{
//				l_update = l_memory[i].m_payoff > l_bestPayoff;
//			}
//			else
//			{
//				l_update = l_memory[i].m_payoff < l_bestPayoff;
//			}
//
//			if(l_update)
//			{
//				m_bestPosition = l_memory[i].m_action;
//				l_bestPayoff = l_memory[i].m_payoff;
//			}
//		}
//	}	
//	return;
//}

////////////////////////////////////////////////////////////////////////////
//IDS::BaseGeometry::Shape2D Agent::getVisibleArea(int i) const
//{
//	return m_position.getVisibleArea();
//}

#pragma region PARETO

Mood Guard::computeMood(double _explorationRate, double _maxValue)
{
	int l_index = getBestTrajectoryIndex(true);
	
	if( l_index >= 0 &&
		m_memory.m_elems[l_index].equals(m_currentTrajectory, m_currentTrajectoryPayoff) && 
		m_memory.m_elems[l_index].m_mood == Mood::C)
	{
		return Mood::C;
	}

	double l_explorationRate = pow( _explorationRate, 1 - m_currentTrajectoryPayoff );
	bool l_agentHasToExperiments = agentHasToExperiments(l_explorationRate);
	if(l_agentHasToExperiments)
		return Mood::C;
	else
		return Mood::D;
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
void Guard::reset(double _explorationRate, double _maxValue)
{
	if( !(m_currentTrajectory.size() == 0) )
	{
		Mood l_mood = computeMood(_explorationRate, _maxValue);

		updateMemoryTrajectories();

		setCurrentMood(l_mood);
	}
	m_currentTrajectory.clear();
	m_currentTrajectoryPayoff = 0.;

	m_oldCoverage = m_coverage;
	m_coverage.clear();
}

///
void Guard::startExperiment(double _explorationRate, double _maxValue)
{
	reset(_explorationRate, _maxValue);
	m_exploring = -1;
}

///
void Guard::followBestTrajectory(double _explorationRate, double _maxValue, bool best)
{
	reset(_explorationRate, _maxValue);
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
	for(auto i = 0; i < l_feasible.size(); ++i)
	{
		if( _alreadyTested.find(i) != _alreadyTested.end() )
			continue;

		if( m_currentTrajectory.contains(l_feasible[i]) )
		{
			//std::cout << "selected an action already visited!" << std::endl;
			continue;
		}

		l_notControlledFeasibleActions.push_back( std::make_pair<AgentPosition, int>(l_feasible[i], i) );
	}

	if(l_notControlledFeasibleActions.empty())
		return m_currentPosition;
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
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///
AgentPosition MemoryGuardTrajectories::getNextPosition(int _indexBest, int _indexNext)
{
	return m_elems.at(_indexBest).m_memTrajectory.getPosition(_indexNext);
}

///
double MemoryGuardTrajectories::getDeltaMemoryBenefit()
{
	if(m_elems.size() == 0)
		return 0.;

	//computeBestWorstTrajectories();

	return (m_best > m_worst ? -1. : 1.) * (m_elems[m_best].m_payoff - m_elems[m_worst].m_payoff);
}

#pragma endregion