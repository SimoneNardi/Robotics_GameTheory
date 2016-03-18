#include "DISLAlgorithm.h"
#include "DiscretizedArea.h"
#include "Guard.h"
#include "Thief.h"
#include "Sink.h"
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
DISLAlgorithm::DISLAlgorithm(std::shared_ptr<DiscretizedArea> _space) 
	: LearningAlgorithm(_space)
{}

//////////////////////////////////////////////////////////////////////////
void DISLAlgorithm::update(std::shared_ptr<Guard> _agent)
{
	if(!_agent->isRunning())
		//	Inizia una nuova traiettoria (sperimentale o no!)
	{
		//	ogni agente guardia sceglie il proprio tasso di esplorazione:
		double l_explorationRate = this->computeExplorationRate();

		//	ogni agente guardia estrae se sperimentare nuove azioni o no
		bool l_agentHasToExperiments = agentHasToExperiments(l_explorationRate);

		if(l_agentHasToExperiments)
		{
			_agent->startExperiment(l_explorationRate);
		}
		else
		{
			_agent->followBestTrajectory(l_explorationRate);
		}
	}

	_agent->selectNextAction(m_space);

	//	ogni agente guardia muove verso la nuova posizione
	_agent->moveToNextPosition();

	return;
}