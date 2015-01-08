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
	: LearningAlgorithm(_space)
{}

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
					_agent->followBestTrajectory(l_explorationRate, true);
				}
				else
				{
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