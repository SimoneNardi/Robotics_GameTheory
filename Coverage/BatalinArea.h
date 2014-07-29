///////////////////////////////////////////////////////////
//  BatalinArea.h
//  Created on:      28-jul-2014 10.14.52
//  Original author: s.nardi
///////////////////////////////////////////////////////////
#ifndef BATALIN_AREA_H
#define BATALIN_AREA_H
#pragma once

//	DTMManager
#include "CoverageExport.h"
#include "Area.h"
#include "DiscretizedArea.h"

#include "BaseGeometry/Box2D.h"

#include <lemon/list_graph.h>
#include <lemon/bfs.h>

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

		class COVERAGE_API BeaconState
		{
			enum Type
			{
				OPEN,
				EXPLORED
			} m_type;

			int m_time;

			void * m_extra;
		};

		class COVERAGE_API Beacon
		{
			std::vector<BeaconState> m_states;
		};

		class COVERAGE_API BatalinSquare : public Square
		{
		protected:
			
			std::shared_ptr<Beacon> m_beacon;

		public:

		};

		typedef std::shared_ptr<BatalinSquare> BatalinSquarePtr;

		class COVERAGE_API BatalinArea : public std::enable_shared_from_this<DiscretizedArea>
		{
		public:
			BatalinArea();
		};
	}
}
#endif