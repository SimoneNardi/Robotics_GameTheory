///////////////////////////////////////////////////////////
//  Sink.h
//  Created on:      13-may-2013 10.14.52
//  Original author: s.nardi
///////////////////////////////////////////////////////////
#ifndef SINK_H
#define SINK_H
#pragma once

//	DTMManager
#include "CoverageExport.h"
#include "Agent.h"

namespace Robotics 
{
	namespace GameTheory 
	{
		class COVERAGE_API Sink : public Agent
		{

		public:
			Sink(int _id, AgentPosition _position ) : Agent(_id, _position) {}

			~Sink() {}

			virtual bool isSink() const {return true;}
		};

		typedef std::shared_ptr<Sink> SinkPtr;
	}
}
#endif