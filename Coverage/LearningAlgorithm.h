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
		class Sink;
		class AgentActionIndex;
		class DiscretizedArea;

		class COVERAGE_API LearningAlgorithm
		{
		protected:
			int m_time;
			std::set< std::shared_ptr<Guard> > m_guards;

			std::shared_ptr<DiscretizedArea> m_space;

			double m_experimentalRate;

		public:
			LearningAlgorithm(std::shared_ptr<DiscretizedArea> _space) : m_time(0), m_guards(), m_experimentalRate(0.), m_space(_space) {}
			LearningAlgorithm(std::set< std::shared_ptr<Guard> > const& _guards, std::shared_ptr<DiscretizedArea> _space) : m_time(0), m_guards(_guards), m_experimentalRate(0.), m_space(_space) {}

			virtual void initialize();

			virtual void computeNextPosition();

			virtual bool forwardOneStep(std::shared_ptr<Guard> _agent);
			virtual bool forwardOneStep();

			inline void setGuards(std::set< std::shared_ptr<Guard> > _guards) {m_guards = _guards;}

			virtual void updateTime() {++m_time;}
			void resetTime() {m_time = 0;}
			virtual void resetCounter();
			virtual void resetValue();

			virtual void monitoringThieves(std::set<std::shared_ptr<Thief>> const& );
			virtual void monitoringSinks(std::set<std::shared_ptr<Sink>> const& );

			virtual void getGuardsPosition(std::vector<AgentPosition> & _pos);
			virtual void getGuardsSquare(std::vector< std::pair<std::shared_ptr<Square>, AgentActionIndex> > & _pos);
			virtual void getGuardsCoverage( std::vector< std::vector<IDS::BaseGeometry::Point2D> > & _areas);
			virtual void setExperimentalRate(double _epsilon) {m_experimentalRate = _epsilon;}

			int getTime() const {return m_time;}
			virtual double getPotentialValue();
			virtual double getBenefitValue();
			virtual std::string getExplorationRateStr();
			virtual double getExplorationRate();
			virtual int getGlobalTrajectoryCoverage();

			virtual double computeExplorationRate(std::shared_ptr<Guard> _agent = nullptr);

			virtual int getNumberOfSteps(double _stopRate);

			//////////////////////////////////////////////////////////////////////////

			virtual void update(std::shared_ptr<Guard> _agent) = 0;

			virtual void communicate(std::shared_ptr<Guard> _agent);

			virtual void compute(std::shared_ptr<Guard> _agent);

			virtual void updateCounterOfVisibleSquare( );
			virtual void updateCounterOfVisibleSquare( std::shared_ptr<Guard> _agent );
		};

		typedef std::shared_ptr<LearningAlgorithm> LearningAlgorithmPtr;

	}
}
#endif