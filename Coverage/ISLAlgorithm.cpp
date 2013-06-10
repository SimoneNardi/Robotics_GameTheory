#include "ISLAlgorithm.h"
#include "DiscretizedArea.h"
#include "Agent.h"
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
ISLAlgorithm::ISLAlgorithm(std::shared_ptr<DiscretizedArea> _space, int _memorySpace) 
	: LearningAlgorithm(), m_numMemory(_memorySpace), m_space(_space)
{}

//////////////////////////////////////////////////////////////////////////
void ISLAlgorithm::initialize()
{
	this->updateCounterOfVisibleSquare();

	for(std::set<AgentPtr>::iterator it = m_guards.begin(); it != m_guards.end(); ++it)
	{
		//	ogni agente guardia comunica con i vicini:
		this->communicate(*it);

		//	ogni agente guardia calcola la propria prima utilità:
		this->compute(*it);

		//	ogni agente guardia compie un'azione random:
		this->selectRandomFeasibleAction(*it);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
std::string ISLAlgorithm::getExplorationRate()
{
	double l_exploration = this->computeExplorationRate();
	std::ostringstream strs;
	strs << l_exploration;
	return strs.str();
}

//////////////////////////////////////////////////////////////////////////
double ISLAlgorithm::computeExplorationRate()
{
	//return 0.5;
	double rate = ( double(m_guards.size()) * (double(Robotics::GameTheory::DISCRETIZATION_COL) + double(Robotics::GameTheory::DISCRETIZATION_ROW) + 1) );
	return pow(double(m_time) , -1./rate);
}

//////////////////////////////////////////////////////////////////////////
void ISLAlgorithm::update(std::shared_ptr<Agent> _agent)
{
	static int count = 0;
	//	ogni agente guardia sceglie il proprio tasso di esplorazione:
	double l_explorationRate = this->computeExplorationRate();

	//	ogni agente guardia estrae se sperimentare nuove azioni o no
	bool l_agentHasToExperiments = agentHasToExperiments(l_explorationRate);
	if(l_agentHasToExperiments)
	{
		this->selectRandomFeasibleAction(_agent);
		++count;
	}
	else
	{
		this->selectBestMemoryAction(_agent);
	}

	//	ogni agente guardia muove verso la nuova posizione
	_agent->moveToNextPosition();

	//Sleep(10);

	return;
}

//////////////////////////////////////////////////////////////////////////
void ISLAlgorithm::communicate(std::shared_ptr<Agent> _agent)
{
	// ogni agente guardia comunica ai vicini quali riquadri può esaminare:

	//	1) recupero il gruppo di agenti con cui posso comunicare
	set<AgentPtr> l_communicableAgents = _agent->getCommunicableAgents(m_guards);

	//	2) recupero i riquadri che riesco a vedere
	set<SquarePtr> l_visibleSquares = _agent->getVisibleSquares(m_space);

	//	3) Spedisco il messaggio
	for(set<AgentPtr>::iterator it = l_communicableAgents.begin(); it != l_communicableAgents.end(); ++it)
	{
		AgentPtr l_agent = *it;
		l_agent->receiveMessage(l_visibleSquares);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
void ISLAlgorithm::compute(std::shared_ptr<Agent> _agent)
{
	//	ogni agente guardia identifica la propria utilità:
	double l_benefit = 0;
	std::set<std::shared_ptr<Square> > l_squares = _agent->getVisibleSquares(m_space);
	for(std::set<std::shared_ptr<Square> >::iterator it = l_squares.begin(); it != l_squares.end(); ++it)
	{
		SquarePtr l_square = *it;
		int l_nq = l_square->getTheNumberOfAgent();
		double l_value = l_square->getValue();
		l_benefit += l_value / double(l_nq);
	}
	l_benefit -= _agent->computeCosts();

	_agent->setCurrentPayoff(l_benefit);

	//	ogni agente guardia identifica le nuove azioni feasible:
	//this->computeFeasibleActions(_agent);
	return;
}

//////////////////////////////////////////////////////////////////////////
void ISLAlgorithm::updateCounterOfVisibleSquare( std::shared_ptr<Agent> _agent )
{
	std::set<SquarePtr> l_visible = _agent->getVisibleSquares(m_space);
	SquarePtr l_currentSquare = m_space->getSquare( m_space->getCoordinate(_agent->getPosition().getPoint2D()) );
	for(std::set<SquarePtr>::iterator it = l_visible.begin(); it != l_visible.end(); ++it)
	{
		//if( l_currentSquare->equals(*it) )
		//{
		//	bool check = l_currentSquare == (*it);
		//	continue;
		//}
		(*it)->increaseCounter();
	}
}

//////////////////////////////////////////////////////////////////////////
void ISLAlgorithm::updateCounterOfVisibleSquare()
{
	for(set<shared_ptr<Agent>>::iterator it = m_guards.begin(); it != m_guards.end(); ++it)
	{
		//m_space->updateSquaresCounter(*it);
		updateCounterOfVisibleSquare(*it);
	}
}

void ISLAlgorithm::forwardOneStep()
{
	//	UPDATE
	for(set<AgentPtr>::iterator it = m_guards.begin(); it!= m_guards.end(); ++it)
	{
		//	ogni agente guardia aggiorna la prossima azione da compiere:
		this->update(*it);
	}

	updateCounterOfVisibleSquare();

	//	COMMUNICATE & COMPUTE:
	for(set<AgentPtr>::iterator it = m_guards.begin(); it!= m_guards.end(); ++it)
	{
		//	ogni agente guardia comunica con i vicini:
		//this->communicate(*it);

		//	ogni agente guardia calcola la prima utilità:
		this->compute(*it);
	}
}

//////////////////////////////////////////////////////////////////////////
void ISLAlgorithm::forwardOneStep(std::shared_ptr<Agent> _agent)
{
	//	ogni agente guardia aggiorna la prossima azione da compiere:
	this->update(_agent);

	//	ogni agente guardia comunica con i vicini:
	this->communicate(_agent);

	//	ogni agente guardia calcola la prima utilità:
	this->compute(_agent);

	return;
}

//////////////////////////////////////////////////////////////////////////
void ISLAlgorithm::computeNextPosition()
{
	for(std::set<AgentPtr>::iterator it = m_guards.begin(); it != m_guards.end(); ++it)
		this->forwardOneStep(*it);
}

//////////////////////////////////////////////////////////////////////////
void ISLAlgorithm::selectRandomFeasibleAction(std::shared_ptr<Agent> _agent)
{
	std::vector<AgentPosition> l_feasible = _agent->getFeasibleActions(m_space);
	if(l_feasible.empty())
		_agent->setNextPosition();
	else
	{
	int l_value = getRandomValue( int( l_feasible.size() ) );
	_agent->setNextPosition(l_feasible[l_value]);
	}
	return;
}

//////////////////////////////////////////////////////////////////////////
void ISLAlgorithm::selectBestMemoryAction(std::shared_ptr<Agent> _agent)
{
	std::vector< MemoryAgentPosition > l_memory = _agent->getMemory();
	_agent->setNextPosition(l_memory);
}

//////////////////////////////////////////////////////////////////////////
void ISLAlgorithm::resetCounter()
{
	m_space->resetCounter();
	m_space->resetValue();
}

//////////////////////////////////////////////////////////////////////////
void ISLAlgorithm::updateTime() 
{
	resetCounter();
	
	//	ogni agente guardia identifica le nuove azioni feasible per popolare l'area:
	//updateCounterOfVisibleSquare();
	++m_time;
}

//////////////////////////////////////////////////////////////////////////
void ISLAlgorithm::monitoringThieves(std::set< AgentPtr > const& _agents)
{
	for(std::set< AgentPtr >::const_iterator it = _agents.begin(); it != _agents.end(); ++it)
	{
		AgentPtr l_agent = *it;
		if( l_agent->isThief() )
		{
			AgentPosition l_thiefPos = l_agent->getPosition();
			m_space->setThiefPosition(l_thiefPos);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void ISLAlgorithm::getGuardsPosition(std::vector<AgentPosition> & _pos)
{
	_pos.clear();
	_pos.reserve(m_guards.size());
	for(set<AgentPtr>::iterator it = m_guards.begin(); it != m_guards.end(); ++it)
	{
		AgentPtr l_agent = *it;
		if(l_agent->isGuard())
		{
			_pos.push_back( l_agent->getPosition() );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void ISLAlgorithm::getGuardsSquare(std::vector<SquarePtr> & _pos)
{
	_pos.clear();
	_pos.reserve(m_guards.size());
	for(set<AgentPtr>::iterator it = m_guards.begin(); it != m_guards.end(); ++it)
	{
		AgentPtr l_agent = *it;
		if(l_agent->isGuard())
		{
			_pos.push_back( m_space->getSquare( l_agent->getPosition().getPoint2D() ) );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void ISLAlgorithm::getGuardsCoverage( std::vector< std::vector<IDS::BaseGeometry::Point2D> > & _areas)
{
	for(set<AgentPtr>::iterator it = m_guards.begin(); it != m_guards.end(); ++it)
	{
		std::vector<IDS::BaseGeometry::Point2D> l_agentArea;
		AgentPtr l_agent = *it;
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

double ISLAlgorithm::getPotentialValue()
{
	double l_total = 0.;
	for(size_t i = 0;  i < m_space->m_lattice.size(); ++i)
	{
		double l_partialTotal = 0.;
		double l_Wq = m_space->m_lattice[i]->getValue();
		for(int j = 1; j <= m_space->m_lattice[i]->getTheNumberOfAgent(); ++j)
		{
			l_partialTotal += l_Wq / j;
		}
		l_total += l_partialTotal;
	}
	return l_total;
}
