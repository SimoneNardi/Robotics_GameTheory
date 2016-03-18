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
#include "BaseGeometry/Shape2D.h"

//	IDSBaseMath
#include "IDSMath.h"

#include <memory>
#include <set>

namespace Robotics 
{
	namespace GameTheory 
	{
		class DiscretizedArea;
		struct AreaCoordinate;
		class Area;
		class Agent;
		class Sink;
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

			bool operator==(CameraPosition const& other) const;
			bool operator!=(CameraPosition const& other) const;
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
			AgentPosition() {}; //costruttore
		
			AgentPosition(IDS::BaseGeometry::Point2D const& point) : m_point(point), m_camera() {}

			AgentPosition(IDS::BaseGeometry::Point2D const& point, CameraPosition _camera) : m_point(point), m_camera(_camera) {}

			//auto ProbabilityOfDetection(std::shared_ptr<DiscretizedArea> area, AreaCoordinate p_r, AreaCoordinate p_t);

			/// Update the counter of the lattice visible from that position
			void updateCounter(std::shared_ptr<DiscretizedArea> area);

			/// Get Point2D
			IDS::BaseGeometry::Point2D getPoint2D() const {return m_point;}

			/// True if other is in communication with this position
			bool communicable(std::shared_ptr<Agent> _other) const;

			/// is the center of the square visible in that position and that camera?
			bool visible(std::shared_ptr<Square> _area) const;

			/// Compute Camera Costs
			double computeCosts() const;

			std::vector<AreaCoordinate> getCoverage(std::shared_ptr<DiscretizedArea> _space ) const;

			IDS::BaseGeometry::Shape2D getVisibleArea() const;

			bool operator==(AgentPosition const& other) const;
			bool operator!=(AgentPosition const& other) const;

			friend class COVERAGE_API Agent;
			friend class COVERAGE_API Guard;
			friend class COVERAGE_API Thief;
			friend class COVERAGE_API Sink;
		};

		//////////////////////////////////////////////////////////////////////////
		class COVERAGE_API Agent : public std::enable_shared_from_this<Agent>
		{
		protected:
			/// Agent Identifier
			int m_id;

			AgentPosition m_currentPosition;

			AgentPosition m_nextPosition;

			mutable enum Status 
			{
				ACTIVE,
				DISABLE,
				STANDBY,
				WAKEUP
			} m_status;

		public:

			Agent(int _id, AgentPosition _position)
				: m_id(_id)
				, m_currentPosition(_position)
				, m_nextPosition()
					, m_status(ACTIVE)
			{}

			~Agent() {}

			/// Get the position of the agent.
			inline AgentPosition getCurrentPosition() const {return m_currentPosition;}

			/// Set Current Position
			void setCurrentPosition(AgentPosition const& _pos);

			/// Set Current Position
			void setNextPosition(AgentPosition const& _pos);

			/// True if the Agent is active, false otherwise.
			bool isActive();

			/// True if the agent is next to be put on Stand-By, false otherwise.
			bool isOutOfInterest( std::shared_ptr<DiscretizedArea> space) const;

			/// Set the agent on standBy status. 
			void sleep();

			/// Set the agent on wakeUp status. 
			void wakeUp();

			virtual bool isThief() const {return false;}
			
			virtual bool isGuard() const {return false;}

			virtual bool isSink() const {return false;}

			virtual bool isNeutral() const {return false;}

			std::shared_ptr<Thief> toThief();

			std::shared_ptr<Guard> toGuard();

			std::shared_ptr<Sink> toSink();

			virtual void moveToNextPosition();

			virtual std::vector<AgentPosition> getFeasibleActions( std::shared_ptr<DiscretizedArea> _space ) const;

			bool getRandomFeasibleAction(std::vector<AgentPosition> const& _feasible, AgentPosition & _pos) const;

			AgentPosition selectRandomFeasibleAction(std::shared_ptr<DiscretizedArea> _space);

			bool equals(std::shared_ptr<Agent>) const;

			inline IDS::BaseGeometry::Shape2D getVisibleArea() const {return m_currentPosition.getVisibleArea();}

		protected:
			Status getStatus() const;
			void setStatus(Status stat);
		};

		typedef std::shared_ptr<Agent> AgentPtr;

		class COVERAGE_API AgentActionIndex
		{
		public:
			int m_elem;
			int m_total;

			AgentActionIndex( int _elem, int _total ) : m_elem(_elem), m_total(_total) {}
		};
	}
}
#endif