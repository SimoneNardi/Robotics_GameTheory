///////////////////////////////////////////////////////////
//  DiscretizedArea.h
//  Created on:      13-may-2013 10.14.52
//  Original author: s.nardi
///////////////////////////////////////////////////////////
#ifndef DISCRETIZED_AREA_H
#define DISCRETIZED_AREA_H
#pragma once

//	DTMManager
#include "CoverageExport.h"
#include "Area.h"

#include "BaseGeometry/Box2D.h"

#include <set>

namespace IDS
{
	namespace BaseGeometry
	{
		class Shape2D;
		class Point2D;
	}
}

namespace Robotics 
{
	namespace GameTheory 
	{
		class Graph;
		class Agent;
		class StructuredArea;
		class UnStructuredArea;

		struct AreaCoordinate
		{
			int row;
			int col;
		};

		inline bool operator< (AreaCoordinate const& a, AreaCoordinate const& b)
		{
			if(a.row < b.row)
				return true;
			else if(a.row > b.row)
				return false;
			else if(a.col < b.col)
				return true;
			else if(a.col > b.col)
				return false;
			else
				return false;
		}

		class COVERAGE_API Square
		{
			/// True if an agent can stay in this square
			bool m_valid;

			/// number of agent which can examine this square
			int m_counter;

			/// The value associated to the square by the monitor
			double m_value;

			IDS::BaseGeometry::Box2D m_box;

			double m_old_value;
		public:
			/// Set the box of the Square
			void setBoundingBox(IDS::BaseGeometry::Box2D const& _box);
			/// Get the Bounding Box of the square
			IDS::BaseGeometry::Box2D getBoundingBox() {return m_box;}

			/// Set counter to zero.
			inline void resetCounter() {m_counter = 0;};

			/// Add one to counter.
			inline void increaseCounter() {++m_counter;};

			inline bool isValid() const {return m_valid;}
			inline void setValid(bool valid);

			inline int getTheNumberOfAgent() const {return m_counter;}

			inline double getValue() const {return m_value;}
			inline void setValue(double _value);

			Square() : m_valid(true), m_counter(0), m_value(0.), m_old_value(0.) {}

			IDS::BaseGeometry::Point2D vertex(int i) const;
			IDS::BaseGeometry::Point2D agentVertex(int i) const;

			bool isChanged() const;
			/// Set the value to zero.
			void resetValue();

			//bool equals(std::shared_ptr<Square> _other) const;
		};

		typedef std::shared_ptr<Square> SquarePtr;

		class COVERAGE_API DiscretizedArea : public std::enable_shared_from_this<DiscretizedArea>
		{
			/// Lattice from bottom left to upper right, per rows.
			std::vector< SquarePtr > m_lattice;
			int m_numRow;
			int m_numCol;

			/// x step to speed up searching
			double m_xStep;
			/// y step to speed up searching
			double m_yStep;

		public:
			DiscretizedArea(IDS::BaseGeometry::Shape2D const& _external, std::set< IDS::BaseGeometry::Shape2D > const& _obstacles);

			DiscretizedArea(std::shared_ptr<StructuredArea> _area);
			DiscretizedArea(std::shared_ptr<UnStructuredArea> _area);
			 
			/// Compute Graph of the discretized area.
			Graph getGraph() const;

			/// Update index of lattice covered by the given agent.
			void updateSquaresCounter(std::shared_ptr<Agent> agent);

			/// Set all the lattice indexes to zero.
			void resetCounter();
			void resetValue();

			/// Get the origin of the lattice.
			IDS::BaseGeometry::Point2D getOrigin() const;

			/// Compute index of row and index of column of the given point.
			AreaCoordinate getCoordinate( IDS::BaseGeometry::Point2D const& point ) const;

			SquarePtr getSquare(int row, int col) const;
			SquarePtr getSquare(AreaCoordinate _coord) const;
			SquarePtr getSquare(IDS::BaseGeometry::Point2D const& V) const;
			IDS::BaseGeometry::Point2D getPosition(AreaCoordinate const& _coord) const;

			std::vector<SquarePtr> getSquares() const;
			
			AgentPosition getRandomPosition() const;
			void setThiefPosition(AgentPosition const& _pos);

			bool isOut(AgentPosition const& pos) const;

			/// 
			void setRandomSquareValue();

			std::vector<AreaCoordinate> getStandardApproachableValidSquares(AreaCoordinate const& _current) const;
			void addSpecialApproachableValidSquares(AreaCoordinate const& _current, std::vector<AreaCoordinate> & _loci) const;

			std::set<std::shared_ptr<Square> > getVisibleSquares(AgentPosition const& _pos) const;

			double getXStep() const {return m_xStep;}
			double getYStep() const {return m_yStep;}
			int getNumRow() const {return m_numRow;}
			int getNumCol() const {return m_numCol;}

			int numberOfSquaresCoveredByGuards() const;

			friend class COVERAGE_API ISLAlgorithm;
		};
	}
}
#endif