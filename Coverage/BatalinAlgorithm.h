#if 0
///////////////////////////////////////////////////////////
//  BatalinAlgorithm.h
//  Created on:      28-Jul-2014 10.14.52
//  Original author: s.nardi
///////////////////////////////////////////////////////////
#ifndef BATALIN_ALGORITHM_H
#define BATALIN_ALGORITHM_H
#pragma once

//	Coverage
#include "CoverageExport.h"
#include "LearningAlgorithm.h"

#include <memory>
#include <set>

namespace Robotics 
{
	namespace GameTheory 
	{
		class Agent;
		class DiscretizedArea;
		class Thief;

		/**
		*	\brief	Batalin: "Coverage, Exploration and Deployment by a Mobile Robot and Communication Network"
		*/
		class COVERAGE_API BatalinAlgorithm : public LearningAlgorithm
		{
		public:
			BatalinAlgorithm(std::shared_ptr<DiscretizedArea> _space);
		protected:
			virtual void update(std::shared_ptr<Guard> _agent);
		};
	}
}
#endif
#endif