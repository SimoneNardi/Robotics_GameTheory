///////////////////////////////////////////////////////////
//  Neutral.h
//  Created on:      13-may-2013 10.14.52
//  Original author: s.nardi
///////////////////////////////////////////////////////////
#ifndef NEUTRAL_H
#define NEUTRAL_H
#pragma once

//	DTMManager
#include "CoverageExport.h"
#include "Agent.h"

namespace Robotics 
{
	namespace GameTheory 
	{
		class COVERAGE_API Neutral : public Agent
		{
		public:
			Neutral(int _id, AgentPosition _position ) : Agent(_id, _position) {}
			
			~Neutral() {}
		};
	}
}
#endif