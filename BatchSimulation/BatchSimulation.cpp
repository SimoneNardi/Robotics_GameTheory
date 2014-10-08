// BatchSimulation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "CoverageAlgorithm.h"
#include "BoxPlotFile.h"

using namespace Robotics::GameTheory;
using namespace IDS::BaseGeometry;
using namespace std;

#define _TEST
#define _STATIC 
#define _EPSILON 
//const int N_MAX = 100000000;
const int N_MAX = 100000;
//const int N_MAX = 500;
struct Log
{
	ofstream m_logFile;
	Log(std::string const& name = "end.txt") : m_logFile()
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
	std::vector<double> StopRate;
	std::vector<int> MonitorUpdateTime;
	std::vector<double> ThiefJump;
	std::vector<int> TimeEnd;
	std::vector<int> Period;
	int TestCase;
} g_config;

void readSimulationConfigFile(Log & _log)
{
#ifdef _EPSILON
	std::ifstream file("simple_config_EPSILON.dat");
#else
#ifndef _TEST
#ifdef _STATIC
	std::ifstream file("config_STATIC.dat");
#else
	std::ifstream file("config.dat");
#endif
#else
#ifdef _STATIC
	std::ifstream file("simple_config_STATIC.dat");
#else
	std::ifstream file("simple_config.dat");
#endif
#endif	
#endif
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

			if(l_token[0] == "StopRate")
			{
				_log << "StopRate" << endl;
				for(size_t i = 1; i < l_token.size(); ++i)
				{
					g_config.StopRate.push_back(atof(l_token[i].c_str()));
					_log << g_config.StopRate.back() << endl;
				}
			}
			else if(l_token[0] == "Monitor")
			{
				_log << "Monitor" << endl;
				for(size_t i = 1; i < l_token.size(); ++i)
				{
					g_config.MonitorUpdateTime.push_back(atoi(l_token[i].c_str()));
					_log << g_config.MonitorUpdateTime.back() << "\t";
				}
				_log << endl;
			}
			else if(l_token[0] == "Jump")
			{
				_log << "Jump" << endl;
				for(size_t i = 1; i < l_token.size(); ++i)
				{
					g_config.ThiefJump.push_back(atof(l_token[i].c_str()));
					_log << g_config.ThiefJump.back() << "\t";
				}
				_log << endl;
			}
			else if(l_token[0] == "Print")
			{
				_log << "EndTime" << endl;
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
	l_result.push_back("Scenario_5G_1T_multiAgent.dat");
#ifndef _TEST
	l_result.push_back("Scenario_10G_1T_multiAgent.dat");
	l_result.push_back("Scenario_15G_1T_multiAgent.dat");
	l_result.push_back("Scenario_20G_1T_multiAgent.dat");
#endif
#if 0
	//l_result.push_back("Scenario_7G_1T_multiAgent.dat");
	//l_result.push_back("Scenario_7G_1T_collide.dat");
	//l_result.push_back("Scenario_15G_1T_collide.dat");

	//l_result.push_back("Scenario_7G_1T_b.dat");
	////l_result.push_back("Scenario_7G_1T_c.dat");
	////l_result.push_back("Scenario_7G_1T_d.dat");
	//l_result.push_back("Scenario_14G_1T_a.dat");
	////l_result.push_back("Scenario_14G_1T_b.dat");
	//l_result.push_back("Scenario_25G_1T_a.dat");
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

#if 0
	//l_result.push_back("sagoma.txt");
#endif
	return l_result;
}


#ifdef _EPSILON// Test al variare di epsilon
const int g_medianNumberObject = 100;
const int g_steadyValueCompared = 10;
void computeSteadyValue(std::vector<double> & _result_container, double& _steadyValue , int &_steadyIndex)
{
	_steadyIndex = _result_container.size();
	//_steadyValue = 0.;
	//for(int i = 1; i <= g_medianNumberObject; ++i)
	//{
	//	int index = _result_container.size() - i;
	//	if(index < 0 && index >= _result_container.size())
	//		break;
	//	_steadyValue += _result_container[_result_container.size() - i];
	//}

	//_steadyValue /= double(g_medianNumberObject);

	_steadyValue = 0.;
	for(int i = 0; i < _result_container.size(); ++i)
	{
		if (_steadyValue < _result_container[i])
			_steadyValue = _result_container[i];
	}

	for(int j = 0; j < _result_container.size(); ++j)
	{
		double l_steadyValuePartialMean=0;
		int num = 0;
		for(int i = 0; i < g_steadyValueCompared; ++i)
		{
			int index = i+j;
			if(index >= _result_container.size())
				break;

			num++;
			l_steadyValuePartialMean += _result_container[index];

		}
		l_steadyValuePartialMean /= double(num);

		if(l_steadyValuePartialMean > _steadyValue * .95)
		{
			_steadyIndex = j;
			break;
		}
	}
}


#if 1
	int main(int argc, char* argv[])
	{
		Log l_log("log.txt");

		//system("pause");
	#ifndef _TEST
		std::string l_folname = "scenarios_EPSILON.dat";
	#else
		std::string l_folname = "simple_scenarios_EPSILON.dat";
	#endif
		if(argc < 1)
			return -1;
		else if(argc >= 2)
			l_folname = argv[1];

		std::vector<std::string> l_AgentFilenames = getAgentNames(l_folname);
		std::vector<std::string> l_AreaFilenames = getAreaNames(l_folname);

		readSimulationConfigFile(l_log);

		for(size_t o = 0; o < l_AgentFilenames.size(); ++o )
		{
			std::string l_AgentFilename = l_AgentFilenames[o];
			std::string l_folder = l_AgentFilename.substr(0,l_AgentFilename.find_last_of("/\\")+1);
			std::string l_AgentName = l_AgentFilename.substr(l_AgentFilename.find_last_of("/\\")+1, l_AgentFilename.find_last_of("."));

			l_log << "******************************************" << endl;
			l_log << "Agent File: " << l_AgentName << endl;

			for( size_t l= 0; l < l_AreaFilenames.size(); ++l )
			{
				std::string l_AreaFilename = l_AreaFilenames[l];
				std::string l_AreaName = l_AreaFilename.substr(l_AreaFilename.find_last_of("/\\")+1, l_AreaFilename.find_last_of("."));

				std::string l_nameoriginal = l_AreaName.substr(0,l_AreaName.find_last_of("."));
				l_nameoriginal += ("_" + l_AgentName.substr(0,l_AgentName.find_last_of(".")));

				l_log << "-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-" << endl;
				l_log << "Area File: " << l_AreaName << endl;

				try
				{
					for(int l_algorithmType = 0; l_algorithmType < 1; ++l_algorithmType )
					{
						std::string l_name = l_nameoriginal;
						std::string l_algName = (l_algorithmType == 0 ? "_DISL" : l_algorithmType == 1 ? "_PIPIP" : "_PARETO");
						l_log << "-----Algorithm:" << l_algName << endl;
						l_name += l_algName;
						l_name+= "_EPSILON";
						BoxPlotFile l_boxPlot;
						for(size_t l_periodIndex = 0; l_periodIndex < g_config.Period.size(); ++l_periodIndex)
						{
							l_log << "---Period:" << g_config.Period[l_periodIndex] << endl;

							// preparo i nomi dei file:
							char buffername[1024], buffernameBoxplot[1024], buffernameBoxPlotAscissa[1024];
							sprintf(buffername,"%s_Algorithm_%s_Period_%d",
								l_name.c_str(), 
								l_algName.c_str(),
								g_config.Period[l_periodIndex]);

							sprintf(buffernameBoxplot, "%s_boxplot.txt", buffername);
							sprintf(buffernameBoxPlotAscissa, "%s_boxplot_ascissa.txt", buffername);
							for(int l_epsilonIndex = 0; l_epsilonIndex < g_config.ThiefJump.size(); ++l_epsilonIndex)
							{
								double l_epsilon = g_config.ThiefJump[l_epsilonIndex];

								for(int l_testIndex = 0; l_testIndex < g_config.TestCase; ++l_testIndex)
									// per g_test volte ripeto lo stesso scenario! con punti di partenza diversi per gli agenti
								{
									l_log << "-Case: " << l_testIndex << "..." << endl;

									std::shared_ptr<Robotics::GameTheory::CoverageAlgorithm> l_coverage = 
										Robotics::GameTheory::CoverageAlgorithm::createFromAreaFile(
										l_AreaFilename, 
										l_AgentFilename, 
										l_algorithmType, 
										g_config.Period[l_periodIndex]);
									l_coverage->setExperimentalRate(l_epsilon);

	#ifdef _PRINT
									l_log << "Ho creato lo scenario" << endl;
	#endif

									double l_rate= 2;// = l_coverage->getExplorationRate();
									int k = 0;
									l_coverage->updateMonitor();
									for(auto l_stoprate_index = 0; l_stoprate_index < g_config.StopRate.size(); ++ l_stoprate_index)
									{
										l_coverage->update(	N_MAX, -1, 0);

										l_coverage->m_stats.printNewPerformanceIndex("../pippo.jpg", false);
										l_coverage->m_stats.printPerformanceIndex("../pippo.jpg", false);
										l_coverage->m_stats.printPotential("../pippo.jpg", false);

										system("pause");

										double l_steadyValue;
										int l_steadyIndex;
										computeSteadyValue(l_coverage->m_stats.m_performanceIndex, l_steadyValue , l_steadyIndex);

										l_boxPlot.add( 0, l_epsilonIndex,
											g_config.Period[l_periodIndex], N_MAX,
											l_steadyValue);

										l_boxPlot.add( 1, l_epsilonIndex,
											g_config.Period[l_periodIndex], N_MAX,
											l_steadyIndex);

										double l_steadyValue_old;
										int l_steadyIndex_old;
										computeSteadyValue(l_coverage->m_stats.m_oldPerformanceIndex, l_steadyValue_old , l_steadyIndex_old);

										/// print data for BoxPlot:
										l_log << "Steady Value " << l_steadyValue_old << "\t Steady Index " << l_steadyIndex_old;
										l_log << endl;

										l_boxPlot.add( 2, l_epsilonIndex,
											g_config.Period[l_periodIndex], N_MAX,
											l_steadyValue_old);

										l_boxPlot.add( 3, l_epsilonIndex,
											g_config.Period[l_periodIndex], N_MAX,
											l_steadyIndex_old);

									}
								}
							}
						}
						l_log << "Print BoxPlot Data _" << l_name << endl;
						l_log.flush();
						l_boxPlot.printOnNewFile(l_name + "_boxPlot.txt", l_name + "_VEC_boxPlot.txt");
					}

				}
				catch(...)
				{
					l_log <<"Unable to process file " << l_nameoriginal << endl;
					l_log.flush();
				}
			}
		}

		l_log <<"Process is ended!"<< endl;
		l_log.close();

		system("pause");

		return 0;
	}

	#else
	int main(int argc, char* argv[])
	{
		Log l_log("log.txt");

		//system("pause");
	#ifndef _TEST
		std::string l_folname = "scenarios_EPSILON.dat";
	#else
		std::string l_folname = "simple_scenarios_EPSILON.dat";
	#endif
		if(argc < 1)
			return -1;
		else if(argc >= 2)
			l_folname = argv[1];

		std::vector<std::string> l_AgentFilenames = getAgentNames(l_folname);
		std::vector<std::string> l_AreaFilenames = getAreaNames(l_folname);

		readSimulationConfigFile(l_log);

		for(size_t o = 0; o < l_AgentFilenames.size(); ++o )
		{
			std::string l_AgentFilename = l_AgentFilenames[o];
			std::string l_folder = l_AgentFilename.substr(0,l_AgentFilename.find_last_of("/\\")+1);
			std::string l_AgentName = l_AgentFilename.substr(l_AgentFilename.find_last_of("/\\")+1, l_AgentFilename.find_last_of("."));

			l_log << "******************************************" << endl;
			l_log << "Agent File: " << l_AgentName << endl;

			for( size_t l= 0; l < l_AreaFilenames.size(); ++l )
			{
				std::string l_AreaFilename = l_AreaFilenames[l];
				std::string l_AreaName = l_AreaFilename.substr(l_AreaFilename.find_last_of("/\\")+1, l_AreaFilename.find_last_of("."));

				std::string l_nameoriginal = l_AreaName.substr(0,l_AreaName.find_last_of("."));
				l_nameoriginal += ("_" + l_AgentName.substr(0,l_AgentName.find_last_of(".")));

				l_log << "-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-" << endl;
				l_log << "Area File: " << l_AreaName << endl;

				try
				{
					for(int l_algorithmType = 1; l_algorithmType < 2; ++l_algorithmType )
					{
						std::string l_name = l_nameoriginal;
						std::string l_algName = (l_algorithmType == 0 ? "_DISL" : l_algorithmType == 1 ? "_PIPIP" : "_PARETO");
						l_log << "-----Algorithm:" << l_algName << endl;
						l_name += l_algName;
						l_name+= "_EPSILON";
						BoxPlotFile l_boxPlot;
						for(size_t l_periodIndex = 0; l_periodIndex < g_config.Period.size(); ++l_periodIndex)
						{
							l_log << "---Period:" << g_config.Period[l_periodIndex] << endl;

							// preparo i nomi dei file:
							char buffername[1024], buffernameBoxplot[1024], buffernameBoxPlotAscissa[1024];
							sprintf(buffername,"%s_Algorithm_%s_Period_%d",
								l_name.c_str(), 
								l_algName.c_str(),
								g_config.Period[l_periodIndex]);

							sprintf(buffernameBoxplot, "%s_boxplot.txt", buffername);
							sprintf(buffernameBoxPlotAscissa, "%s_boxplot_ascissa.txt", buffername);
							for(int l_epsilonIndex = 0; l_epsilonIndex < g_config.ThiefJump.size(); ++l_epsilonIndex)
							{
								double l_epsilon = g_config.ThiefJump[l_epsilonIndex];

								for(int l_testIndex = 0; l_testIndex < g_config.TestCase; ++l_testIndex)
								// per g_test volte ripeto lo stesso scenario! con punti di partenza diversi per gli agenti
								{
									l_log << "-Case: " << l_testIndex << "..." << endl;

									std::shared_ptr<Robotics::GameTheory::CoverageAlgorithm> l_coverage = 
										Robotics::GameTheory::CoverageAlgorithm::createFromAreaFile(
										l_AreaFilename, 
										l_AgentFilename, 
										l_algorithmType, 
										g_config.Period[l_periodIndex]);
									l_coverage->setExperimentalRate(l_epsilon);

		#ifdef _PRINT
									l_log << "Ho creato lo scenario" << endl;
		#endif

									double l_rate= 2;// = l_coverage->getExplorationRate();
									int k = 0;
									l_coverage->updateMonitor();
									for(auto l_stoprate_index = 0; l_stoprate_index < g_config.StopRate.size(); ++ l_stoprate_index)
									{
										l_coverage->update(	N_MAX, -1, 0);

										double l_steadyValue;
										int l_steadyIndex;
										computeSteadyValue(l_coverage->m_stats.m_performanceIndex, l_steadyValue , l_steadyIndex);

										l_boxPlot.add( 0, l_epsilonIndex,
											g_config.Period[l_periodIndex], N_MAX,
											l_steadyValue);

										l_boxPlot.add( 1, l_epsilonIndex,
											g_config.Period[l_periodIndex], N_MAX,
											l_steadyIndex);

										double l_steadyValue_old;
										int l_steadyIndex_old;
										computeSteadyValue(l_coverage->m_stats.m_oldPerformanceIndex, l_steadyValue_old , l_steadyIndex_old);

										/// print data for BoxPlot:
										l_log << "Steady Value " << l_steadyValue_old << "\t Steady Index " << l_steadyIndex_old;
										l_log << endl;

										l_boxPlot.add( 2, l_epsilonIndex,
											g_config.Period[l_periodIndex], N_MAX,
											l_steadyValue_old);

										l_boxPlot.add( 3, l_epsilonIndex,
											g_config.Period[l_periodIndex], N_MAX,
											l_steadyIndex_old);

									}
								}
							}
						}
						l_log << "Print BoxPlot Data _" << l_name << endl;
						l_log.flush();
						l_boxPlot.printOnNewFile(l_name + "_boxPlot.txt", l_name + "_VEC_boxPlot.txt");
					}

				}
				catch(...)
				{
					l_log <<"Unable to process file " << l_nameoriginal << endl;
					l_log.flush();
				}
			}
		}

		l_log <<"Process is ended!"<< endl;
		l_log.close();

		system("pause");

		return 0;
	}
	#endif
#else
#ifdef _STATIC
int main(int argc, char* argv[])
{
	Log l_log("log.txt");

	//system("pause");
#ifndef _TEST
	std::string l_folname = "scenarios_STATIC.dat";
#else
	std::string l_folname = "simple_scenarios_STATIC.dat";
#endif
	if(argc < 1)
		return -1;
	else if(argc >= 2)
		l_folname = argv[1];

	std::vector<std::string> l_AgentFilenames = getAgentNames(l_folname);
	std::vector<std::string> l_AreaFilenames = getAreaNames(l_folname);

	readSimulationConfigFile(l_log);

	for(size_t o = 0; o < l_AgentFilenames.size(); ++o )
	{
		std::string l_AgentFilename = l_AgentFilenames[o];
		std::string l_folder = l_AgentFilename.substr(0,l_AgentFilename.find_last_of("/\\")+1);
		std::string l_AgentName = l_AgentFilename.substr(l_AgentFilename.find_last_of("/\\")+1, l_AgentFilename.find_last_of("."));

		l_log << "******************************************" << endl;
		l_log << "Agent File: " << l_AgentName << endl;

		for( size_t l= 0; l < l_AreaFilenames.size(); ++l )
		{
			std::string l_AreaFilename = l_AreaFilenames[l];
			std::string l_AreaName = l_AreaFilename.substr(l_AreaFilename.find_last_of("/\\")+1, l_AreaFilename.find_last_of("."));

			std::string l_nameoriginal = l_AreaName.substr(0,l_AreaName.find_last_of("."));
			l_nameoriginal += ("_" + l_AgentName.substr(0,l_AgentName.find_last_of(".")));

			l_log << "-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-" << endl;
			l_log << "Area File: " << l_AreaName << endl;

			try
			{
				for(int l_algorithmType = 0; l_algorithmType < 1; ++l_algorithmType )
				{
					std::string l_name = l_nameoriginal;
					std::string l_algName = (l_algorithmType == 0 ? "_DISL" : l_algorithmType == 1 ? "_PIPIP" : "_PARETO");
					l_log << "-----Algorithm:" << l_algName << endl;
					l_name += l_algName;
					l_name+= "_STATIC";
					BoxPlotFile l_boxPlot;
					for(size_t l_periodIndex = 0; l_periodIndex < g_config.Period.size(); ++l_periodIndex)
					{
						l_log << "---Period:" << g_config.Period[l_periodIndex] << endl;

						// preparo i nomi dei file:
						char buffername[1024], buffernameBoxplot[1024], buffernameBoxPlotAscissa[1024];
						sprintf(buffername,"%s_Algorithm_%s_Period_%d",
							l_name.c_str(), 
							l_algName.c_str(),
							g_config.Period[l_periodIndex]);

						sprintf(buffernameBoxplot, "%s_boxplot.txt", buffername);
						sprintf(buffernameBoxPlotAscissa, "%s_boxplot_ascissa.txt", buffername);

						for(int l_testIndex = 0; l_testIndex < g_config.TestCase; ++l_testIndex)
							// per g_test volte ripeto lo stesso scenario! con punti di partenza diversi per gli agenti
						{
							l_log << "-Case: " << l_testIndex << "..." << endl;

							std::shared_ptr<Robotics::GameTheory::CoverageAlgorithm> l_coverage = 
								Robotics::GameTheory::CoverageAlgorithm::createFromAreaFile(
								l_AreaFilename, 
								l_AgentFilename, 
								l_algorithmType, 
								g_config.Period[l_periodIndex]);

#ifdef _PRINT
							l_log << "Ho creato lo scenario" << endl;
#endif

							double l_rate= 2;// = l_coverage->getExplorationRate();
							int k = 0;
							l_coverage->updateMonitor();
							for(auto l_stoprate_index = 0; l_stoprate_index < g_config.StopRate.size(); ++ l_stoprate_index)
							{
								do
								{
									l_coverage->update(	1, -1, 0);
									l_rate = l_coverage->getExplorationRate();
									k++;
									if(k== 100000)
										cout << "Rate 100000 = " << l_rate << endl;
								}
								while((l_rate >= g_config.StopRate[l_stoprate_index] - 1.e-9) && (k < g_config.Period[l_periodIndex]*N_MAX));

								l_coverage->update(	g_config.Period[l_periodIndex]+1, -1, 0 );
								k+=g_config.Period[l_periodIndex]+1;

								double l_value = l_coverage->getGlobalTrajectoryCoverage();
								double l_valueBenefit = l_coverage->getBoxPlotValue();
								double l_valueBenefitNewIndex = l_coverage->getBoxPlotValueNewIndex();

								/// print data for BoxPlot:
								//l_valueBoxPlot.push_back(l_value);
								l_log << "Coverage " << l_value << "\t Old Index " << l_valueBenefit << "\t New Index " << l_valueBenefitNewIndex;
								l_log << endl;

								l_boxPlot.add( 0, l_stoprate_index,
									g_config.Period[l_periodIndex], k,
									l_value);

								l_boxPlot.add( 1, l_stoprate_index,
									g_config.Period[l_periodIndex], k,
									l_valueBenefit);

								l_boxPlot.add( 2, l_stoprate_index,
									g_config.Period[l_periodIndex], k,
									l_valueBenefitNewIndex);
							}
						}
					}
					l_log << "Print BoxPlot Data _" << l_name << endl;
					l_log.flush();
					l_boxPlot.printOnNewFile(l_name + "_boxPlot.txt", l_name + "_VEC_boxPlot.txt");
				}

			}
			catch(...)
			{
				l_log <<"Unable to process file " << l_nameoriginal << endl;
				l_log.flush();
			}
		}
	}

	l_log <<"Process is ended!"<< endl;
	l_log.close();

	system("pause");

	return 0;
}
#else
int main(int argc, char* argv[])
{
	Log l_log("log.txt");

	//system("pause");
#ifndef _TEST
	std::string l_folname = "scenarios.dat";
#else
	std::string l_folname = "simple_scenarios.dat";
#endif
	if(argc < 1)
		return -1;
	else if(argc >= 2)
		l_folname = argv[1];

	std::vector<std::string> l_AgentFilenames = getAgentNames(l_folname);
	std::vector<std::string> l_AreaFilenames = getAreaNames(l_folname);

	readSimulationConfigFile(l_log);

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
			l_name += ("_" + l_AgentName.substr(0,l_AgentName.find_last_of(".")));

			l_log << "-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-" << endl;
			l_log << "Area File: " << l_AreaName << endl;

			try
			{
				for(int l_algorithmType = 1; l_algorithmType < 2; ++l_algorithmType )
				{
					std::string l_algName = (l_algorithmType == 0 ? "_DISL" : l_algorithmType == 1 ? "_PIPIP" : "_PARETO");
					l_log << "-----Algorithm:" << l_algName << endl;
					l_name += l_algName;
					BoxPlotFile l_boxPlot;
					for(size_t l_periodIndex = 0; l_periodIndex < g_config.Period.size(); ++l_periodIndex)
					{
						l_log << "-------Period:" << g_config.Period[l_periodIndex] << endl;

						for(size_t l_monitorUpdateTimeIndex = 0; l_monitorUpdateTimeIndex < g_config.MonitorUpdateTime.size(); ++l_monitorUpdateTimeIndex)
						{
							l_log << "-----Monitor Update " << g_config.MonitorUpdateTime[l_monitorUpdateTimeIndex] << endl;

							for(size_t l_thiefJumpIndex = 0; l_thiefJumpIndex < g_config.ThiefJump.size(); ++l_thiefJumpIndex)
							{
								l_log << "---Thief Jump " << g_config.ThiefJump[l_thiefJumpIndex] << endl;	

								// preparo i nomi dei file:
								char buffername[1024], buffernameBoxplot[1024], buffernameBoxPlotAscissa[1024];
								sprintf(buffername,"%s_Algorithm_%s_Period_%d_MonitorUpdate_%d_ThiefJump_%f",
									l_name.c_str(), 
									l_algName.c_str(),
									g_config.Period[l_periodIndex],
									g_config.MonitorUpdateTime[l_monitorUpdateTimeIndex], 
									g_config.ThiefJump[l_thiefJumpIndex]);

								sprintf(buffernameBoxplot, "%s_boxplot.txt", buffername);
								sprintf(buffernameBoxPlotAscissa, "%s_boxplot_ascissa.txt", buffername);

								//std::vector<double> l_valueBoxPlot;

								for(int l_testIndex = 0; l_testIndex < g_config.TestCase; ++l_testIndex)
									// per g_test volte ripeto lo stesso scenario! con punti di partenza diversi per gli agenti
								{
									l_log << "-Case: " << l_testIndex << "..." << endl;

									std::shared_ptr<Robotics::GameTheory::CoverageAlgorithm> l_coverage = 
										Robotics::GameTheory::CoverageAlgorithm::createFromAreaFile(
										l_AreaFilename, 
										l_AgentFilename, 
										l_algorithmType, 
										g_config.Period[l_periodIndex],
										g_config.ThiefJump[l_thiefJumpIndex]);

									for(size_t l_TimeEndIndex = 0; l_TimeEndIndex < g_config.TimeEnd.size(); ++l_TimeEndIndex)
									{
										if(g_config.TimeEnd[l_TimeEndIndex] == 0)
											continue;

										l_log << "End Time " << g_config.TimeEnd[l_TimeEndIndex] << " ";


										//l_coverage->printArea(l_AreaName + "_area.txt");

										l_coverage->update(
											g_config.TimeEnd[l_TimeEndIndex] - (l_TimeEndIndex==0? 0 : g_config.TimeEnd[l_TimeEndIndex-1]), 
											g_config.MonitorUpdateTime[l_monitorUpdateTimeIndex], 
											1);

#if 0
										// non sono necessarie le stampe dei grafici, 
										// è sufficiente il boxplot per avere l'andamento dell'indice
										if(l_testIndex>0)
											// stampa i grafici su file
										{
											char bufferpot[1024];
											char bufferben[1024];
											//system((std::string("mkdir ") + l_filename.substr(0,l_filename.find_last_of("."))).c_str());
											sprintf(bufferpot,"%s_Iteration_%d_Pot",//
												buffername,
												g_config.TimeEnd[l_TimeEndIndex]);
											l_coverage->printPotential(bufferpot, true);

											sprintf(bufferben,"%s_Iteration_%d_Ben",//
												buffername,
												g_config.TimeEnd[l_TimeEndIndex]);
											l_coverage->printBenefit(bufferben, true);
										}
#endif
										/// print data for BoxPlot:
										double l_value = l_coverage->getBoxPlotValue();
										double l_valueNewIndex = l_coverage->getBoxPlotValueNewIndex();
										//l_valueBoxPlot.push_back(l_value);
										l_log << l_value;
										l_log << endl;

										l_boxPlot.add(
											g_config.MonitorUpdateTime[l_monitorUpdateTimeIndex], 
											l_thiefJumpIndex,
											g_config.Period[l_periodIndex],
											g_config.TimeEnd[l_TimeEndIndex],
											l_value);

										l_boxPlot.add(
											-g_config.MonitorUpdateTime[l_monitorUpdateTimeIndex], 
											l_thiefJumpIndex,
											g_config.Period[l_periodIndex],
											g_config.TimeEnd[l_TimeEndIndex],
											l_valueNewIndex);
									}
								}
							}
						}
					}
					l_log << "Print BoxPlot Data _" << l_name << endl;
					l_log.flush();
					l_boxPlot.printOnNewFile(l_name + "_boxPlot.txt", l_name + "_VEC_boxPlot.txt");
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
#endif
#endif