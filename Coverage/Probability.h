#include "CoverageExport.h"

namespace Robotics 
{
	namespace GameTheory 
	{
		/// Function to get value with a specified distribution.
		int COVERAGE_API getRandomValue();

		/// Return value between 0 and maxValue-1
		int COVERAGE_API getRandomValue(int maxValue);

		bool COVERAGE_API agentHasToExperiments(double _explorationRate);
	}
}
