#include "ParetoEfficientAlgorithm.h"
#include "DiscretizedArea.h"
#include "Guard.h"
#include "Thief.h"
#include "Probability.h"
#include "CoverageUtility.h"

#include <sstream>
#include <string>
#include <set>

#include "windows.h"

using namespace std;
using namespace Robotics;
using namespace Robotics::GameTheory;
using namespace IDS::BaseGeometry;

//////////////////////////////////////////////////////////////////////////
ParetoEfficientAlgorithm::ParetoEfficientAlgorithm(std::shared_ptr<DiscretizedArea> _space) 
	: LearningAlgorithm(_space)
{}

//////////////////////////////////////////////////////////////////////////
void ParetoEfficientAlgorithm::update(std::shared_ptr<Guard> _agent)
{
	if(!_agent->isRunning())
		//	Inizia una nuova traiettoria (sperimentale o no!)
	{
		//	ogni agente guardia sceglie il proprio tasso di esplorazione:
		double l_explorationRate = this->computeExplorationRate();

		double l_maxValue = m_guards.size();

		switch(_agent->getCurrentMood())
		{
		case Content:
			{
				double l_powExplorationRate = pow( l_explorationRate, l_maxValue );

				//	ogni agente guardia estrae se sperimentare nuove azioni o no
				bool l_agentHasToExperiments = agentHasToExperiments(l_powExplorationRate);
				if(l_agentHasToExperiments)
				{
					_agent->startExperiment(l_explorationRate);
				}
				else
				{
					_agent->followBestTrajectory(l_explorationRate);
				}
				break;
			}
		case Discontent:
		default:
			{
				_agent->startExperiment(l_explorationRate);
				break;
			}
		}
	}

	_agent->selectNextAction(m_space);

	//	ogni agente guardia muove verso la nuova posizione
	_agent->moveToNextPosition();

	return;
}