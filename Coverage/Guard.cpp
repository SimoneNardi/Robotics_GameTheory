#include "Guard.h"

using namespace Robotics::GameTheory;
using namespace IDS::BaseGeometry;

Guard::Guard( int _teamID, int _id, AgentPosition _position ) 
	: Agent(_id, _position), m_teamID(_teamID) 
{}

Guard::~Guard()
{}