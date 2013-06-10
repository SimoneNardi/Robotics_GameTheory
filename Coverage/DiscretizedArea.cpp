#include "DiscretizedArea.h"
#include "CoverageUtility.h"
#include "StructuredArea.h"
#include "UnStructuredArea.h"
#include "Agent.h"
#include "Graph.h"

#include "BaseGeometry/Line2D.h"
#include "BaseGeometry/Point2D.h"
#include "BaseGeometry/Shape2D.h"
#include "BaseGeometry/MakePoint2D.h"
#include "BaseGeometry/Box2D.h"

#include "BaseGeometry/BaseGeometryTypes.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;
using namespace Robotics;
using namespace Robotics::GameTheory;
using namespace IDS;
using namespace IDS::BaseGeometry;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////////////////		DISCRETIZED AREA		//////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
Graph DiscretizedArea::getGraph() const
{
	Graph graph;
	return graph;
}

//////////////////////////////////////////////////////////////////////////
void DiscretizedArea::updateSquaresCounter(AgentPtr agent)
{
	AgentPosition l_position = agent->getPosition();
	l_position.updateCounter( this->shared_from_this() );
}

//////////////////////////////////////////////////////////////////////////
void DiscretizedArea::resetCounter()
{
	for(size_t i = 0; i < m_lattice.size(); ++i)
		m_lattice[i]->resetCounter();
}

//////////////////////////////////////////////////////////////////////////
SquarePtr DiscretizedArea::getSquare(AreaCoordinate _coord) const
{
	return getSquare(_coord.row, _coord.col);
}

//////////////////////////////////////////////////////////////////////////
SquarePtr DiscretizedArea::getSquare(int row, int col) const
{
	if( row >= m_numRow || row < 0 ||
		col >= m_numCol || col < 0 )
		return nullptr;

	return m_lattice.at(row * m_numCol + col);
}

//////////////////////////////////////////////////////////////////////////
AgentPosition DiscretizedArea::getRandomPosition() const
{
	// TODO
	AgentPosition pos;
	return pos;
}

//////////////////////////////////////////////////////////////////////////
bool DiscretizedArea::isOut(AgentPosition const& _pos) const
{
	Point2D l_point = _pos.getPoint2D();
	AreaCoordinate l_coord = this->getCoordinate( l_point );
	SquarePtr l_square = this->getSquare(l_coord);
	if(!l_square)
		return false;
	return l_square->isValid();
}

//////////////////////////////////////////////////////////////////////////
std::vector<SquarePtr> DiscretizedArea::getSquares() const
{
	return m_lattice;
}

//////////////////////////////////////////////////////////////////////////
DiscretizedArea::DiscretizedArea(std::shared_ptr<StructuredArea> _area)
{
	Box2D l_box = _area->getBoundingBox();

	Point2D l_bottomLeft = l_box.corner(0);
	Point2D	l_bottomRight = l_box.corner(1);
	Point2D	l_topLeft = l_box.corner(2);
	Point2D	l_topRight = l_box.corner(3);

	double l_xdist = l_bottomLeft.distance(l_bottomRight);
	double l_ydist = l_bottomLeft.distance(l_topLeft);

	m_xStep = l_xdist / double(DISCRETIZATION_COL);
	m_yStep = l_ydist / double(DISCRETIZATION_ROW);

	double l_xpos = 0.;
	double l_ypos = 0.;
	m_numRow = 0;
	m_numCol = 0;
	bool l_firstrow = true;
	while(l_ypos < l_ydist + IDSMath::TOLERANCE)
	{
		double l_yorigin = l_ypos + l_bottomLeft.coord(1);
		int l_count = 0;
		l_xpos=0.;
		while( (l_firstrow && l_xpos < l_xdist + IDSMath::TOLERANCE) || (!l_firstrow && l_count < m_numCol))
		{
			double l_xorigin = l_xpos + l_bottomLeft.coord(0);

			Point2D l_mincord = makePoint( IDSReal2D(l_xorigin, l_yorigin), l_box.isEllipsoidic() ? BaseGeometry::EllMetric : BaseGeometry::EucMetric);
			Point2D l_maxcord = makePoint( IDSReal2D(l_xorigin+m_xStep, l_yorigin+m_yStep), l_box.isEllipsoidic() ? BaseGeometry::EllMetric : BaseGeometry::EucMetric);

			Box2D l_boxSquare = makeBoundingBox(l_mincord, l_maxcord);

			SquarePtr l_square = std::make_shared<Square>();
			l_square->setBoundingBox(l_boxSquare);

			if( _area->isInside(l_boxSquare) )
				l_square->setValid(true);
			else
				l_square->setValid(false);

			m_lattice.push_back(l_square);

			l_xpos+=m_xStep;
			if(l_firstrow)
				m_numCol++;
			l_count++;
		}
		l_ypos+=m_yStep;
		m_numRow++;
		l_firstrow = false;
	}
}

//////////////////////////////////////////////////////////////////////////
IDS::BaseGeometry::Point2D DiscretizedArea::getOrigin() const
{
	return m_lattice.at(0)->getBoundingBox().minCoord();
}

//////////////////////////////////////////////////////////////////////////
DiscretizedArea::DiscretizedArea(std::shared_ptr<UnStructuredArea> _area)
{}

//////////////////////////////////////////////////////////////////////////
DiscretizedArea::DiscretizedArea(IDS::BaseGeometry::Shape2D const& _external, std::set< IDS::BaseGeometry::Shape2D > const& _obstacles)
{
	Box2D l_box = _external.getBoundingBox();

	Point2D l_bottomLeft = l_box.corner(0);
	Point2D	l_bottomRight = l_box.corner(1);
	Point2D	l_topLeft = l_box.corner(2);
	Point2D	l_topRight = l_box.corner(3);

	double l_xdist = l_bottomLeft.distance(l_bottomRight);
	double l_ydist = l_bottomLeft.distance(l_topLeft);

	m_xStep = l_xdist / double(DISCRETIZATION_COL);
	m_yStep = l_ydist / double(DISCRETIZATION_ROW);

	double l_xpos = 0.;
	double l_ypos = 0.;
	m_numRow = 0;
	m_numCol = 0;
	bool l_firstrow = true;
	while(l_ypos < l_ydist + IDSMath::TOLERANCE)
	{
		double l_yorigin = l_ypos + l_bottomLeft.coord(1);
		int l_count = 0;
		l_xpos=0.;
		while( (l_firstrow && l_xpos < l_xdist + IDSMath::TOLERANCE) || (!l_firstrow && l_count < m_numCol))
		{
			double l_xorigin = l_xpos + l_bottomLeft.coord(0);

			Point2D l_mincord = makePoint( IDSReal2D(l_xorigin, l_yorigin), l_box.isEllipsoidic() ? BaseGeometry::EllMetric : BaseGeometry::EucMetric);
			Point2D l_maxcord = makePoint( IDSReal2D(l_xorigin+m_xStep, l_yorigin+m_yStep), l_box.isEllipsoidic() ? BaseGeometry::EllMetric : BaseGeometry::EucMetric);

			Box2D l_boxSquare = makeBoundingBox(l_mincord, l_maxcord);

			SquarePtr l_square = std::make_shared<Square>();
			l_square->setBoundingBox(l_boxSquare);

			if( _external.contains(l_boxSquare.center()) )
				l_square->setValid(true);
			else
				l_square->setValid(false);

			m_lattice.push_back(l_square);

			l_xpos+=m_xStep;
			if(l_firstrow)
				m_numCol++;
			l_count++;
		}
		l_ypos+=m_yStep;
		m_numRow++;
		l_firstrow = false;
	}
}

//////////////////////////////////////////////////////////////////////////
AreaCoordinate DiscretizedArea::getCoordinate(Point2D const& point) const
{
	static Point2D l_bottomLeft = m_lattice[0]->getBoundingBox().minCoord();
	static Point2D l_bottomRight = m_lattice[0]->getBoundingBox().corner(1);
	static Point2D l_topLeft = m_lattice[0]->getBoundingBox().corner(2);

	static Line2D l_xLine = l_bottomLeft.lineTo(l_bottomRight);
	static Line2D l_yLine = l_bottomLeft.lineTo(l_topLeft);

	Point2D l_prjVertical = l_yLine.projectPoint(point);
	Point2D l_prjOrizontal = l_xLine.projectPoint(point);

	double l_ydist = l_yLine.parameterAt(l_prjVertical);
	double l_xdist = l_xLine.parameterAt(l_prjOrizontal);

	AreaCoordinate l_coord;
	l_coord.col = int( floor(l_xdist / m_xStep) );
	l_coord.row = int( floor(l_ydist / m_yStep) );
	return l_coord;
}

//////////////////////////////////////////////////////////////////////////
SquarePtr DiscretizedArea::getSquare(Point2D const& V) const
{
	AreaCoordinate l_coord = getCoordinate(V);
	return this->getSquare(l_coord);
}

Point2D DiscretizedArea::getPosition(AreaCoordinate const& _coord) const
{
	SquarePtr l_square = this->getSquare(_coord);
	if(!l_square)
		return Point2D();
	return l_square->getBoundingBox().center();
}

bool g_reverse = false;
//////////////////////////////////////////////////////////////////////////
void DiscretizedArea::setRandomSquareValue()
{
	g_reverse = !g_reverse;
	/// Compute Random Position:
	srand ( (unsigned int) time(NULL) );

	int flag = m_lattice.size() / 100;

	for(size_t i = 0; i < m_lattice.size(); ++i)
	{
#if 1
		int l_valueSecret = rand() % 100;
		m_lattice[i]->setValue(l_valueSecret);
#else
		m_lattice[i]->setValue(g_reverse ? 100- i / flag:  i / flag);
#endif
	}
}

//////////////////////////////////////////////////////////////////////////
std::vector<AreaCoordinate> DiscretizedArea::getStandardApproachableValidSquares(AreaCoordinate const& _current) const
{
	std::vector<AreaCoordinate> result;
	if(_current.col != 0)
	{
		AreaCoordinate pos;
		pos.col = _current.col-1;
		pos.row = _current.row;
		if(this->getSquare(pos)->isValid())
			result.push_back(pos);
	}
	if(_current.col != DISCRETIZATION_COL)
	{
		AreaCoordinate pos;
		pos.col = _current.col+1;
		pos.row = _current.row;
		if(this->getSquare(pos)->isValid())
			result.push_back(pos);
	}
	if(_current.row != 0)
	{
		AreaCoordinate pos;
		pos.col = _current.col;
		pos.row = _current.row-1;
		if(this->getSquare(pos)->isValid())
			result.push_back(pos);
	}
	if(_current.row != DISCRETIZATION_ROW)
	{
		AreaCoordinate pos;
		pos.col = _current.col;
		pos.row = _current.row+1;
		if(this->getSquare(pos)->isValid())
			result.push_back(pos);
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
void DiscretizedArea::addSpecialApproachableValidSquares(AreaCoordinate const& _current, std::vector<AreaCoordinate> & _loci) const
{
	if( _current.col != 0 )
	{
		if(_current.row != 0)
		{
			AreaCoordinate pos;
			pos.col = _current.col-1;
			pos.row = _current.row-1;
			if(this->getSquare(pos)->isValid())
				_loci.push_back(pos);
		}

		if(_current.row != DISCRETIZATION_ROW)
		{
			AreaCoordinate pos;
			pos.col = _current.col-1;
			pos.row = _current.row+1;
			if(this->getSquare(pos)->isValid())
				_loci.push_back(pos);
		}
	}

	if(_current.col != DISCRETIZATION_COL)
	{
		if(_current.row != 0)
		{
			AreaCoordinate pos;
			pos.col = _current.col+1;
			pos.row = _current.row-1;
			if(this->getSquare(pos)->isValid())
				_loci.push_back(pos);
		}

		if(_current.row != DISCRETIZATION_ROW)
		{
			AreaCoordinate pos;
			pos.col = _current.col+1;
			pos.row = _current.row+1;
			if(this->getSquare(pos)->isValid())
				_loci.push_back(pos);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
std::set< std::shared_ptr<Square> > DiscretizedArea::getVisibleSquares(AgentPosition const& _pos) const
{
	AreaCoordinate l_currentPos = this->getCoordinate(_pos.getPoint2D());

	std::set< std::shared_ptr<Square> > result;
	for( int row = 0; row < m_numRow; ++row )
	{
		for( int col = 0; col < m_numCol; ++col )
		{
			if( _pos.visible(m_lattice[row * m_numCol + col]) )
				result.insert(m_lattice[row * m_numCol + col]);
		}
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
void DiscretizedArea::resetValue()
{
	for(size_t i = 0; i < m_lattice.size(); ++i)
		m_lattice[i]->resetValue();
}

//////////////////////////////////////////////////////////////////////////
void DiscretizedArea::setThiefPosition(AgentPosition const& _pos)
{
	SquarePtr l_square = this->getSquare( _pos.getPoint2D() );
	if(l_square)
		l_square->setValue(100.);
	else
		assert(1 == 0);

	AreaCoordinate l_coord = this->getCoordinate( _pos.getPoint2D() );

	for(int i = -4; i < 5; ++i)
	{
		int row = l_coord.row + i;
		if(row < 0 || row >= m_numRow)
			continue;
		for(int j = -4; j < 5; ++j)
		{
			int col = l_coord.col + j;
			if(col < 0 || col >= m_numCol)
				continue;

			if(i == 0 && j == 0)
				continue; 

			double l_value = 100./ double( abs(i)+abs(j) );
			m_lattice[row * m_numCol + col]->setValue(l_value);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
int DiscretizedArea::numberOfSquaresCoveredByGuards() const
{
	int l_total = 0;
	for(size_t i = 0; i < m_lattice.size(); ++i)
		if( m_lattice[i]->getTheNumberOfAgent() > 0)
			++l_total;

	return l_total;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///////////////////////		SQUARE		//////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//bool Square::equals(std::shared_ptr<Square> _other) const
//{
//	return this->getBoundingBox().center().equals(_other->getBoundingBox().center());
//}

//////////////////////////////////////////////////////////////////////////
IDS::BaseGeometry::Point2D Square::vertex(int i) const
{
	int index = i;
	if(i == 2)
		index = 3;
	else if(i == 3)
		index = 2;
	return m_box.corner( index );
}

//////////////////////////////////////////////////////////////////////////
IDS::BaseGeometry::Point2D Square::agentVertex(int i) const
{
	int index = i;
	if(i == 2)
		index = 3;
	else if(i == 3)
		index = 2;
	Point2D l_center = m_box.center();
	Line2D l_line = m_box.corner( index ).lineTo(l_center);
	double l_dist = m_box.corner( index ).distance(l_center);
	return l_line.pointFromOrigin(l_dist/2.);
}

//////////////////////////////////////////////////////////////////////////
void Square::setBoundingBox(IDS::BaseGeometry::Box2D const& _box)
{
	m_box = _box;
}

//////////////////////////////////////////////////////////////////////////
bool Square::isChanged() const
{
	return fabs(m_value - m_old_value) > IDSMath::TOLERANCE;
}

//////////////////////////////////////////////////////////////////////////
void Square::resetValue()
{
	m_value = 1.;
}

//////////////////////////////////////////////////////////////////////////
void Square::setValid(bool valid) 
{
	m_valid = valid;
}

//////////////////////////////////////////////////////////////////////////
void Square::setValue(double _value) 
{
	m_old_value = m_value;
	m_value = _value;
}

