///////////////////////////////////////////////////////////
//  Coverage.h
//  Created on:      13-may-2013 10.14.52
//  Original author: s.nardi
///////////////////////////////////////////////////////////
#ifndef COVERAGE_H
#define COVERAGE_H
#pragma once

//	DTMManager
#include "CoverageExport.h"

#include <memory>
#include <set>
#include <vector>

namespace IDS 
{
	namespace BaseGeometry
	{
		class Point2D;
	}
}

namespace Robotics
{
	namespace GameTheory 
	{
		class DiscretizedArea;
		class Agent;
		class LearningAlgorithm;
		class Area;
		class AgentPosition;
		class Square;

		class COVERAGE_API CoverageAlgorithm
		{
		protected:
			/// The set of all the agents in the area.
			std::set< std::shared_ptr<Agent> > m_agent;

			/// The space where agents can move.
			std::shared_ptr<DiscretizedArea> m_space;

			/// The learning algorithm used by agents to change their status.
			std::shared_ptr<LearningAlgorithm> m_learning;

		protected:
			/************************************************************************/
			/* \brief Compute initial position and payoff
			*
			*/
			/************************************************************************/
			void Initialize();

		public:

			/************************************************************************/
			/* \brief	Coverage Algorithm Constructor
			*
			*	@param 
			*/
			/************************************************************************/
			CoverageAlgorithm(
				const std::set< std::shared_ptr<Agent> >& _agent, 
				std::shared_ptr<Area> _space);

			/************************************************************************/
			/* \brief	Set the rule that agents follow to move to next location
			*
			*	@param _learning [in] The reference to the learning algorithm
			*/
			/************************************************************************/
			void setLearningAlgorithm(std::shared_ptr<LearningAlgorithm> _learning);

			/************************************************************************/
			/* \brief Update agent position and camera for a number of steps (step in time)
			*
			*	@param nStep [in] The number of step.
			*
			*	@throw 
			*/
			/************************************************************************/
			void update(int nStep);

			/************************************************************************/
			/*	\brief Initialize position of agent uniformly at random in the area.
			*
			*/
			/************************************************************************/
			void randomInitializeAllAgent();

			void randomInitializeGuards();

			void randomInitializeNeutrals();

			void randomInitializeThief();
			void setPositionOfThief(AgentPosition const& pos);

#pragma region Test
		
		public:
			std::vector< std::shared_ptr<Square> > getSquares() const;

			bool areaContains(const IDS::BaseGeometry::Point2D & _thiefStartingPt) const;

#pragma endregion

			std::shared_ptr<Square> findSquare(IDS::BaseGeometry::Point2D const& point) const;

		protected:
			/// Wake Up agent if the security is too low.
			void wakeUpAgentIfSecurityIsLow();
		};
	}
}
#endif