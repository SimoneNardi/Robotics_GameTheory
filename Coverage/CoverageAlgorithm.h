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
#include "Statistics.h"

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
		COVERAGE_API void setLostBattery(double _lostBattery);

		class DiscretizedArea;
		class Agent;
		class Thief;
		class Sink;
		class LearningAlgorithm;
		class Area;
		class AgentPosition;
		class Square;
		class World;
		class AgentActionIndex;

		class COVERAGE_API CoverageAlgorithm
		{
		protected:
			std::shared_ptr<World> m_world;

			/// The learning algorithm used by agents to change their status.
			std::shared_ptr<LearningAlgorithm> m_learning;

			//int m_time;
		public:
			/// For Stats:
			Statistics m_stats;
			/// For Stats:
			int m_count;

		public:

			static std::shared_ptr<CoverageAlgorithm> createFromFile(
				std::string const & _filename, 
				int _type, 
				int _period);

			static std::shared_ptr<CoverageAlgorithm> createFromAreaFile(
				std::string const & _areaFile, 
				std::string const & _agentFile, 
				int _type, 
				int _period,
				double _epsilon = 0.2);

			/************************************************************************/
			/* \brief Compute initial position and payoff
			*
			*/
			/************************************************************************/
			void Initialize();

			/// Reset Data of algorithm
			void Reset();

			/************************************************************************/
			/* \brief	Coverage Algorithm Constructor
			*
			*	@param 
			*/
			/************************************************************************/
			CoverageAlgorithm(
				const std::set< std::shared_ptr<Agent> >& _agent, 
				std::shared_ptr<Area> _space, 
				int _type);

			//////////////////////////////////////////////////////////////////////////
			CoverageAlgorithm(
				const std::set< std::shared_ptr<Agent> >& _agent, 
				std::shared_ptr<DiscretizedArea> _space, 
				int _type);

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
			bool update(int nStep, int _monitorUpdateTime = 5, int _thiefJump = 1, bool _continuousUpdate = true);

			/************************************************************************/
			/*	\brief Initialize position of agent uniformly at random in the area.
			*
			*/
			/************************************************************************/
			void randomInitializeAllAgent();

			//void randomInitializeGuards();

			//void randomInitializeNeutrals();

			//void randomInitializeThief();

			void setPositionOfThief(AgentPosition const& pos, std::shared_ptr<Thief> _agent/* = nullptr*/);
			void getThievesPosition(std::vector<AgentPosition> & _pos);

#pragma region Test
		
		public:
			void printArea(const std::string & filename);

			std::vector< std::shared_ptr<Square> > getSquares() const;

			bool areaContains(const IDS::BaseGeometry::Point2D & _thiefStartingPt) const;

			void getGuardsPosition(std::vector<AgentPosition> & _pos);
			void getGuardsSquare(std::vector<std::pair<std::shared_ptr<Square>, AgentActionIndex>> & _pos);
			void getGuardsCoverage( std::vector< std::vector<IDS::BaseGeometry::Point2D> > & _areas);
			int numberOfSquaresCoveredByGuards() const;

			void getSinksPosition(std::vector<AgentPosition> & _pos);
			void getSinksSquare(std::vector<std::pair<std::shared_ptr<Square>,int>> & _pos);
			void getSinksCoverage( std::vector< std::vector<IDS::BaseGeometry::Point2D> > & _areas);
			void setPositionOfSink(AgentPosition const& pos, std::shared_ptr<Sink> _agent/* = nullptr*/);
			void removeAllSinks();

			//int getTime() const;
			void printPotential(std::string const& name, bool printOnFile = true);
			void printBenefit(std::string const& name, bool printOnFile = true);
			void printPotentialIndex(std::string const& name, bool printOnFile = true);
			void printBenefitIndex(std::string const& name, bool printOnFile = true);
			void printPotentialIndexVersusExplorationRate(std::string const& name, bool printOnFile = true);
			void printExplorationRate(std::string const& name, bool printOnFile = true);
			
			void printNewPerformanceIndex(std::string const& name, bool printOnFile = true);
			void printNewPerformanceIndexVersusExplorationRate(std::string const& name, bool printOnFile = true);


			
			std::string getBatteryValueStr();
			double getBatteryValue();
			std::string getExplorationRateStr();
			double getExplorationRate();

			int getNumberOfAgent();

			double getMaximumPotentialValue();
			virtual double getMaximumBenefitValue();
			virtual double getSteadyNonCoopertativeBenefitValue();

			double getPotentialIndexMediumValue();
			double getBenefitIndexMediumValue();

			void removeAllThieves();
			void updateMonitor();
			int getGlobalTrajectoryCoverage();

			void setExperimentalRate(double _epsilon);

			int getNumberOfSteps(double _stopRate);

#pragma endregion

			std::shared_ptr<Square> findSquare(IDS::BaseGeometry::Point2D const& point) const;

#pragma region CONFIGURATION

			/// print data for BoxPlot:
			double getTrajectoryPotentialIndex();
			double getTrajectoryBenefitIndex();
			double getTrajectoryCoverage();

			void printPhoto(std::string const& _outputFileName);

#pragma endregion

#pragma region VIDEO

			std::shared_ptr<World> getWorld() {return m_world;}

#pragma endregion


		protected:
			/// Wake Up agent if the security is too low.
			void wakeUpAgentIfSecurityIsLow();
		};
	}
}

#endif