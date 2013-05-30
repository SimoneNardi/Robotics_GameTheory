///////////////////////////////////////////////////////////
//  Guard.h
//  Created on:      13-may-2013 10.14.52
//  Original author: s.nardi
///////////////////////////////////////////////////////////
#ifndef GUARD_H
#define GUARD_H
#pragma once

//	DTMManager
#include "CoverageExport.h"
#include "Agent.h"

namespace Robotics 
{
	namespace GameTheory 
	{
		class COVERAGE_API Guard : public Agent
		{
			int m_teamID;

		public:
			Guard( int _teamID, int _id, AgentPosition _position );

			~Guard();

			inline virtual bool isGuard() const {return true;}
		};
	}
}
#endif