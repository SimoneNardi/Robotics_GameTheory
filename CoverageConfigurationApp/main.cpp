// main.cpp : Defines the entry point for the console application.
//

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "CoverageAlgorithm.h"

using namespace Robotics::GameTheory;
using namespace IDS::BaseGeometry;
using namespace std;

#define D_TEST

struct Log
{
	ofstream m_logFile;
	Log(std::string const& name = "end.dat") : m_logFile()
	{
		m_logFile = ofstream(name);
		if(!m_logFile.is_open())
			throw std::exception("Unable to open log file");
	}

	Log& operator<<(std::string const& str)
	{
		if(m_logFile.is_open())
			m_logFile << str;
		cout << str;
		return *this;
	}

	Log& operator<<(double const& num)
	{
		if(m_logFile.is_open())
			m_logFile << num;
		cout << num;
		return *this;
	}

	Log& operator<<(int const& num)
	{
		if(m_logFile.is_open())
			m_logFile << num;
		cout << num;
		return *this;
	}

	void flush()
	{
		if(m_logFile.is_open())
			m_logFile.flush();
	}

	void close()
	{
		if(m_logFile.is_open())
			m_logFile.close();
	}

	// this is the type of std::cout
	typedef std::basic_ostream<char, std::char_traits<char> > CoutType;

	// this is the function signature of std::endl
	typedef CoutType& (*StandardEndLine)(CoutType&);

	// define an operator<< to take in std::endl
	Log& operator<<(StandardEndLine manip)
	{
		// call the function, but we cannot return it's value
		manip(m_logFile);
		manip(std::cout);

		return *this;
	}
};

//////////////////////////////////////////////////////////////////////////
// String Tokenizer
void tokenize(const std::string& str, const std::string& separators, std::vector<std::string> &tokens)
{
	tokens.clear();

	// skip separators at beginning
	std::string::size_type lastPos = str.find_first_not_of(separators, 0);

	// find first separator
	std::string::size_type pos = str.find_first_of(separators, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		// found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));

		// skip separators.  Note the "not_of"
		lastPos = str.find_first_not_of(separators, pos);

		// find next "non-separator"
		pos = str.find_first_of(separators, lastPos);
	}
}

struct SimulationConfig
{
	std::vector<int> TimeEnd;
	std::vector<int> Period;
	int TestCase;
} g_config;

void readSimulationConfigFile(Log & _log, std::string const& _filename)
{
	std::ifstream file(_filename);

	std::string l_sep("\t");

	if( file.is_open() )
	{
		std::string l_line;
		while (std::getline(file, l_line))
		{
			std::vector<std::string> l_token;
			tokenize(l_line, l_sep, l_token);
			if(l_token.empty())
				continue;

			if(l_token[0] == "TimeEnd")
			{
				_log << "TimeEnd" << endl;
				for(size_t i = 1; i < l_token.size(); ++i)
				{
					g_config.TimeEnd.push_back(atoi(l_token[i].c_str()));
					_log << g_config.TimeEnd.back() << "\t";
				}
				_log << endl;
			}
			else if(l_token[0] == "Period")
			{
				_log << "Period" << endl;
				for(size_t i = 1; i < l_token.size(); ++i)
				{
					g_config.Period.push_back(atoi(l_token[i].c_str()));
					_log << g_config.Period.back() << "\t";
				}
				_log << endl;
			}
			else if(l_token[0] == "TestCase")
			{
				_log << "TestCase" << endl;
				for(size_t i = 1; i < l_token.size(); ++i)
				{
					g_config.TestCase = atoi(l_token[i].c_str());
					_log << g_config.TestCase << endl;
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
std::vector<std::string> getAgentNames(std::string const& _folname)
{
	std::vector<std::string> l_result;
	l_result.push_back("Scenario_5G_1T_multiAgent.txt");

#ifndef _TEST
	l_result.push_back("Scenario_10G_1T_multiAgent.txt");
	l_result.push_back("Scenario_15G_1T_multiAgent.txt");
	l_result.push_back("Scenario_20G_1T_multiAgent.txt");
#endif
	return l_result;
}

//////////////////////////////////////////////////////////////////////////
std::vector<std::string> getAreaNames(std::string const& _folname)
{
	std::vector<std::string> l_result;
	/// più scenari
	l_result.push_back("External.txt");

#ifndef _TEST
	l_result.push_back("External_trig.txt");
	l_result.push_back("External_road.txt");
	l_result.push_back("External_room.txt");
#endif
	return l_result;
}

int main(int argc, char* argv[])
{
	Log l_log("Coverage_Configuration_log.dat");

	std::string l_folname;

	if(argc < 1)
		return -1;
	else if(argc >= 2)
		l_folname = argv[1];

	std::vector<std::string> l_AgentFilenames = getAgentNames(l_folname);
	std::vector<std::string> l_AreaFilenames = getAreaNames(l_folname);

#ifndef _TEST
	std::string l_file("Coverage_Configuration_Config.txt");
#else
	std::string l_file("Coverage_Configuration_Config_Test.txt");
#endif

	readSimulationConfigFile(l_log, l_file);

	for(size_t o = 0; o < l_AgentFilenames.size(); ++o )
	{
		std::string l_AgentFilename = l_AgentFilenames[o];
		std::string l_folder = l_AgentFilename.substr(0,l_AgentFilename.find_last_of("/\\")+1);
		std::string l_AgentName = l_AgentFilename.substr(l_AgentFilename.find_last_of("/\\")+1, l_AgentFilename.find_last_of("."));

		l_log << "******************************************" << endl;
		l_log << "Agent File: " << l_AgentName << endl;

		for(size_t l= 0; l < l_AreaFilenames.size(); ++l )
		{
			std::string l_AreaFilename = l_AreaFilenames[l];
			std::string l_AreaName = l_AreaFilename.substr(l_AreaFilename.find_last_of("/\\")+1, l_AreaFilename.find_last_of("."));

			std::string l_name = l_AreaName.substr(0,l_AreaName.find_last_of("."));
			l_name += ("-" + l_AgentName.substr(0,l_AgentName.find_last_of(".")));

			l_log << "-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-" << endl;
			l_log << "Area File: " << l_AreaName << endl;

			try
			{
				for(int l_algorithmType = 2; l_algorithmType < 3; ++l_algorithmType )
				{
					std::string l_algName = (l_algorithmType == 0 ? "DISL" : l_algorithmType == 1 ? "PIPIP" : "PARETO");
					l_log << "---------Algorithm: " << l_algName << endl;
					l_name += "-";
					l_name += l_algName;

					for(size_t l_periodIndex = 0; l_periodIndex < g_config.Period.size(); ++l_periodIndex)
					{
						l_log << "-------Period: " << g_config.Period[l_periodIndex] << endl;

						for(int l_testIndex = 1; l_testIndex <= g_config.TestCase; ++l_testIndex)
							// per g_test volte ripeto lo stesso scenario! con punti di partenza diversi per gli agenti
						{
							std::shared_ptr<Robotics::GameTheory::CoverageAlgorithm> l_coverage = 
								Robotics::GameTheory::CoverageAlgorithm::createFromAreaFile(
								l_AreaFilename, 
								l_AgentFilename, 
								l_algorithmType, 
								g_config.Period[l_periodIndex],
								0.);

							l_log << "---Case: " << l_testIndex << "..." << endl;

							// preparo i nomi dei file:
							char bufferName[1024], bufferNameConf[1024];
							sprintf(bufferName,"Output-%s-Period_%d-Case_%d",
								l_name.c_str(),
								g_config.Period[l_periodIndex],
								l_testIndex);

							for(int l_timeEndIndex = 0; l_timeEndIndex < g_config.TimeEnd.size(); ++l_timeEndIndex)
							{
								int l_timeEnd = g_config.TimeEnd[l_timeEndIndex];

								l_log << "-Time End: " << l_timeEnd << endl;	

								sprintf(bufferNameConf, "%s-ConfigAtTime_%d.dat", bufferName, l_timeEnd);

								l_coverage->update(
									g_config.TimeEnd[l_timeEndIndex] - (l_timeEndIndex==0? 0 : g_config.TimeEnd[l_timeEndIndex-1]),
									0,0, false);

								/// print photo:
								std::string l_outputFileName (bufferNameConf);
								l_coverage->printPhoto(l_outputFileName);

								l_log << "Printed " << bufferNameConf << endl;
								l_log.flush();
							}
						}
					}
				}
			}
			catch(...)
			{
				l_log <<"Unable to process file " << l_name << endl;
				l_log.flush();
			}
		}
	}

	l_log <<"Process is ended!"<< endl;
	l_log.close();

	system("pause");

	return 0;
}