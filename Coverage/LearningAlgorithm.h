///////////////////////////////////////////////////////////
//  LearningAlgorithm.h
//  Created on:      13-may-2013 10.14.52
//  Original author: s.nardi
///////////////////////////////////////////////////////////
#ifndef LEARNING_ALGORITHM_H
#define LEARNING_ALGORITHM_H
#pragma once

//	Coverage
#include "CoverageExport.h"

#include "BaseGeometry/Point2D.h"

#include <memory>
#include <set>

//const int N_MAX = 100000000;
const int N_MAX = 100000;
//const int N_MAX = 500;

namespace Robotics 
{
	namespace GameTheory 
	{
		class Guard;
		class Thief;
		class AgentPosition;
		class Square;
		class AgentActionIndex;

		class COVERAGE_API LearningAlgorithm
		{
		protected:
			int m_time;
			std::set< std::shared_ptr<Guard> > m_guards;

			double m_experimentalRate;

		public:
			LearningAlgorithm() : m_time(0), m_guards(),m_experimentalRate(0.) {}
			LearningAlgorithm(std::set< std::shared_ptr<Guard> > const& _guards) : m_time(0), m_guards(_guards), m_experimentalRate(0.) {}

			virtual void initialize() = 0;

			virtual void computeNextPosition() = 0;

			virtual bool forwardOneStep(std::shared_ptr<Guard> _agent) = 0;
			virtual bool forwardOneStep() = 0;

			inline void setGuards(std::set< std::shared_ptr<Guard> > _guards) {m_guards = _guards;}

			virtual void updateTime() {++m_time;}
			void resetTime() {m_time = 0;}
			virtual void resetCounter() {};
			virtual void resetValue() {};

			//virtual void selectRandomFeasibleAction(std::shared_ptr<Guard> _agent) = 0;

			virtual void monitoringThieves(std::set<std::shared_ptr<Thief>> const& ) = 0;

			virtual void getGuardsPosition(std::vector<AgentPosition> & _pos) = 0;
			virtual void getGuardsSquare(std::vector< std::pair<std::shared_ptr<Square>, AgentActionIndex> > & _pos) = 0;
			virtual void getGuardsCoverage( std::vector< std::vector<IDS::BaseGeometry::Point2D> > & _areas) = 0;
			virtual void setExperimentalRate(double _epsilon) {m_experimentalRate = _epsilon;}

			int getTime() const {return m_time;}
			virtual double getPotentialValue() = 0;
			virtual double getBenefitValue() = 0;
			virtual std::string getExplorationRateStr() =0;
			virtual double getExplorationRate() =0;
			virtual int getGlobalTrajectoryCoverage()=0;

			virtual double computeExplorationRate(std::shared_ptr<Guard> _agent = nullptr);

			virtual int getNumberOfSteps(double _stopRate);

		};

		typedef std::shared_ptr<LearningAlgorithm> LearningAlgorithmPtr;

	}
}
#endif