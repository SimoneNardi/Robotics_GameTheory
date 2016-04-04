#include "Agent.h"
#include "Guard.h"
#include "DiscretizedArea.h"


#include "BaseGeometry/Shape2D.h"
#include "BaseGeometry/Arc2D.h"
#include "BaseGeometry/Point2D.h"

//aggiunta
#include "World.h"
#include "CoverageExport.h"
#include "Agent.h"
#include "CoverageAlgorithm.h"
#include "LearningAlgorithm.h"



using namespace std;
using namespace Robotics;
using namespace Robotics::GameTheory;
using namespace IDS;
using namespace IDS::BaseGeometry;

// da me
typedef std::shared_ptr<Thief> ThiefPtr;
std::shared_ptr<World> m_world;


//////////////////////////////////////////////////////////////////////////
// MemoryGuardTrajectories
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
void MemoryGuardTrajectories::add(MemoryGuardTrajectory const& _elem)
{
	m_elems.insert(m_elems.begin(), _elem);

	if(m_elems.size() > size_t(m_maxSize))
		m_elems.erase(m_elems.begin()+m_maxSize);
}

//////////////////////////////////////////////////////////////////////////
void MemoryGuardTrajectories::computeBestWorstTrajectories()
{
	double l_bestPayoff = -IDSMath::Infinity;
	double l_worstPayoff = IDSMath::Infinity;
	for(size_t i = 0; i < m_elems.size(); ++i)
	{
		double l_currentPayoff = m_elems[i].m_payoff;

		if(l_currentPayoff > l_bestPayoff)
		{
			l_bestPayoff = l_currentPayoff;
			m_best= i;
		}

		if(l_currentPayoff < l_worstPayoff)
		{
			l_worstPayoff = l_currentPayoff;
			m_worst = i;
		}
	}
	return;
}

//////////////////////////////////////////////////////////////////////////
//	GuardTrajectoryPosition
//////////////////////////////////////////////////////////////////////////
bool GuardTrajectoryPosition::operator==(GuardTrajectoryPosition const& other) const
{
	return m_position == other.m_position && m_index == other.m_index;
}

bool GuardTrajectoryPosition::operator!=(GuardTrajectoryPosition const& other) const
{
	return !( *this == other );
}

//////////////////////////////////////////////////////////////////////////
//	GuardTrajectory
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
bool GuardTrajectory::contains(AgentPosition _nextPos)
{
	for(size_t i = 0; i < m_elems.size(); ++i)
	{
		if( m_elems[i].m_position.getPoint2D().equals(_nextPos.getPoint2D()) )
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool GuardTrajectory::operator==(GuardTrajectory const& other) const
{
	if(m_elems.size() != other.m_elems.size())
		return false;

	for(size_t i = 0; i < m_elems.size(); ++i)
	{
		if(!(m_elems[i] == other.m_elems[i]) )
			return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
bool GuardTrajectory::operator!=(GuardTrajectory const& other) const
{
	return !(*this == other);
}

//////////////////////////////////////////////////////////////////////////
//	AgentPosition
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
bool AgentPosition::operator==(AgentPosition const& other) const
{
	return m_point.equals(other.m_point) && m_camera == other.m_camera;
}

//////////////////////////////////////////////////////////////////////////
bool AgentPosition::operator!=(AgentPosition const& other) const
{
	return !(*this == other);
}



//////////////////////////////////////////////////////////////////////////
void AgentPosition::updateCounter(std::shared_ptr<DiscretizedArea> area)
{
	AreaCoordinate l_coord = area->getCoordinate(m_point);

	area->getSquare( l_coord.row, l_coord.col )->increaseCounter();

	// Calcolo la copertura in base alla camera:
	// per ora conta solo il raggio massimo e non cala con la distanza!
	std::vector<AreaCoordinate> l_cover = m_camera.getCoverage(l_coord, area);

	for(size_t i = 0; i < l_cover.size(); ++i)
	{
		std::shared_ptr<Square> l_square = area->getSquare( l_cover[i].row, l_cover[i].col );
		if(l_square)
			l_square->increaseCounter();
	}
}

//////////////////////////////////////////////////////////////////////////
bool AgentPosition::communicable(std::shared_ptr<Agent> _otherAgent) const
{
	return m_point.distance( _otherAgent->getCurrentPosition().getPoint2D() ) < 2 * m_camera.getFarRadius() + IDSMath::TOLERANCE;
}

//////////////////////////////////////////////////////////////////////////
bool AgentPosition::visible(std::shared_ptr<Square> _area) const
{
	Shape2D sh = m_camera.getVisibleArea(m_point);
	if(sh.isValid())
		return sh.contains( _area->getBoundingBox().center() );
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////
double AgentPosition::computeCosts() const
{
	return m_camera.computeCosts();
}

//////////////////////////////////////////////////////////////////////////
std::vector<AreaCoordinate> AgentPosition::getCoverage(std::shared_ptr<DiscretizedArea> _space ) const
{
	AreaCoordinate l_center = _space->getCoordinate(m_point);
	return m_camera.getCoverage(l_center, _space);
}

//////////////////////////////////////////////////////////////////////////
IDS::BaseGeometry::Shape2D AgentPosition::getVisibleArea() const
{
	return m_camera.getVisibleArea(m_point);
}

//////////////////////////////////////////////////////////////////////////
//	CameraPosition
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
bool CameraPosition::operator==(CameraPosition const& other) const
{
	if(fabs(m_farRadius-other.m_farRadius) > IDSMath::TOLERANCE)
		return false;
	else if(fabs(m_nearRadius-other.m_nearRadius) > IDSMath::TOLERANCE)
		return false;
	else if(fabs(m_orientation-other.m_orientation) > IDSMath::TOLERANCE)
		return false;
	else if(fabs(m_angle-other.m_angle) > IDSMath::TOLERANCE)
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CameraPosition::operator!=(CameraPosition const& other) const
{
	return !(*this==other);
}

void printArray(std::vector<AreaCoordinate> v, int c_row, int c_col) {
	for (int i = 0; i < v.size(); ++i) {
		double x = v.at(i).row;
		double y = v.at(i).col;
		double dist = sqrt(pow((x - c_row), 2) + pow((y - c_col), 2));
		std::cout << "row: " << v.at(i).row << " \t col: " << v.at(i).col << "\t prob: " << v.at(i).p << std::endl;//<< "\n d : " << dist << std::endl;
	}
}

//////////////////////////////////////////////////////////////////////////
/*auto AgentPosition::ProbabilityOfDetection(std::shared_ptr<DiscretizedArea> area, AreaCoordinate p_r, AreaCoordinate p_t)
{
	int d;
	return  d = area->getDistance(p_r, p_t);
}*/


std::vector<AreaCoordinate> CameraPosition::getCoverage(AreaCoordinate _center, std::shared_ptr<DiscretizedArea> _area) const
{
	Point2D l_pt = _area->getPosition(_center);
	Shape2D l_sensorArea = this->getVisibleArea(l_pt);

	if (!l_sensorArea.isValid())
		return std::vector<AreaCoordinate>();

	int l_rowDelta =int(floor(m_farRadius / _area->getYStep())) + 1;
	int l_colDelta =int(floor(m_farRadius / _area->getXStep())) + 1;


	std::vector<AreaCoordinate> result;
	AreaCoordinate l_elem;
	for (int i = -l_rowDelta; i <= l_rowDelta; ++i)
	{
		int row = _center.row + i;
		if (row < 0 || row > _area->getNumRow()) //check sui margini
			continue;
		for (int j = -l_colDelta; j <= l_colDelta; ++j)
		{
			int col = _center.col + j;
			if (col < 0 || col > _area->getNumCol())
				continue;

			SquarePtr l_square = _area->getSquare(row, col); //return m_lattice

			//std::cout << "Questi sono nella riga row: " << row << " col:" << col << std::endl;
			if (!l_square)
				continue;
			
			Point2D l_center = l_square->getBoundingBox().center();
			// assegno solo i punti che sono interni all'area del sensore
			if (l_sensorArea.contains(l_center))
				//funzione che assegna la probabilita
			{
				l_elem.row = row;
				l_elem.col = col;
				//l_elem.p = ProbabilityOfDetection(_center, row, col);
				result.push_back(l_elem);

			}
		}
	}

	//auto p = AgentPosition::ProbabilityOfDetection(_area, _center, c);
	
	
	// Debug screen output
	
	//std::cout << " m_farRadius: " << m_farRadius << std::endl;
	//std::cout << " l_rowDelta: " << l_rowDelta << std::endl;
	//std::cout << " l_colDelta: " << l_colDelta << std::endl;
	/*std::cout << " getXSquare: " << _area->getXStep() << std::endl;
	std::cout << " getYSquare: " << _area->getYStep() << std::endl;
	std::cout << " getSquare: " << _area->getSquare(1, 1) << std::endl;
	std::cout << " getNumCol: " << _area->getNumCol() << std::endl;
	std::cout << " getNumRow: " << _area->getNumRow() << std::endl;*/
	//std::cout << " size of Lattice: " << _area->getLattice().size() << endl;
	//std::cout << " centro ROW: " << _center.row << " COL: " << _center.col << std::endl;
	/*
	std::cout << "dimensione di result: " << result.size() << std::endl;
	printArray(result, _center.row, _center.col);
	*/
	//std::cout << "dimensione di result: " << result.size() << std::endl;
	
	//printArray(result, _center.row, _center.col);
	
	return result;
}



//////////////////////////////////////////////////////////////////////////
BaseGeometry::Shape2D CameraPosition::getVisibleArea(BaseGeometry::Point2D const& point) const
	try
{
	if(fabs(m_farRadius) < IDSMath::TOLERANCE)
		return Shape2D();

	Curve2D l_external = makeCircle( point, m_farRadius, false);
	return Shape2D( std::vector<Curve2D>(1,l_external) );
}
catch (...)
{
	return Shape2D();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
/*						MemoryGuardTrajectory							*/
//////////////////////////////////////////////////////////////////////////

bool MemoryGuardTrajectory::equals(MemoryGuardTrajectory const& _other) const
{
	if(this->m_memTrajectory != _other.m_memTrajectory)
		return false;

	if(fabs( this->m_payoff - _other.m_payoff ) > IDSMath::TOLERANCE )
		return false;

	return true;
}

bool MemoryGuardTrajectory::equals(GuardTrajectory const& _trajectory, double _payoff) const
{
	if(this->m_memTrajectory != _trajectory)
		return false;

	if(fabs( this->m_payoff - _payoff ) > IDSMath::TOLERANCE )
		return false;

	return true;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
