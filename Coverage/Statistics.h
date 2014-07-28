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
		class COVERAGE_API Statistics
		{
		public:

			std::vector<double> m_times;
			std::vector<int> m_squares;
			std::vector<double> m_potValues;
			std::vector<double> m_benefitValues;
			std::vector<double> m_maxThiefValue;
			std::vector<double> m_performanceIndex;

			void addValues(int time, int square, double potValue, double benefitValue, double maxThiefValue);

			void reset();

			double getBoxPlotValue();
			double getBoxPlotValueNewIndex();
			
			void printPerformanceIndex(std::string const& name, bool printOnFile = true);
			void printBenefit(std::string const& _filename, bool _printOnFile = true);
			void printPotential(std::string const& _filename, bool _printOnFile = true);


		};
	}
}
#endif