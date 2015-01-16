#include "LearningAlgorithm.h"

#include "Guard.h"
#include "CoverageUtility.h"

#include "DiscretizedArea.h"
#include "Thief.h"
#include "Sink.h"
#include "Probability.h"

#include <sstream>
#include <string>
#include <set>

using namespace std;
using namespace Robotics;
using namespace Robotics::GameTheory;
using namespace IDS::BaseGeometry;

//////////////////////////////////////////////////////////////////////////
double LearningAlgorithm::computeExplorationRate(std::shared_ptr<Guard> _agent)
{
	if( fabs(m_experimentalRate) > IDSMath::TOLERANCE )
		return m_experimentalRate;

	if(!_agent)
		_agent = *m_guards.begin();

	double rate = max(double(Robotics::GameTheory::DISCRETIZATION_COL), double(Robotics::GameTheory::DISCRETIZATION_ROW)) + 1.;
	return pow(double(m_time)/double(_agent->getTrajectoryLength()) , -double(m_guards.size())/rate); 
}

//////////////////////////////////////////////////////////////////////////
int LearningAlgorithm::getNumberOfSteps(double _stopRate)
{
	if( _stopRate < 0 || fabs(m_experimentalRate) > IDSMath::TOLERANCE )
		return N_MAX;
	
	return 5000; // CONTROLLARE
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
void LearningAlgorithm::initialize()
{
	this->updateCounterOfVisibleSquare();

	for(std::set<GuardPtr>::iterator it = m_guards.begin(); it != m_guards.end(); ++it)
	{
		//	ogni agente guardia comunica con i vicini:
		this->communicate(*it);

		//	ogni agente guardia calcola la propria prima utilità:
		this->compute(*it);

		//	ogni agente guardia compie un'azione random:
		AgentPosition pos = (*it)->selectRandomFeasibleAction(m_space);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
std::string LearningAlgorithm::getExplorationRateStr()
{
	double l_exploration = this->computeExplorationRate();
	std::ostringstream strs;
	strs << l_exploration;
	return strs.str();
}

//////////////////////////////////////////////////////////////////////////
double LearningAlgorithm::getExplorationRate()
{
	return this->computeExplorationRate();
}

//////////////////////////////////////////////////////////////////////////
void LearningAlgorithm::communicate(std::shared_ptr<Guard> _agent)
{
	// ogni agente guardia comunica ai vicini quali riquadri può esaminare:

	//	1) recupero il gruppo di agenti con cui posso comunicare
	set<GuardPtr> l_communicableAgents = _agent->getCommunicableAgents(m_guards);

	//	2) recupero i riquadri che riesco a vedere
	set<SquarePtr> l_visibleSquares = _agent->getVisibleSquares(m_space);

	//	3) Spedisco il messaggio
	for(set<GuardPtr>::iterator it = l_communicableAgents.begin(); it != l_communicableAgents.end(); ++it)
	{
		GuardPtr l_agent = *it;
		l_agent->receiveMessage(l_visibleSquares);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
void LearningAlgorithm::compute(std::shared_ptr<Guard> _agent)
{
	//	ogni agente guardia identifica la propria utilità:
	double l_benefit = 0;
	std::set<std::shared_ptr<Square> > l_squares = _agent->getVisibleSquares(m_space);
	for(std::set<std::shared_ptr<Square> >::iterator it = l_squares.begin(); it != l_squares.end(); ++it)
	{
		SquarePtr l_square = *it;
		if(!l_square->isValid())
			continue;
		int l_nq = l_square->getTheNumberOfAgent();
		double l_value = l_square->getThiefValue();
		l_benefit += l_value / double(l_nq);
	}
	l_benefit -= _agent->computeCurrentCosts();

	l_benefit -= _agent->computeBatteryCosts(m_space);

	_agent->setCurrentPayoff(l_benefit);

	// Battery
	std::shared_ptr<Square> l_square = m_space->getSquare(_agent->getCurrentPosition().getPoint2D());
	_agent->updateBattery(100.*LOSTBATTERY_PERSTEP*l_square->getEnergyValue());

	//	ogni agente guardia identifica le nuove azioni feasible:
	//this->computeFeasibleActions(_agent);
	return;
}

//////////////////////////////////////////////////////////////////////////
void LearningAlgorithm::updateCounterOfVisibleSquare( std::shared_ptr<Guard> _agent )
{
	std::set<SquarePtr> l_visible = _agent->getVisibleSquares(m_space);
	for(std::set<SquarePtr>::iterator it = l_visible.begin(); it != l_visible.end(); ++it)
	{
		(*it)->increaseCounter();
	}
}

//////////////////////////////////////////////////////////////////////////
void LearningAlgorithm::updateCounterOfVisibleSquare()
{
	for(auto it = m_guards.begin(); it != m_guards.end(); ++it)
	{
		updateCounterOfVisibleSquare(*it);
	}
}

///
bool LearningAlgorithm::forwardOneStep()
{
	double l_rate = this->computeExplorationRate();
	if(l_rate < 1.e-5)
		return false;

	//	UPDATE
	for(set<GuardPtr>::iterator it = m_guards.begin(); it!= m_guards.end(); ++it)
	{
		//	ogni agente guardia aggiorna la prossima azione da compiere:
		this->update(*it);
	}

	updateCounterOfVisibleSquare();

	//	COMMUNICATE & COMPUTE:
	for(set<GuardPtr>::iterator it = m_guards.begin(); it!= m_guards.end(); ++it)
	{
		//	ogni agente guardia calcola la prima utilità:
		this->compute(*it);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
bool LearningAlgorithm::forwardOneStep(std::shared_ptr<Guard> _agent)
{
	double l_rate = this->computeExplorationRate();
	if(l_rate < 1.e-5)
		return false;

	//	ogni agente guardia aggiorna la prossima azione da compiere:
	this->update(_agent);

	//	ogni agente guardia comunica con i vicini:
	this->communicate(_agent);

	//	ogni agente guardia calcola la prima utilità:
	this->compute(_agent);

	return true;
}

//////////////////////////////////////////////////////////////////////////
void LearningAlgorithm::computeNextPosition()
{
	for(std::set<GuardPtr>::iterator it = m_guards.begin(); it != m_guards.end(); ++it)
		this->forwardOneStep(*it);
}

//////////////////////////////////////////////////////////////////////////
void LearningAlgorithm::resetCounter()
{
	m_space->resetCounter();
}

//////////////////////////////////////////////////////////////////////////
void LearningAlgorithm::resetValue()
{
	m_space->resetValue();
}

//////////////////////////////////////////////////////////////////////////
void LearningAlgorithm::monitoringThieves(std::set< ThiefPtr > const& _agents)
{
	for(std::set< ThiefPtr >::const_iterator it = _agents.begin(); it != _agents.end(); ++it)
	{
		AgentPosition l_thiefPos = (*it)->getCurrentPosition();
		m_space->setThiefPosition(l_thiefPos);
	}
}

//////////////////////////////////////////////////////////////////////////
void LearningAlgorithm::monitoringSinks(std::set< SinkPtr > const& _agents)
{
	for(std::set< SinkPtr >::const_iterator it = _agents.begin(); it != _agents.end(); ++it)
	{
		AgentPosition l_sinkPos = (*it)->getCurrentPosition();
		m_space->setSinkPosition(l_sinkPos);
	}
}

/////////////////////////////////////////////////////////////////////////
void LearningAlgorithm::getGuardsPosition(std::vector<AgentPosition> & _pos)
{
	_pos.clear();
	_pos.reserve(m_guards.size());
	for(set<GuardPtr>::iterator it = m_guards.begin(); it != m_guards.end(); ++it)
	{
		GuardPtr l_agent = *it;
		if(l_agent->isGuard())
		{
			_pos.push_back( l_agent->getCurrentPosition() );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void LearningAlgorithm::getGuardsSquare(std::vector<std::pair<SquarePtr, AgentActionIndex>> & _pos)
{
	_pos.clear();
	_pos.reserve(m_guards.size());
	for(set<GuardPtr>::iterator it = m_guards.begin(); it != m_guards.end(); ++it)
	{
		GuardPtr l_agent = *it;
		if(l_agent->isGuard())
		{
			_pos.push_back( std::make_pair(m_space->getSquare( l_agent->getCurrentPosition().getPoint2D() ),
				AgentActionIndex( l_agent->actualActionIndex(), l_agent->totalActions() ) ) );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void LearningAlgorithm::getGuardsCoverage( std::vector< std::vector<IDS::BaseGeometry::Point2D> > & _areas)
{
	for(set<GuardPtr>::iterator it = m_guards.begin(); it != m_guards.end(); ++it)
	{
		std::vector<IDS::BaseGeometry::Point2D> l_agentArea;
		GuardPtr l_agent = *it;
		Shape2D l_area = l_agent->getVisibleArea();
		std::vector<Curve2D> l_bound = l_area.getBoundary();
		for(size_t i = 0 ; i < l_bound.size(); ++i)
		{
			std::vector<IDS::BaseGeometry::Point2D> l_partial = l_bound[i].approxByPoints(1);
			l_agentArea.insert(l_agentArea.end(), l_partial.begin(), l_partial.end());
		}
		_areas.push_back(l_agentArea);
	}
}

//////////////////////////////////////////////////////////////////////////
int LearningAlgorithm::getGlobalTrajectoryCoverage()
{
	std::set< std::shared_ptr<Square> > l_globalSquare;
	for(set<GuardPtr>::iterator it = m_guards.begin(); it != m_guards.end(); ++it)
	{
		GuardPtr l_guard = *it;
		std::set< std::shared_ptr<Square> > l_squares = l_guard->getTrajectoryCoverage();
		l_globalSquare.insert(l_squares.begin(), l_squares.end());
	}
	return l_globalSquare.size();
}

//////////////////////////////////////////////////////////////////////////
double LearningAlgorithm::getBenefitValue()
{
	double l_total = 0.;
	for(std::set<GuardPtr>::iterator it = m_guards.begin(); it != m_guards.end(); ++it)
	{
		GuardPtr l_agent = *it;
		l_total += l_agent->getCurrentPayoff();
	}
	return l_total;
}

//////////////////////////////////////////////////////////////////////////
double LearningAlgorithm::getPotentialValue()
{
	double l_total = 0.;
	for(size_t i = 0;  i < m_space->m_lattice.size(); ++i)
	{
		double l_partialTotal = 0.;
		double l_Wq = m_space->m_lattice[i]->getThiefValue();
		for(int j = 1; j <= m_space->m_lattice[i]->getTheNumberOfAgent(); ++j)
		{
			l_partialTotal += l_Wq / j;
		}
		l_total += l_partialTotal;
	}
	return l_total;
}

//////////////////////////////////////////////////////////////////////////
double LearningAlgorithm::getBatteryValue()
{
	double l_total = 0.;
	for(std::set<GuardPtr>::iterator it = m_guards.begin(); it != m_guards.end(); ++it)
	{
		GuardPtr l_agent = *it;
		l_total += l_agent->getBatteryValue();
	}
	return l_total / (double(m_guards.size()) * MAXIMUM_BATTERY);
}

//////////////////////////////////////////////////////////////////////////
std::string LearningAlgorithm::getBatteryValueStr()
{
	double l_value = this->getBatteryValue();
	std::ostringstream strs;
	strs << l_value;
	return strs.str();
}

