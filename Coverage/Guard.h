///////////////////////////////////////////////////////////
//  Guard.h
//  Created on:      13-may-2013 10.14.52
//  Original author: s.nardi
///////////////////////////////////////////////////////////
#ifndef GUARD_H
#define GUARD_H
#pragma once

#include "CoverageExport.h"
#include "Agent.h"

extern const double MAXIMUM_PERIOD;
const double MAXIMUM_BATTERY = 100.;
const double MINIMUM_BATTERY = 60.;
const double LOSTBATTERY_PERSTEP = .01;

namespace Robotics 
{
	namespace GameTheory 
	{
		//////////////////////////////////////////////////////////////////////////
		class COVERAGE_API GuardTrajectoryPosition
		{
		public:
			AgentPosition m_position;
			int m_index;
			
			GuardTrajectoryPosition(AgentPosition const& _position, int _index = 0) 
				: m_position(_position), m_index(_index) {}

			bool operator==(GuardTrajectoryPosition const& other) const;
			bool operator!=(GuardTrajectoryPosition const& other) const;
		};

		enum Mood
		{
			Content,	//	Content
			Hopeful,
			Watchful,
			Discontent	//	Discontent
		};

		//////////////////////////////////////////////////////////////////////////
		class COVERAGE_API GuardTrajectory
		{
			std::vector<GuardTrajectoryPosition> m_elems;

		public:
			std::vector<AgentPosition> getTrajectory() const;

			AgentPosition getPosition(int i) const {return m_elems.at(i).m_position;}

			GuardTrajectory() : m_elems() {}
			GuardTrajectory(AgentPosition pos) : m_elems( std::vector<GuardTrajectoryPosition>(1,pos) ) {}
			GuardTrajectory(std::vector<GuardTrajectoryPosition> _traj) : m_elems( _traj ) {}
			GuardTrajectory(GuardTrajectory const& _other) : m_elems( _other.m_elems ) {}
			GuardTrajectory& operator=(GuardTrajectory const& _traj) 
			{
				m_elems = _traj.m_elems;
				return *this;
			}

			/// Check if the two trajectory are equals until now.
			bool operator==(GuardTrajectory const& other) const;
			/// Check if the two trajectory are equals until now.
			bool operator!=(GuardTrajectory const& other) const;

			/// Check if the trajectory is length maxLength - 1, if it is necessary one last movement
			bool isLast(int _maxLength) {return int(m_elems.size()) < _maxLength-1;}

			/// Get the last position of the trajectory
			AgentPosition getLastPosition() const {return m_elems.back().m_position;}

			inline int size() const { return int(m_elems.size());}

			inline void clear() {m_elems.clear();}

			inline void add(AgentPosition pos) {m_elems.push_back( GuardTrajectoryPosition(pos, m_elems.size()) );};

			bool contains(AgentPosition _nextPos);

		};

		//////////////////////////////////////////////////////////////////////////
		struct MemoryGuardTrajectory
		{
			GuardTrajectory m_memTrajectory;
			double m_payoff;
			Mood m_mood;
			
			/// Constructor for reference trajectory
			MemoryGuardTrajectory() : m_memTrajectory(), m_payoff(0.), m_mood(Content) {}

			MemoryGuardTrajectory(GuardTrajectory const& _action, double _payoff, Mood _state = Content)
				: m_memTrajectory(_action), m_payoff(_payoff), m_mood(_state) {}
			
			/// Check if the two trajectory are equals until now.
			bool equals(MemoryGuardTrajectory const& _other) const;
			/// Check if the two trajectory are equals until now.
			bool equals(GuardTrajectory const& _trajectory, double _payoff) const;
			
			/// Get the last position of the trajectory
			AgentPosition getLastPosition() const {return m_memTrajectory.getLastPosition();}
		};

		class MemoryGuardTrajectories
		{
		public:
			std::vector<MemoryGuardTrajectory> m_elems;
			int m_best;
			int m_worst;
			int m_maxSize;

			MemoryGuardTrajectories(int _maxSize = 0): m_elems(), m_best(-1), m_worst(-1), m_maxSize(_maxSize) {}

			void computeBestWorstTrajectories();

			double getDeltaMemoryBenefit();

			inline bool bestTrajectory(MemoryGuardTrajectory & _best) 
			{
				if(m_elems.size() == 0)
					return false;

				if(m_best == -1) 
					computeBestWorstTrajectories();

				_best = m_elems[m_best];
				return true;
			}

			void add(MemoryGuardTrajectory const& _elem);

			AgentPosition getNextPosition(int _indexBest, int _indexNext);

			void reset();
		};

		///////////////////////////////////////////////////////////
		class COVERAGE_API Guard : public Agent
		{
			int m_teamID;

			/// Agent Position in space
			GuardTrajectory m_currentTrajectory;
			
			/// Current payoff received in the current position
			double m_currentTrajectoryPayoff;
			
			/// Current payoff in current position
			double m_currentPayoff;

			Mood m_currentMood;

			MemoryGuardTrajectories m_memory;

			// if -1 not exploring, else the index of the memory
			int m_exploring;

			int m_maxTrajectoryLength;

			mutable std::set< std::shared_ptr<Square> > m_coverage;
			std::set< std::shared_ptr<Square> > m_oldCoverage;

			// Current level of battery
			double m_current_battery;
			// Minimum level of battery
			double m_minimum_battery;
			// Maximum level of battery
			double m_maximum_battery;

			bool m_change_period;

		public:
			Guard( int _teamID, int _id, AgentPosition _position, int _trajectoryLength = 4, int _memorySpace = 2 );

			~Guard();

			inline virtual bool isGuard() const {return true;}

			std::vector<AgentPosition> getFeasibleActions( std::shared_ptr<DiscretizedArea> _space ) const;

			/// Trajectory Section

			/// Get memory of the guard.
			inline MemoryGuardTrajectories getMemory() {return m_memory;}

			/// Get the index of the best trajectory
			int getBestTrajectoryIndex(bool _best = true);

			/// Remove Best Position if necessary
			void removeBestPositionFromFeasible(std::vector<AgentPosition> & _feasible);

			/// CONTROLLARE
			//void setBestTrajectory(MemoryGuardTrajectories const& memory, std::vector<AgentPositionTrajectory> const& _feasibletrajectory, bool _best = true);

			/// CONTROLLARE
			//void selectRandomFeasiblePosition(std::shared_ptr<DiscretizedArea> _space);

			/// CONTROLLARE
			//void followBestTrajectory();

			/// Position Section

			/// The set of agents can communicate with this agent
			std::set<std::shared_ptr<Guard> > getCommunicableAgents(std::set< std::shared_ptr<Guard> > const& _guards) const;

			/// Receive the square visible from an other guard.
			void receiveMessage( std::set<std::shared_ptr<Square> > const& _visible);

			void updateMemoryTrajectories();

			/// Compute cost of sensor.
			double computeCurrentCosts() const;

			bool isRunning() const;

			void startExperiment(double _explorationRate);

			void followBestTrajectory(double _explorationRate, bool best = true);

			void selectNextAction(std::shared_ptr<DiscretizedArea> _space);

			virtual void moveToNextPosition();

			void reset(double _explorationRate);

			AgentPosition selectNextFeasiblePosition(std::shared_ptr<DiscretizedArea> _space);

			void setCurrentPayoff(double _benefit);
			/// Get Current Payoff
			inline double getCurrentPayoff() const {return m_currentPayoff;}

			void setCurrentMood(Mood _state);
			/// Get Current Payoff
			inline Mood getCurrentMood() const {return m_currentMood;}
			Mood computeMood(double _explorationRate);

			int actualActionIndex();
			int totalActions();

			std::set< std::shared_ptr<Square> > getTrajectoryCoverage() const;
			void collectVisitedSquare(std::set<std::shared_ptr<Square>>const& _squares);
			std::set<std::shared_ptr<Square> > getVisibleSquares( std::shared_ptr<DiscretizedArea> _space );
			int getTrajectoryLength() const {return m_maxTrajectoryLength;}
			double getBatteryValue() const {return m_current_battery;}

			void updateBattery(double value);
			void updatePeriod(int value);
			int computePeriod();
			double computeBatteryCosts(std::shared_ptr<DiscretizedArea> _space);

			void resetMemory();

		protected:
			AgentPosition selectNextFeasiblePositionWithoutConstraint(std::shared_ptr<DiscretizedArea> _space);
			AgentPosition selectNextFeasiblePositionWithoutConstraint(std::shared_ptr<DiscretizedArea> _space, std::set<int> & _alreadyTested);
		};

		typedef std::shared_ptr<Guard> GuardPtr;
	}
}
#endif