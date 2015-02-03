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

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	if(argc > 1)
	{
		std::string name = argv[1];
		g_configurationFile->readFromPhotoFile(name);
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

	return 0;
}

