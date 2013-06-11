#include "CoverageAlgorithm.h"
#include "CoverageUtility.h"
#include "LearningAlgorithm.h"
#include "ISLAlgorithm.h"
#include "Probability.h"
#include "DiscretizedArea.h"
#include "Agent.h"
#include "Guard.h"
#include "Neutral.h"
#include "Thief.h"

#include "IDSGraphicsC.h"

using namespace Robotics;
using namespace Robotics::GameTheory;

//////////////////////////////////////////////////////////////////////////
Robotics::GameTheory::CoverageAlgorithm::CoverageAlgorithm(
	const std::set< std::shared_ptr<Agent> >& _agent, 
	std::shared_ptr<Area> _space) : m_agent(_agent), m_space(nullptr), m_potential(true)
{
	m_space = _space->discretize();

	for(int i = 0; i < MAX_NUMBER_NEUTRAL_AGENT; ++i)
	{
		m_agent.insert( std::make_shared<Neutral>( m_agent.size(), _space->randomPosition() ) );
	}

	std::shared_ptr<ISLAlgorithm> l_learning = std::make_shared<ISLAlgorithm>(m_space, 2);
	this->setLearningAlgorithm(l_learning);
}

//////////////////////////////////////////////////////////////////////////
void Robotics::GameTheory::CoverageAlgorithm::setLearningAlgorithm(std::shared_ptr<LearningAlgorithm> _learning)
{
	m_learning = _learning;

	// create Guard set:
	std::set< AgentPtr > l_guards;
	for(std::set< AgentPtr >::iterator it = m_agent.begin(); it != m_agent.end(); ++it)
	{
		AgentPtr l_agent = *it;

		if(l_agent->isGuard())
			l_guards.insert(l_agent);
	}

	m_learning->setGuards(l_guards);
}

//////////////////////////////////////////////////////////////////////////
void Robotics::GameTheory::CoverageAlgorithm::update(int nStep)
{
#if 0
	m_space->setRandomSquareValue();
	this->wakeUpAgentIfSecurityIsLow();
#else
	static int count = 0;
	for(int i = 0; i < nStep; ++i)
	{
		if(m_learning)
		{
			m_learning->updateTime();
			m_learning->monitoringThieves(m_agent);
			m_learning->forwardOneStep();
		}

		++count;
		for(std::set< AgentPtr >::iterator it = m_agent.begin(); it != m_agent.end(); ++it)
		{
			AgentPtr l_agent = *it;
			if(l_agent->isActive())
			{
				if(!l_agent->isGuard())
				{
					//if(l_agent->isThief())
					if(count%10)
						continue;

					if(m_learning)
					{
						m_learning->selectRandomFeasibleAction(l_agent);
						l_agent->moveToNextPosition();
					}
				}
				else if( l_agent->isOutOfInterest(m_space) )
					l_agent->sleep();
			}
		}

		this->wakeUpAgentIfSecurityIsLow();

		double l_potValue = m_learning->getPotentialValue();
		m_potValues.push_back(l_potValue);
		int l_numsquare = this->numberOfSquaresCoveredByGuards();
		m_squares.push_back(l_numsquare);
		int l_time = this->getTime();
		m_times.push_back(l_time);
	}
	return;
#endif
}

//////////////////////////////////////////////////////////////////////////
void Robotics::GameTheory::CoverageAlgorithm::wakeUpAgentIfSecurityIsLow()
{
	for(std::set< AgentPtr >::iterator it = m_agent.begin(); it != m_agent.end(); ++it)
	{
		AgentPtr l_agent = *it;
		if(!l_agent->isActive())
		{
			l_agent->wakeUp();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::Initialize()
{
	//this->randomInitializeAllAgent();

	m_learning->initialize();
}

//////////////////////////////////////////////////////////////////////////
void Robotics::GameTheory::CoverageAlgorithm::randomInitializeAllAgent()
{
	this->randomInitializeGuards();
	this->randomInitializeNeutrals();
	this->randomInitializeThief();
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::randomInitializeGuards()
{
	/// Put agent in the space
	for(std::set< std::shared_ptr<Agent> >::iterator it = m_agent.begin(); it != m_agent.end(); ++it)
	{
		AgentPtr l_agent = *it;
		if(!l_agent->isGuard())
			continue;

		// compute position of agent and camera:
		AgentPosition pos = m_space->getRandomPosition();
		// set position of the agent:
		l_agent->setPosition(pos);
	}
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::randomInitializeNeutrals()
{
	/// Put agent in the space
	for(std::set< std::shared_ptr<Agent> >::iterator it = m_agent.begin(); it != m_agent.end(); ++it)
	{
		AgentPtr l_agent = *it;
		if( l_agent->isThief() || l_agent->isGuard() )
			continue;

		// compute position of agent and camera:
		AgentPosition pos = m_space->getRandomPosition();
		// set position of the agent:
		l_agent->setPosition(pos);
	}
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::randomInitializeThief()
{
	/// Put agent in the space
	for(std::set< std::shared_ptr<Agent> >::iterator it = m_agent.begin(); it != m_agent.end(); ++it)
	{
		AgentPtr l_agent = *it;
		if(!l_agent->isThief())
			continue;

		// compute position of agent and camera:
		AgentPosition pos = m_space->getRandomPosition();
		// set position of the agent:
		l_agent->setPosition(pos);
	}
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::setPositionOfThief(AgentPosition const& pos)
{
	bool found = false;
	/// Put agent in the space
	for(std::set< std::shared_ptr<Agent> >::iterator it = m_agent.begin(); it != m_agent.end(); ++it)
	{
		AgentPtr l_agent = *it;
		if(!l_agent->isThief())
			continue;

		found = true;

		// compute position of agent and camera:
		//AgentPosition pos = m_space->getRandomPosition();
		// set position of the agent:
		l_agent->setPosition(pos);
	}
	if(!found)
		m_agent.insert( std::make_shared<Thief>(m_agent.size(), pos) );
}

//////////////////////////////////////////////////////////////////////////
std::vector< std::shared_ptr<Square> > CoverageAlgorithm::getSquares() const
{
	return m_space->getSquares(); 
}

//////////////////////////////////////////////////////////////////////////
bool CoverageAlgorithm::areaContains(const IDS::BaseGeometry::Point2D & _thiefStartingPt) const
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
SquarePtr CoverageAlgorithm::findSquare(IDS::BaseGeometry::Point2D const& point) const
{
	return m_space->getSquare(point);
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::getGuardsPosition(std::vector<AgentPosition> & _pos)
{
	return m_learning->getGuardsPosition(_pos);
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::getGuardsCoverage( std::vector< std::vector<IDS::BaseGeometry::Point2D> > & _areas)
{
	return m_learning->getGuardsCoverage(_areas);
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::getGuardsSquare(std::vector<std::shared_ptr<Square>> & _pos)
{
	return m_learning->getGuardsSquare(_pos);
}

//////////////////////////////////////////////////////////////////////////
int CoverageAlgorithm::numberOfSquaresCoveredByGuards() const
{
	return m_space->numberOfSquaresCoveredByGuards();
}

//////////////////////////////////////////////////////////////////////////
int CoverageAlgorithm::getTime() const
{
	return m_learning->getTime();
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::printGraph(std::string const& _filename)
{
	bool _showpoints = true;
	bool _print = true; 
	std::string _title = "Tempo/Potential Function";
	std::string _yLabel = "Potential Function";
	if(!m_potential)
	{
		_title = "Tempo/Coverage";
		_yLabel = "Coverage";
	}
	//	Draw Graphics:
	int n_graph = 1;
	int flags= 0;
	if(_print)
		flags = IDSG_MULTITHREAD;
	std::vector<int> np;
	np.push_back( (int)m_times.size() );

	std::vector<double> ascissa;
	std::vector<double> ordinata;
	ascissa.insert(ascissa.end(), m_times.begin(), m_times.end());
	if(m_potential)
		ordinata.insert(ordinata.end(), m_potValues.begin(), m_potValues.end());
	else
		ordinata.insert(ordinata.end(), m_squares.begin(), m_squares.end());

	CARTESIAN_GRAPH_DP(&n_graph, &flags, &np[0], &ascissa[0], &ordinata[0],"");

	char buffer[1024];
	sprintf(buffer, "%s", _title.c_str());
	GRAPH_COMMAND(buffer);

	std::string cmd = "xaxis label '";
	cmd +=  std::string("Time");
	cmd += "'";
	GRAPH_COMMAND(const_cast<char*>(cmd.c_str()));
	cmd = "yaxis label '";
	cmd += _yLabel;
	cmd += "'";
	GRAPH_COMMAND(const_cast<char*>(cmd.c_str()));

	GRAPH_COMMAND("curve 0 color 0 0 1");
	GRAPH_COMMAND("curve 0 filled false");
	GRAPH_COMMAND("curve 0 fillcolor 0 0 1");
	if(m_potential)
		GRAPH_COMMAND("curve 0 label Potential");
	else
		GRAPH_COMMAND("curve 0 label Coverage");

	GRAPH_COMMAND("vdiv .9");
	GRAPH_COMMAND("show");

	if(!_print)
	{
		char buffer[1024];
		sprintf(buffer,"hardcopy ../Graph/%s.jpg 99", _filename.c_str());
		//system("pause");
		GRAPH_COMMAND(buffer);
		GRAPH_COMMAND("close");
	}
}

//////////////////////////////////////////////////////////////////////////
std::string CoverageAlgorithm::getExplorationRate()
{
	return m_learning->getExplorationRate();
}
