#include "LearningAlgorithm.h"

#include "Guard.h"
#include "CoverageUtility.h"

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