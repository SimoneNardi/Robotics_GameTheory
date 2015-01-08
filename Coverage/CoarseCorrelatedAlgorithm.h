///////////////////////////////////////////////////////////
//  CoarseCorrelatedtAlgorithm.h
//  Created on:      1-dec-2014 10.14.52
//  Original author: s.nardi
///////////////////////////////////////////////////////////
#ifndef COARSE_CORRELATED_ALGORITHM_H
#define COARSE_CORRELATED_ALGORITHM_H
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
		class Sink;

		/**
		*	\brief	Learning Efficient Correlated Equilibria
		*	see P.Borowski, J.R.Marden, J.S.Shamma 
		*	in Proceedings of the IEEE Conference on Decision and Control, 2014.
		*/
		class COVERAGE_API CoarseCorrelatedAlgorithm : public LearningAlgorithm
		{
		public:
			CoarseCorrelatedAlgorithm(std::shared_ptr<DiscretizedArea> _space);
		protected:
			virtual void update(std::shared_ptr<Guard> _agent);
		};
	}
}
#endif