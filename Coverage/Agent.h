///////////////////////////////////////////////////////////
//  Agent.h
//  Created on:      13-may-2013 10.14.52
//  Original author: s.nardi
///////////////////////////////////////////////////////////
#ifndef AGENT_H
#define AGENT_H
#pragma once

//	Coverage
#include "CoverageExport.h"

// GeometricKernel
#include "BaseGeometry/Point2D.h"

//	IDSBaseMath
#include "IDSMath.h"

#include <memory>
#include <set>

namespace IDS 
{
	namespace BaseGeometry
	{
		class Shape2D;
	}
}
namespace Robotics 
{
	namespace GameTheory 
	{
		class DiscretizedArea;
		struct AreaCoordinate;
		class Area;
		class Agent;
		class Square;

		//////////////////////////////////////////////////////////////////////////
		class COVERAGE_API CameraPosition
		{
		protected:
			/// Major radius.
			double m_farRadius;
			/// Minor Radius.
			double m_nearRadius;
			/// Orientation.
			double m_orientation; // [0, 2*Pi)
			/// Angle of view.
			double m_angle;

		public:
			CameraPosition(double _farRadius = 0., double _nearRadius = 0., double _orientation = 0., double _angle = IDSMath::TwoPi) 
				: m_farRadius(_farRadius), m_nearRadius(_nearRadius), m_orientation(_orientation), m_angle(_angle) {}

			std::vector<AreaCoordinate> getCoverage(AreaCoordinate _center, std::shared_ptr<DiscretizedArea> _area) const;

			double getFarRadius() const {return m_farRadius;}

			IDS::BaseGeometry::Shape2D getVisibleArea(IDS::BaseGeometry::Point2D const& point) const;

			double computeCosts() const {return 0.;}
		};

		//////////////////////////////////////////////////////////////////////////
		class COVERAGE_API AgentPosition
		{
		protected:
			/// The position of the agent.
			IDS::BaseGeometry::Point2D m_point;

			/// The camera orientation.
			CameraPosition m_camera;

		public:
			AgentPosition() {};
		
			AgentPosition(IDS::BaseGeometry::Point2D const& point) : m_point(point), m_camera() {}

			AgentPosition(IDS::BaseGeometry::Point2D const& point, CameraPosition _camera) : m_point(point), m_camera(_camera) {}

			/// Update the counter of the lattice visible from that position
			void updateCounter(std::shared_ptr<DiscretizedArea> area);

			/// Get Point2D
			IDS::BaseGeometry::Point2D getPoint2D() const {return m_point;}

			/// True if other is in communication with this position
			bool communicable(std::shared_ptr<Agent> _other) const;

			/// Compute all the square visible by that position and that camera
			bool visible(std::shared_ptr<Square> _area) const;

			/// Compute Camera Costs
			double computeCosts() const;

			std::vector<AreaCoordinate> getCoverage(std::shared_ptr<DiscretizedArea> _space ) const;

			IDS::BaseGeometry::Shape2D getVisibleArea() const;

			friend class COVERAGE_API Agent;
			friend class COVERAGE_API Guard;
			friend class COVERAGE_API Thief;
		};

		//////////////////////////////////////////////////////////////////////////
		struct MemoryAgentPosition
		{
			AgentPosition m_action;
			double m_payoff;

			MemoryAgentPosition(AgentPosition _action, double _payoff) : m_action(_action), m_payoff(_payoff) {}
		};

		//////////////////////////////////////////////////////////////////////////
		class COVERAGE_API Agent : public std::enable_shared_from_this<Agent>
		{
		protected:
			/// Agent Identifier
			int m_id;

			/// Agent Position in space
			AgentPosition m_position;
			
			/// Current payoff received in the current position
			double m_currentPayoff;

			/// Agent Position in space
			AgentPosition m_nextPosition;

			mutable enum Status 
			{
				ACTIVE,
				DISABLE,
				SLEEP,
				WAKEUP
			} m_status;

			std::vector<MemoryAgentPosition> m_memory;
			int m_memorySpace;

		public:

			Agent(int _id, AgentPosition _position, int _memorySpace = 5 )
				: m_id(_id), m_position(_position), m_currentPayoff(0.), m_nextPosition(), m_memorySpace(_memorySpace) {}

			~Agent() {}

			/// Set the position of the agent.
			void setPosition(AgentPosition const& pos);
			void setNextPosition(AgentPosition const& pos);
			void setNextPosition();

			/// Get the position of the agent.
			inline AgentPosition getPosition() const {return m_position;}

			std::vector<MemoryAgentPosition> getMemory() {return m_memory;}

			/// True if the Agent is active, false otherwise.
			bool isActive();

			/// True if the agent is next to be put on Stand-By, false otherwise.
			bool isOutOfInterest( std::shared_ptr<DiscretizedArea> space) const;

			/// Set the agent on standBy status. 
			void sleep();

			/// Set the agent on standBy status. 
			void wakeUp();

			virtual bool isThief() const {return false;}
			
			virtual bool isGuard() const {return false;}

			std::shared_ptr<Thief> toThief();

			std::shared_ptr<Guard> toGuard();

			void moveToNextPosition();

			void setNextPosition(std::vector< MemoryAgentPosition > const& memory);

			virtual std::vector<AgentPosition> getFeasibleActions( std::shared_ptr<DiscretizedArea> _space ) const;

			IDS::BaseGeometry::Shape2D getVisibleArea() const;

		public:
#pragma region ISLAlgorithm
			/// The set of agents can communicate with this agent
			virtual std::set<std::shared_ptr<Agent> > getCommunicableAgents(std::set<std::shared_ptr<Agent> > const& _guards) const;
			/// The set of square can be controlled by this agent
			virtual std::set<std::shared_ptr<Square> > getVisibleSquares( std::shared_ptr<DiscretizedArea> _space ) const;

			bool equals(std::shared_ptr<Agent>) const;

			void receiveMessage( std::set<std::shared_ptr<Square> > const& _visible);

			/// Compute cost of sensor.
			double computeCosts() const;
#pragma endregion

			void setCurrentPayoff(double _benefit);

		protected:
			Status getStatus() const;
			void setStatus(Status stat);
		};

		typedef std::shared_ptr<Agent> AgentPtr;
	}
}
#endif