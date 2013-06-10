///////////////////////////////////////////////////////////
//  StructuredArea.h
//  Created on:      13-may-2013 10.14.52
//  Original author: s.nardi
///////////////////////////////////////////////////////////
#ifndef STRUCTURED_AREA_H
#define STRUCTURED_AREA_H
#pragma once

//	DTMManager
#include "CoverageExport.h"
#include "Area.h"

#include "BaseGeometry/Shape2D.h"

#include <memory>

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
		
		class COVERAGE_API StructuredArea: public Area
		{
			IDS::BaseGeometry::Shape2D m_external;

			std::set<IDS::BaseGeometry::Shape2D> m_obstacles;
		public:
			/// Create an area without obstacles
			StructuredArea(std::vector<IDS::BaseGeometry::Point2D> const& points);

			/// Discretize the Area
			virtual std::shared_ptr<DiscretizedArea> discretize();

			/// Get a point inside the area
			virtual IDS::BaseGeometry::Point2D StructuredArea::randomPosition() const;

			/// True if a corner or the center is inside the area, False otherwise
			virtual bool isInside( IDS::BaseGeometry::Box2D const& _box) const;

			IDS::BaseGeometry::Box2D getBoundingBox() const;

			/// 
			double getDistance() const;
		};
	}
}
#endif