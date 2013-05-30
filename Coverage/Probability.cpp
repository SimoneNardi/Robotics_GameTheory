#include "Probability.h"

#include <random>
#include <ctime>

//////////////////////////////////////////////////////////////////////////
int Robotics::GameTheory::getRandomValue(int maxValue)
{
	srand( (unsigned int) time(NULL) );

	return rand() % maxValue;
}

//////////////////////////////////////////////////////////////////////////
int Robotics::GameTheory::getRandomValue()
{
	srand( (unsigned int) time(NULL) );

	double val = (double)rand() / RAND_MAX;

	int random;
	if (val < 0.05)       //  5%
		random = 0;
	else if (val < 0.25)  //  5% + 20%
		random = 1;
	else if (val < 0.55)  //  5% + 20% + 30%
		random = 2;
	else
		random = 3;
	return random;
}

//////////////////////////////////////////////////////////////////////////
bool Robotics::GameTheory::agentHasToExperiments(double _explorationRate)
{
	// srand( (unsigned int) time(NULL) );

	double val = (double)rand() / RAND_MAX;

	if (val < _explorationRate)
		return true;
	else
		return false;
}
