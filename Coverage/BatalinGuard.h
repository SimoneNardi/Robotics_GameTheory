#if 0
///////////////////////////////////////////////////////////
//  BatalinGuard.h
//  Created on:      29-jul-2014 10.14.52
//  Original author: s.nardi
///////////////////////////////////////////////////////////
#ifndef BATALIN_GUARD_H
#define BATALIN_GUARD_H
#pragma once

//	DTMManager
#include "CoverageExport.h"
#include "Guard.h"

namespace Robotics 
{
	namespace GameTheory 
	{
		///////////////////////////////////////////////////////////
		class COVERAGE_API BatalinGuard : public Guard
		{
		public:
			BatalinGuard( int _teamID, int _id, AgentPosition _position, int _trajectoryLength = 4, int _memorySpace = 2 );

			~BatalinGuard();

		protected:
		};

		typedef std::shared_ptr<BatalinGuard> BatalinGuardPtr;

	}
}
#endif
#endif