#include "PIPIPAlgorithm.h"
#include "DiscretizedArea.h"
#include "Agent.h"
#include "Guard.h"
#include "Thief.h"
#include "Probability.h"
#include "CoverageUtility.h"

#include <sstream>
#include <string>

#include "windows.h"

using namespace std;
using namespace Robotics;
using namespace Robotics::GameTheory;
using namespace IDS::BaseGeometry;

//////////////////////////////////////////////////////////////////////////
PIPIPAlgorithm::PIPIPAlgorithm(std::shared_ptr<DiscretizedArea> _space) 
	: LearningAlgorithm(), m_space(_space)
{}

//////////////////////////////////////////////////////////////////////////
void PIPIPAlgorithm::initialize()
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
std::string PIPIPAlgorithm::getExplorationRateStr()
{
	double l_exploration = this->computeExplorationRate();
	std::ostringstream strs;
	strs << l_exploration;
	return strs.str();
}

//////////////////////////////////////////////////////////////////////////
double PIPIPAlgorithm::getExplorationRate()
{
	return this->computeExplorationRate();
}

//////////////////////////////////////////////////////////////////////////
double PIPIPAlgorithm::computeIrrationalRate()
{
	return 1./3.;
}

//////////////////////////////////////////////////////////////////////////
double PIPIPAlgorithm::computeDeltaMemoryBenefit(std::shared_ptr<Guard> _agent)
{
	return _agent->getMemory().getDeltaMemoryBenefit();
}

//////////////////////////////////////////////////////////////////////////
void PIPIPAlgorithm::update(std::shared_ptr<Guard> _agent)
{
	if(!_agent->isRunning())
		//	Inizia una nuova traiettoria (sperimentale o no!)
	{
		//	ogni agente guardia sceglie il proprio tasso di esplorazione:
		double l_explorationRate = this->computeExplorationRate();

		//	ogni agente guardia estrae se sperimentare nuove azioni o no
		bool l_agentHasToExplore = agentHasToExperiments(l_explorationRate);

		double l_delta = this->computeDeltaMemoryBenefit(_agent);
		if(l_delta < 0)
			// irrational:
		{
			if(l_agentHasToExplore)
			{
				_agent->startExperiment(l_explorationRate);
				//this->selectBestMemoryAction(_agent);
				//this->selectRandomFeasibleAction(_agent);
			}
			else
			{
				//	ogni agente guardia sceglie il proprio tasso di esplorazione:
				double l_irrationalFactor = this->computeIrrationalRate();
				double l_irrationalRate = (1.-l_explorationRate) * l_irrationalFactor * pow(l_explorationRate, l_delta);

				//	ogni agente guardia estrae se sperimentare nuove azioni o no
				bool l_agentHasToBeIrrational = agentHasToExperiments( l_irrationalRate );

				if(l_agentHasToBeIrrational)
				{
					//this->selectWorstMemoryAction(_agent);
					_agent->followBestTrajectory(l_explorationRate, true);
				}
				else
				{
					//this->selectBestMemoryAction(_agent);
					_agent->followBestTrajectory(l_explorationRate, false);
				}
			}
		}
		else
		{
			if(l_agentHasToExplore)
			{
				_agent->startExperiment(l_explorationRate);
			}
			else
			{
				_agent->followBestTrajectory(l_explorationRate, true);
			}
		}
	}

	_agent->selectNextAction(m_space);

	//	ogni agente guardia muove verso la nuova posizione
	_agent->moveToNextPosition();

	return;
}

//////////////////////////////////////////////////////////////////////////
void PIPIPAlgorithm::communicate(std::shared_ptr<Guard> _agent)
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
void PIPIPAlgorithm::compute(std::shared_ptr<Guard> _agent)
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

	_agent->setCurrentPayoff(l_benefit);

	//	ogni agente guardia identifica le nuove azioni feasible:
	//this->computeFeasibleActions(_agent);
	return;
}

//////////////////////////////////////////////////////////////////////////
void PIPIPAlgorithm::updateCounterOfVisibleSquare( std::shared_ptr<Guard> _agent )
{
	std::set<SquarePtr> l_visible = _agent->getVisibleSquares(m_space);
	SquarePtr l_currentSquare = m_space->getSquare( m_space->getCoordinate(_agent->getCurrentPosition().getPoint2D()) );
	for(std::set<SquarePtr>::iterator it = l_visible.begin(); it != l_visible.end(); ++it)
	{
		(*it)->increaseCounter();
	}
}

//////////////////////////////////////////////////////////////////////////
void PIPIPAlgorithm::updateCounterOfVisibleSquare()
{
	for(set<shared_ptr<Guard>>::iterator it = m_guards.begin(); it != m_guards.end(); ++it)
	{
		//m_space->updateSquaresCounter(*it);
		updateCounterOfVisibleSquare(*it);
	}
}

bool PIPIPAlgorithm::forwardOneStep()
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
		//	ogni agente guardia comunica con i vicini:
		//this->communicate(*it);

		//	ogni agente guardia calcola la prima utilità:
		this->compute(*it);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
bool PIPIPAlgorithm::forwardOneStep(std::shared_ptr<Guard> _agent)
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
void PIPIPAlgorithm::computeNextPosition()
{
	for(std::set<GuardPtr>::iterator it = m_guards.begin(); it != m_guards.end(); ++it)
		this->forwardOneStep(*it);
}

////////////////////////////////////////////////////////////////////////////
//void PIPIPAlgorithm::selectRandomFeasibleAction(std::shared_ptr<Guard> _agent)
//{
//	std::vector<AgentPosition> l_feasible = _agent->getFeasibleActions(m_space);
//	if(l_feasible.empty())
//		_agent->setNextPosition();
//	else
//	{
//		int l_value = getRandomValue( int( l_feasible.size() ) );
//		_agent->setNextPosition(l_feasible[l_value]);
//	}
//	return;
//}

////////////////////////////////////////////////////////////////////////////
//void PIPIPAlgorithm::selectBestMemoryAction(std::shared_ptr<Agent> _agent)
//{
//	std::vector< MemoryAgentPosition > l_memory = _agent->getMemory();
//	std::vector<AgentPosition> l_feasibleposition = _agent->getFeasibleActions( m_space );
//	_agent->setBestPosition(l_memory, l_feasibleposition, true);
//}

////////////////////////////////////////////////////////////////////////////
//void PIPIPAlgorithm::selectWorstMemoryAction(std::shared_ptr<Agent> _agent)
//{
//	std::vector< MemoryAgentPosition > l_memory = _agent->getMemory();
//	std::vector<AgentPosition> l_feasibleposition = _agent->getFeasibleActions( m_space );
//	_agent->setBestPosition(l_memory, l_feasibleposition, false);
//}

//////////////////////////////////////////////////////////////////////////
void PIPIPAlgorithm::resetCounter()
{
	m_space->resetCounter();
}

//////////////////////////////////////////////////////////////////////////
void PIPIPAlgorithm::resetValue()
{
	m_space->resetValue();
}

//////////////////////////////////////////////////////////////////////////
//void PIPIPAlgorithm::updateTime() 
//{
//	resetCounter();

//	ogni agente guardia identifica le nuove azioni feasible per popolare l'area:
//updateCounterOfVisibleSquare();
//	++m_time;
//}

//////////////////////////////////////////////////////////////////////////
void PIPIPAlgorithm::monitoringThieves(std::set< ThiefPtr > const& _agents)
{
	for(std::set< ThiefPtr >::const_iterator it = _agents.begin(); it != _agents.end(); ++it)
	{
		AgentPosition l_thiefPos = (*it)->getCurrentPosition();
		m_space->setThiefPosition(l_thiefPos);
	}
}

/////////////////////////////////////////////////////////////////////////
void PIPIPAlgorithm::getGuardsPosition(std::vector<AgentPosition> & _pos)
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
void PIPIPAlgorithm::getGuardsSquare(std::vector<std::pair<SquarePtr,int>> & _pos)
{
	_pos.clear();
	_pos.reserve(m_guards.size());
	for(auto it = m_guards.begin(); it != m_guards.end(); ++it)
	{
		GuardPtr l_agent = *it;
		if(l_agent->isGuard())
		{
			_pos.push_back(  std::make_pair(m_space->getSquare( l_agent->getCurrentPosition().getPoint2D() ),
				double(l_agent->actualActionIndex()) / double(l_agent->totalActions()) * 255. ) );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void PIPIPAlgorithm::getGuardsCoverage( std::vector< std::vector<IDS::BaseGeometry::Point2D> > & _areas)
{
	for(auto it = m_guards.begin(); it != m_guards.end(); ++it)
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
int PIPIPAlgorithm::getGlobalTrajectoryCoverage()
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
double PIPIPAlgorithm::getBenefitValue()
{
	double l_total = 0.;
	for(auto it = m_guards.begin(); it != m_guards.end(); ++it)
	{
		GuardPtr l_agent = *it;
		l_total += l_agent->getCurrentPayoff();
	}
	return l_total;
}

//////////////////////////////////////////////////////////////////////////
double PIPIPAlgorithm::getPotentialValue()
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
