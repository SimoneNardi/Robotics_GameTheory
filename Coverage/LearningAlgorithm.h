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

namespace Robotics 
{
	namespace GameTheory 
	{
		class Agent;
		class Thief;
		class AgentPosition;
		class Square;

		class COVERAGE_API LearningAlgorithm
		{
		protected:
			int m_time;
			std::set< std::shared_ptr<Agent> > m_guards;

		public:
			LearningAlgorithm() : m_time(0), m_guards() {}
			LearningAlgorithm(std::set< std::shared_ptr<Agent> > const& _guards) : m_time(0), m_guards(_guards) {}

			virtual void initialize() = 0;

			virtual void computeNextPosition() = 0;

			virtual void forwardOneStep(std::shared_ptr<Agent> _agent) = 0;
			virtual void forwardOneStep() = 0;

			inline void setGuards(std::set< std::shared_ptr<Agent> > _guards) {m_guards = _guards;}

			virtual void updateTime() {++m_time;}

			virtual void selectRandomFeasibleAction(std::shared_ptr<Agent> _agent) = 0;

			virtual void monitoringThieves(std::set<std::shared_ptr<Agent>> const& ) = 0;

			virtual void getGuardsPosition(std::vector<AgentPosition> & _pos) = 0;
			virtual void getGuardsSquare(std::vector<std::shared_ptr<Square>> & _pos) = 0;
			virtual void getGuardsCoverage( std::vector< std::vector<IDS::BaseGeometry::Point2D> > & _areas) = 0;

			int getTime() const {return m_time;}
			virtual double getPotentialValue() = 0;
			virtual std::string getExplorationRate() =0;
		};

		typedef std::shared_ptr<LearningAlgorithm> LearningAlgorithmPtr;

	}
}
#endif