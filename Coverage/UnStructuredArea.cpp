#include "UnStructuredArea.h"

#include "BaseGeometry/Point2D.h"

using namespace IDS::BaseGeometry;
using namespace Robotics::GameTheory;

/// Discretize the Area
std::shared_ptr<DiscretizedArea> UnStructuredArea::discretize()
{
	return nullptr;
}

/// Get a point inside the area
IDS::BaseGeometry::Point2D UnStructuredArea::randomPosition() const
{
	return Point2D();
}