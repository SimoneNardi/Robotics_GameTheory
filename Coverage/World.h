///////////////////////////////////////////////////////////
//  World.h
//  Created on:      05-may-2014 10.14.52
//  Original author: s.nardi
///////////////////////////////////////////////////////////
#ifndef WORLD_H
#define WORLD_H
#pragma once

//	DTMManager
#include "CoverageExport.h"

#include <memory>
#include <set>

namespace Robotics
{
	namespace GameTheory 
	{
		class DiscretizedArea;
		class Agent;
		class Guard;
		class Thief;
		class LearningAlgorithm;
		class Area;

		class COVERAGE_API World
		{
		protected:
			/// The set of all the agents in the area.
			std::set< std::shared_ptr<Agent> > m_agent;

			/// The space where agents can move.
			std::shared_ptr<DiscretizedArea> m_space;

		public:
			World(std::set< std::shared_ptr<Agent> > _agent, std::shared_ptr<DiscretizedArea> _space);
			World(std::set< std::shared_ptr<Agent> > _agent, std::shared_ptr<Area> _space);

			/// Guards
			std::set< std::shared_ptr<Guard> > getGuards() const;

			/// Thieves
			std::set< std::shared_ptr<Thief> > getThieves() const;

			/// Neutrals
			std::set< std::shared_ptr<Agent> > getNeutrals() const;

			// Move Thieves in the world
			void moveThieves(int _thiefJump);

			inline std::shared_ptr<DiscretizedArea> getSpace() const {return m_space;}

			void wakeUpAgentIfSecurityIsLow();

			void randomInitializeGuards();

			void randomInitializeNeutrals();

			void randomInitializeThief();

			int getNumberOfAgent();

			void addThief(std::shared_ptr<Thief> _thief);

			double getMaximumValue();

			void removeAllThieves();
		};
	}
}
#endif