// Grid_Escape.cpp : Defines the entry point for the console application.
//
#include "SFML\Graphics.hpp"

#include <windows.h>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "Identifier.h"
#include "ConfigurationFile.h"
#include "WindowEventHandler.h"
#include "Challenge.h"
#include "Viewer.h"

#include "CoverageAlgorithm.h"

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	/* initialize random seed: */
	srand (time(NULL));

	std::shared_ptr<Brain> l_brain = nullptr;
	if(argc > 1)
	{
		std::string name = argv[1];
		g_configurationFile->readFromPhotoFile(name);
		l_brain = std::make_shared<Challenge>();
	}
	else
	{
		std::string name = argv[0];
		std::string folder = name.substr(0, name.find_last_of('/\\')+1);
		folder+="Configuration/";
		std::string l_AreaFilename = folder + "External.txt";
		std::string l_AgentFilename = folder + "5G_1T.dat";

		std::shared_ptr<Robotics::GameTheory::CoverageAlgorithm> l_coverage = 
			Robotics::GameTheory::CoverageAlgorithm::createFromAreaFile(
			l_AreaFilename, 
			l_AgentFilename, 
			0, // type algorithm
			0, // period
			0); // epsilon
		
		g_configurationFile->createFromAlgorithm(l_coverage);
		l_brain = std::make_shared<Viewer>();
	}
	
	std::cout << "Start the Game!" <<std::endl;

	l_brain->run();

	if(argc == 1)
	{
		l_brain->print();
	}

	std::cout << std::endl << "Game is finished!" <<std::endl;

	return 0;
}

