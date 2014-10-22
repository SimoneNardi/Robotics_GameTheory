#include "stdafx.h"

#include "BoxPlotFile.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

//////////////////////////////////////////////////////////////////////////
void Index_MonitorUpdate::set(std::string _statistics, int _monitor, double _jump, double _epsilon, int _period, int _endTime, std::vector<double> const& _values)
{
	if(m_index.find(_statistics) == m_index.end())
		m_index[_statistics] = MonitorUpdate_ThiefJump();

	m_index[_statistics].set(_monitor, _jump, _epsilon, _period,_endTime,_values);

	return;
}

void Index_MonitorUpdate::set(std::string _statistics, int _monitor, double _jump, double _epsilon, int _period, int _endTime,  double _value)
{
	if(m_index.find(_statistics) == m_index.end())
		m_index[_statistics] = MonitorUpdate_ThiefJump();

	m_index[_statistics].set(_monitor, _jump, _epsilon, _period,_endTime,_value);

	return;
}

std::vector<double> Index_MonitorUpdate::get(std::string _statistics, int _monitor, double _jump, double _epsilon, int _period, int _endTime)
{
	std::vector<double> l_result;
	if(m_index.find(_statistics) != m_index.end())
		l_result = m_index[_statistics].get(_monitor, _jump, _epsilon, _period, _endTime);
	return l_result;
}

//////////////////////////////////////////////////////////////////////////
void MonitorUpdate_ThiefJump::set(int _monitor, double _jump, double _epsilon, int _period, int _endTime, std::vector<double> const& _values)
{
	if(m_monitorUpdate.find(_monitor) == m_monitorUpdate.end())
		m_monitorUpdate[_monitor] = ThiefJump_Epsilon();

	m_monitorUpdate[_monitor].set(_jump, _epsilon, _period,_endTime,_values);

	return;
}

void MonitorUpdate_ThiefJump::set(int _monitor, double _jump, double _epsilon, int _period, int _endTime, double _value)
{
	if(m_monitorUpdate.find(_monitor) == m_monitorUpdate.end())
		m_monitorUpdate[_monitor] = ThiefJump_Epsilon();

	m_monitorUpdate[_monitor].set(_jump, _epsilon, _period,_endTime,_value);

	return;
}

std::vector<double> MonitorUpdate_ThiefJump::get(int _monitor, double _jump, double _epsilon, int _period, int _endTime)
{
	std::vector<double> l_result;
	if(m_monitorUpdate.find(_monitor) != m_monitorUpdate.end())
		l_result = m_monitorUpdate[_monitor].get(_jump, _epsilon, _period, _endTime);
	return l_result;
}

//////////////////////////////////////////////////////////////////////////
void ThiefJump_Epsilon::set(double _jump, double _epsilon, int _period, int _endTime, std::vector<double> const& _values)
{
	if(m_thiefJump.find(_jump) == m_thiefJump.end())
		m_thiefJump[_jump] = Epsilon_Period();

	m_thiefJump[_jump].set(_epsilon, _period,_endTime,_values);

	return;
}
void ThiefJump_Epsilon::set(double _jump, double _epsilon, int _period, int _endTime, double _value)
{
	if(m_thiefJump.find(_jump) == m_thiefJump.end())
		m_thiefJump[_jump] = Epsilon_Period();

	m_thiefJump[_jump].set(_epsilon, _period, _endTime, _value);

	return;
}
std::vector<double> ThiefJump_Epsilon::get(double _jump, double _epsilon, int _period, int _endTime)
{
	std::vector<double> l_result;
	if(m_thiefJump.find(_jump) != m_thiefJump.end())
		l_result = m_thiefJump[_jump].get(_epsilon, _period,_endTime);
	return l_result;
}

//////////////////////////////////////////////////////////////////////////
void Epsilon_Period::set(double _epsilon, int _period, int _endTime, std::vector<double> const& _values)
{
	if(m_epsilon.find(_epsilon) == m_epsilon.end())
		m_epsilon[_epsilon] = Period_EndTime();

	m_epsilon[_epsilon].set(_period,_endTime,_values);

	return;
}
void Epsilon_Period::set(double _epsilon, int _period, int _endTime, double _value)
{
	if(m_epsilon.find(_epsilon) == m_epsilon.end())
		m_epsilon[_epsilon] = Period_EndTime();

	m_epsilon[_epsilon].set(_period, _endTime, _value);

	return;
}
std::vector<double> Epsilon_Period::get(double _epsilon, int _period, int _endTime)
{
	std::vector<double> l_result;
	if(m_epsilon.find(_epsilon) != m_epsilon.end())
		l_result = m_epsilon[_epsilon].get(_period,_endTime);
	return l_result;
}

//////////////////////////////////////////////////////////////////////////
void Period_EndTime::set(int _period, int _endTime, std::vector<double> const& _values)
{
	if(m_period.find(_period) == m_period.end())
		m_period[_period] = EndTime_Value();

	m_period[_period].set(_endTime,_values);

	return;
}
void Period_EndTime::set(int _period, int _endTime, double _value)
{
	if(m_period.find(_period) == m_period.end())
		m_period[_period] = EndTime_Value();

	m_period[_period].set( _endTime, _value);

	return;
}
std::vector<double> Period_EndTime::get(int _period, int _endTime)
{
	std::vector<double> l_result;
	if(m_period.find(_period) != m_period.end())
		l_result = m_period[_period].get(_endTime);
	return l_result;
}

//////////////////////////////////////////////////////////////////////////
void EndTime_Value::set(int _endTime, std::vector<double> const& _values)
{
	if(m_endTime.find(_endTime) == m_endTime.end())
		m_endTime[_endTime] = std::vector<double>();

	m_endTime[_endTime] = _values;

	return;
}
void EndTime_Value::set(int _endTime, double _value)
{
	if(m_endTime.find(_endTime) == m_endTime.end())
		m_endTime[_endTime] = std::vector<double>();

	m_endTime[_endTime].push_back(_value);

	return;
}
std::vector<double> EndTime_Value::get(int _endTime)
{
	std::vector<double> l_result;
	if(m_endTime.find(_endTime) != m_endTime.end())
		l_result = m_endTime[_endTime];
	return l_result;
}

//////////////////////////////////////////////////////////////////////////
void BoxPlotFile::printOnFile(std::string const& filename)
{
	std::ofstream outFile;
	outFile.open(filename);
	if(outFile.is_open())
	{
		outFile << "Index" << "\t";
		outFile << "Monitor" << "\t";
		outFile << "Thief Jump" << "\t";
		outFile << "Epsilon" << "\t";
		outFile << "Period" << "\t";
		outFile << "End Time" << endl;

		for(auto it1 = m_values.m_index.begin(); it1 != m_values.m_index.end(); ++it1)
		{
			MonitorUpdate_ThiefJump l_monitor_jump = it1->second;

			for(auto it2 = l_monitor_jump.m_monitorUpdate.begin(); it2 != l_monitor_jump.m_monitorUpdate.end(); ++it2)
			{
				ThiefJump_Epsilon l_jump_epsilon = it2->second; 
				for(auto it3 = l_jump_epsilon.m_thiefJump.begin(); it3 != l_jump_epsilon.m_thiefJump.end(); ++it3)
				{
					Epsilon_Period l_epsilon_period = it3->second; 
					for(auto it4 = l_epsilon_period.m_epsilon.begin(); it4 != l_epsilon_period.m_epsilon.end(); ++it4)
					{
						Period_EndTime l_period_endTime = it4->second; 
						for(auto it5 = l_period_endTime.m_period.begin(); it5 != l_period_endTime.m_period.end(); ++it5)
						{
							EndTime_Value l_endTime_value = it5->second; 
							for(auto it6 = l_endTime_value.m_endTime.begin(); it6 != l_endTime_value.m_endTime.end(); ++it6)
							{
								outFile << it1->first << "\t";
								outFile << it2->first << "\t";
								outFile << it3->first << "\t";
								outFile << it4->first << "\t";
								outFile << it5->first << "\t";
								outFile << it6->first << endl;
								std::vector<double> l_values = it6->second; 
								for(size_t i = 0; i < l_values.size(); ++i)
								{
									outFile << l_values[i];
									if(i == l_values.size()-1)
										outFile << ";";
									else
										outFile << ",";
								}
								outFile << endl;
								outFile.flush();
							}
						}
					}
				}
			}
		}
	}
	outFile.close();
}

//////////////////////////////////////////////////////////////////////////
void BoxPlotFile::printOnNewFile(std::string const& filename, std::string const& filenameVec)
{
	printOnFile(filename);

	std::ofstream outFileAscisse;
	outFileAscisse.open(filenameVec);
	if(outFileAscisse.is_open())
	{
		for(auto it = m_values.m_index.begin(); it!= m_values.m_index.end(); ++it)
		{
			for(auto it1 = it->second.m_monitorUpdate.begin(); it1!= it->second.m_monitorUpdate.end(); ++it1)
			{
				for(auto it2 = it1->second.m_thiefJump.begin(); it2!= it1->second.m_thiefJump.end(); ++it2)
				{
					for(auto it3 = it2->second.m_epsilon.begin(); it3!= it2->second.m_epsilon.end(); ++it3)
					{
						for(auto it4 = it3->second.m_period.begin(); it4!= it3->second.m_period.end(); ++it4)
						{
							for(auto it5 = it4->second.m_endTime.begin(); it5!= it4->second.m_endTime.end(); ++it5)
							{
								outFileAscisse << it->first << " ";
								outFileAscisse << it1->first << " ";
								outFileAscisse << it2->first << " ";
								outFileAscisse << it3->first << " ";
								outFileAscisse << it4->first << endl;
							}
						}
					}
				}
			}
		}
	}
	outFileAscisse.close();
}

void BoxPlotFile::add(std::string _index, int _monitorUpdate, double _thiefJump, double _epsilon, int _period, int _endTime, double _value)
{
	m_values.set(_index, _monitorUpdate, _thiefJump, _epsilon, _period, _endTime, _value);
}

void BoxPlotFile::add(std::string _index, int _monitorUpdate, double _thiefJump, double _epsilon, int _period, int _endTime, std::vector<double> const& _values)
{
	m_values.set(_index, _monitorUpdate, _thiefJump, _epsilon, _period, _endTime, _values);
}