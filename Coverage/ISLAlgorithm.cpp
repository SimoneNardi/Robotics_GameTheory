#include "ISLAlgorithm.h"
#include "DiscretizedArea.h"
#include "Agent.h"
#include "Probability.h"

using namespace std;
using namespace Robotics;
using namespace Robotics::GameTheory;

//////////////////////////////////////////////////////////////////////////
ISLAlgorithm::ISLAlgorithm(std::shared_ptr<DiscretizedArea> _space, int _memory) 
	: LearningAlgorithm(), m_numMemory(_memory), m_space(_space)
{}

//////////////////////////////////////////////////////////////////////////
void ISLAlgorithm::initialize()
{
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
void ISLAlgorithm::update(std::shared_ptr<Agent> _agent)
{
	//	ogni agente guardia sceglie il proprio tasso di esplorazione:
	double l_explorationRate = 0.;

	//	ogni agente guardia calcola la miglior azione passata: (2 step di memoria)

	//	ogni agente guardia estrae se sperimentare nuove azioni o no
	bool l_agentHasToExperiments = agentHasToExperiments(l_explorationRate);
	if(l_agentHasToExperiments)
		this->selectRandomFeasibleAction(_agent);
	else
		this->selectBestMemoryAction(_agent);

	//	ogni agente guardia muove verso la nuova posizione
	_agent->moveToNextPosition();

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

	//	ogni agente guardia identifica le nuove azioni feasible:

	return;
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

	int l_value = getRandomValue( int( l_feasible.size() ) );

	_agent->setNextPosition(l_feasible[l_value]);
	return;
}

//////////////////////////////////////////////////////////////////////////
void ISLAlgorithm::selectBestMemoryAction(std::shared_ptr<Agent> _agent)
{
	std::vector< MemoryAgentPosition > l_memory = _agent->getMemory();
	return;
}
