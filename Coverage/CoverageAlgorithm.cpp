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

using namespace Robotics;
using namespace Robotics::GameTheory;

//////////////////////////////////////////////////////////////////////////
Robotics::GameTheory::CoverageAlgorithm::CoverageAlgorithm(
	const std::set< std::shared_ptr<Agent> >& _agent, 
	std::shared_ptr<Area> _space) : m_agent(_agent), m_space(nullptr)
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
#else
	for(int i = 0; i < nStep; ++i)
	{
		for(std::set< AgentPtr >::iterator it = m_agent.begin(); it != m_agent.end(); ++it)
		{
			AgentPtr l_agent = *it;

			if(l_agent->isActive())
			{
				if(m_learning)
					m_learning->forwardOneStep(l_agent);

				if(l_agent->isGuard())
					if( l_agent->isOutOfInterest(m_space) )
						l_agent->sleep();
			}
		}

		this->wakeUpAgentIfSecurityIsLow();
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
	this->randomInitializeAllAgent();

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



