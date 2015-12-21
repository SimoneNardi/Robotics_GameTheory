///////////////////////////////////////////////////////////
//  Monitor.h
//  Created on:      07-may-2014 10.14.52
//  Original author: s.nardi
///////////////////////////////////////////////////////////
#ifndef STATISTICS_H
#define STATISTICS_H
#pragma once

//	DTMManager
#include "CoverageExport.h"

#include <vector>

namespace Robotics
{
	namespace GameTheory 
	{
		COVERAGE_API void computeSteadyValue(std::vector<double> & _result_container, double& _steadyValue , int &_steadyIndex);
		
		class COVERAGE_API Statistics
		{
		public:

			std::vector<double> m_times;
			std::vector<int> m_squares;
			std::vector<double> m_potValues;
			std::vector<double> m_benefitValues;
			std::vector<double> m_maxBenefitValue;
			std::vector<double> m_NonCooperativeSteadyValue;
			std::vector<double> m_potentialIndex;
			std::vector<double> m_benefitIndex;
			std::vector<double> m_explorationRate;
			std::vector<double> m_batteryValue;

			void addValues(
				int time, 
				int square, 
				double potValue, 
				double benefitValue, 
				double maxBenefitValue, 
				double NonCooperativeSteadyValue, 
				double _explorationRate,
				double _batteryValue);

			void reset();

			double getBenefitIndexMediumValue();
			double getBenefitIndexSteadyValue();

			double getPotentialIndexMediumValue();
			double getPotentialIndexSteadyValue();
			
			void printPotentialIndex(std::string const& name, bool printOnFile = true);
			void printBenefit(std::string const& _filename, bool _printOnFile = true);
			void printPotential(std::string const& _filename, bool _printOnFile = true);
			void printBenefitIndex(std::string const& _filename, bool _printOnFile = true);
			void printPotentialIndexVersusExplorationRate(std::string const& _filename, bool _printOnFile = true);
			void printExplorationRate(std::string const& _filename, bool _printOnFile = true);
		};
	}
}
#endif