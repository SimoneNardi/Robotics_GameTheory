#include "StructuredArea.h"
#include "DiscretizedArea.h"
#include "CoverageUtility.h"

#include "BaseGeometry/Point2D.h"
#include "BaseGeometry/Line2D.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;
using namespace IDS::BaseGeometry;
using namespace Robotics::GameTheory;

std::set<IDS::BaseGeometry::Shape2D> createRandomObstacles()
{
	return std::set<IDS::BaseGeometry::Shape2D>();
}

//////////////////////////////////////////////////////////////////////////
StructuredArea::StructuredArea(std::vector<Point2D> const& points) : m_external(points, true)
{
	m_obstacles = createRandomObstacles();
}

//////////////////////////////////////////////////////////////////////////
std::shared_ptr<DiscretizedArea> StructuredArea::discretize()
{
	std::shared_ptr<Area> l_area = this->shared_from_this();
	std::shared_ptr<StructuredArea> l_struct = dynamic_pointer_cast<StructuredArea>(l_area);
	return std::make_shared<DiscretizedArea>( l_struct );	
}

//////////////////////////////////////////////////////////////////////////
IDS::BaseGeometry::Point2D StructuredArea::randomPosition() const
{
	/// Compute Random Position:
	srand ( (unsigned int) time(NULL) );

	int xSecret = rand() % DISCRETIZATION_COL;
	int ySecret = rand() % DISCRETIZATION_ROW;

	Box2D l_box = m_external.getBoundingBox();
	Point2D l_bottomLeft = l_box.corner(0);
	Point2D	l_bottomRight = l_box.corner(1);
	Point2D	l_topLeft = l_box.corner(2);
	Point2D	l_topRight = l_box.corner(3);

	double l_xdist = l_bottomLeft.distance(l_bottomRight);
	double l_ydist = l_bottomLeft.distance(l_topLeft);

	double l_xstep = l_xdist / double(DISCRETIZATION_COL);
	double l_ystep = l_ydist / double(DISCRETIZATION_ROW);

	Line2D l_xlineBottom = l_bottomLeft.lineTo(l_bottomRight);
	Line2D l_xlineTop = l_topLeft.lineTo(l_topRight);

	Point2D l_bottom = l_xlineBottom.pointFromOrigin( l_xstep * double(xSecret) );
	Point2D l_top = l_xlineTop.pointFromOrigin( l_xstep * double(xSecret) );

	Line2D l_yline = l_bottom.lineTo(l_top);
	return l_yline.pointFromOrigin( l_ystep * double(ySecret) );
}

//////////////////////////////////////////////////////////////////////////
bool StructuredArea::isInside( IDS::BaseGeometry::Box2D const& _box) const
{
	bool l_inside = false;
	for(int i = 0; i < 4; ++i)
	{
		l_inside = m_external.contains(_box.corner(i));
		if(l_inside)
		{
			bool l_outFromObstacle = true;
			for(std::set<IDS::BaseGeometry::Shape2D>::iterator it = m_obstacles.begin(); it != m_obstacles.end(); ++it)
			{
				l_outFromObstacle = !it->contains(_box.corner(i));
				if(!l_outFromObstacle)
				{
					l_inside = false;
					break;
				}
			}
			if(l_inside)
				return true;
		}
	}

	l_inside = m_external.contains(_box.center());
	if(l_inside)
	{
		bool l_outFromObstacle = true;
		for(std::set<IDS::BaseGeometry::Shape2D>::iterator it = m_obstacles.begin(); it != m_obstacles.end(); ++it)
		{
			l_outFromObstacle = !it->contains(_box.center());
			if(!l_outFromObstacle)
			{
				l_inside = false;
				break;
			}
		}
		if(l_inside)
			return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
IDS::BaseGeometry::Box2D StructuredArea::getBoundingBox() const
{
	return m_external.getBoundingBox();
}
