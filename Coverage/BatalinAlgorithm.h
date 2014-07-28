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
			std::shared_ptr<DiscretizedArea> m_space;

		protected:

			void update(std::shared_ptr<Guard> _agent);
			void communicate(std::shared_ptr<Guard> _agent);
			void compute(std::shared_ptr<Guard> _agent);

			void computeCurrentUtilities(std::shared_ptr<Guard> _agent);

			void sendInformationToNeighbors(std::shared_ptr<Guard> _agent);

			//virtual void selectRandomFeasibleAction(std::shared_ptr<Guard> _agent);

			void selectBestMemoryAction(std::shared_ptr<Guard> _agent);

			bool forwardOneStep(std::shared_ptr<Guard> _agent);
			bool forwardOneStep();

			void updateCounterOfVisibleSquare( );
			void updateCounterOfVisibleSquare( std::shared_ptr<Guard> _agent );

			virtual void monitoringThieves(std::set< std::shared_ptr<Thief> > const& _agents);

			virtual void getGuardsPosition(std::vector<AgentPosition> & _pos);
			void getGuardsSquare(std::vector<std::pair<std::shared_ptr<Square>,int>> & _pos);
			void getGuardsCoverage( std::vector< std::vector<IDS::BaseGeometry::Point2D> > & _areas);
			double getPotentialValue();
			double getBenefitValue();
			
			std::string getExplorationRateStr();
			double getExplorationRate();
			double computeExplorationRate();
			int getGlobalTrajectoryCoverage();

		public:
			BatalinAlgorithm(std::shared_ptr<DiscretizedArea> _space);

			virtual void computeNextPosition();

			virtual void initialize();

			virtual void resetCounter();
			virtual void resetValue();
			
			//virtual void updateTime(); 
		};
	}
}
#endif