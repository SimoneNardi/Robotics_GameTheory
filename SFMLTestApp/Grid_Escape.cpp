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
<<<<<<< HEAD
#include "Viewer.h"
=======
>>>>>>> origin/Pareto_Efficient

#include "CoverageAlgorithm.h"

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
<<<<<<< HEAD
	/* initialize random seed: */
	srand (time(NULL));

	std::shared_ptr<Brain> l_brain = nullptr;
=======
>>>>>>> origin/Pareto_Efficient
	if(argc > 1)
	{
		std::string name = argv[1];
		g_configurationFile->readFromPhotoFile(name);
<<<<<<< HEAD
		l_brain = std::make_shared<Challenge>();
=======
>>>>>>> origin/Pareto_Efficient
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
<<<<<<< HEAD
		l_brain = std::make_shared<Viewer>();
	}
	
	std::cout << "Start the Game!" <<std::endl;

	l_brain->run();

	if(argc == 1)
	{
		l_brain->print();
	}

	std::cout << std::endl << "Game is finished!" <<std::endl;
=======
	}


	/* initialize random seed: */
	srand (time(NULL));

	std::shared_ptr<Challenge> l_challenge = std::make_shared<Challenge>();
	
	std::cout << "Start the Game!" <<std::endl;

	l_challenge->run();

	if(argc == 1)
	{
		l_challenge->print();
	}

	std::cout << std::endl << "Game is finished!" <<std::endl;
	//system("pause"); l_challenge->close();
>>>>>>> origin/Pareto_Efficient

	return 0;
}

