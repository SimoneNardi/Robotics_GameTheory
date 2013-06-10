#include "UnStructuredArea.h"

#include "BaseGeometry/Point2D.h"

using namespace IDS::BaseGeometry;
using namespace Robotics::GameTheory;

//////////////////////////////////////////////////////////////////////////
std::shared_ptr<DiscretizedArea> UnStructuredArea::discretize()
{
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
IDS::BaseGeometry::Point2D UnStructuredArea::randomPosition() const
{
	return Point2D();
}

//////////////////////////////////////////////////////////////////////////
double UnStructuredArea::getDistance() const
{
	return 0.;
}
