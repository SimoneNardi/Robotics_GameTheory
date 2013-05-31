#include "Guard.h"
#include "DiscretizedArea.h"

using namespace Robotics::GameTheory;
using namespace IDS::BaseGeometry;

Guard::Guard( int _teamID, int _id, AgentPosition _position ) 
	: Agent(_id, _position), m_teamID(_teamID) 
{}

Guard::~Guard()
{}

//////////////////////////////////////////////////////////////////////////
std::vector<AgentPosition> Guard::getFeasibleActions( std::shared_ptr<DiscretizedArea> _space ) const
{
	AreaCoordinate l_currCoord = _space->getCoordinate( m_position.getPoint2D() );

	std::vector<AreaCoordinate> l_squares = _space->getStandardApproachableValidSquares(l_currCoord);
	_space->addSpecialApproachableValidSquares(l_currCoord, l_squares);

	std::vector<AgentPosition> l_result;
	for(size_t i = 0; i < l_squares.size(); ++i )
	{
		l_result.push_back( AgentPosition(_space->getPosition(l_squares[i]), m_position.m_camera) );
	}

	return l_result;
}