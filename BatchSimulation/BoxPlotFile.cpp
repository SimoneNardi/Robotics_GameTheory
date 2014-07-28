#include "stdafx.h"

#include "BoxPlotFile.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

//////////////////////////////////////////////////////////////////////////
void MonitorUpdate_ThiefJump::set(int _monitor, int _jump, int _period, int _endTime, std::vector<double> const& _values)
{
	if(m_monitorUpdate.find(_monitor) == m_monitorUpdate.end())
		m_monitorUpdate[_monitor] = ThiefJump_Period();

	m_monitorUpdate[_monitor].set(_jump, _period,_endTime,_values);

	return;
}

void MonitorUpdate_ThiefJump::set(int _monitor, int _jump, int _period, int _endTime,double _value)
{
	if(m_monitorUpdate.find(_monitor) == m_monitorUpdate.end())
		m_monitorUpdate[_monitor] = ThiefJump_Period();

	m_monitorUpdate[_monitor].set(_jump, _period,_endTime,_value);

	return;
}

std::vector<double> MonitorUpdate_ThiefJump::get(int _monitor, int _jump, int _period, int _endTime)
{
	std::vector<double> l_result;
	if(m_monitorUpdate.find(_monitor) != m_monitorUpdate.end())
		l_result = m_monitorUpdate[_monitor].get(_jump, _period, _endTime);
	return l_result;
}

//////////////////////////////////////////////////////////////////////////
void ThiefJump_Period::set(int _jump, int _period, int _endTime, std::vector<double> const& _values)
{
	if(m_thiefJump.find(_jump) == m_thiefJump.end())
		m_thiefJump[_jump] = Period_EndTime();

	m_thiefJump[_jump].set(_period,_endTime,_values);

	return;
}
void ThiefJump_Period::set(int _jump, int _period, int _endTime, double _value)
{
	if(m_thiefJump.find(_jump) == m_thiefJump.end())
		m_thiefJump[_jump] = Period_EndTime();

	m_thiefJump[_jump].set(_period, _endTime, _value);

	return;
}
std::vector<double> ThiefJump_Period::get(int _jump, int _period, int _endTime)
{
	std::vector<double> l_result;
	if(m_thiefJump.find(_jump) != m_thiefJump.end())
		l_result = m_thiefJump[_jump].get(_period,_endTime);
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
		outFile << "Monitor" << "\t";
		outFile << "Thief Jump" << "\t";
		outFile << "Period" << "\t";
		outFile << "End Time" << endl;

		for(auto it1 = m_values.m_monitorUpdate.begin(); it1 != m_values.m_monitorUpdate.end(); ++it1)
		{
			ThiefJump_Period l_jump_period = it1->second; 
			for(auto it2 = l_jump_period.m_thiefJump.begin(); it2 != l_jump_period.m_thiefJump.end(); ++it2)
			{
				Period_EndTime l_period_endTime = it2->second; 
				for(auto it3 = l_period_endTime.m_period.begin(); it3 != l_period_endTime.m_period.end(); ++it3)
				{
					EndTime_Value l_endTime_value = it3->second; 
					for(auto it4 = l_endTime_value.m_endTime.begin(); it4 != l_endTime_value.m_endTime.end(); ++it4)
					{
						outFile << it1->first << "\t";
						outFile << it2->first << "\t";
						outFile << it3->first << "\t";
						outFile << it4->first << endl;
						std::vector<double> l_values = it4->second; 
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
		for(auto it = m_values.m_monitorUpdate.begin(); it!= m_values.m_monitorUpdate.end(); ++it)
		{
			for(auto it2 = it->second.m_thiefJump.begin(); it2!= it->second.m_thiefJump.end(); ++it2)
			{
				for(auto it3 = it2->second.m_period.begin(); it3!= it2->second.m_period.end(); ++it3)
				{
					for(auto it4 = it3->second.m_endTime.begin(); it4!= it3->second.m_endTime.end(); ++it4)
					{
						outFileAscisse << it->first << " ";
						outFileAscisse << it2->first << " ";
						outFileAscisse << it3->first << " ";
						outFileAscisse << it4->first << endl;
					}
				}
			}
		}
	}
	outFileAscisse.close();
}

void BoxPlotFile::add(int _monitorUpdate, int _thiefJump, int _period, int _endTime, double _value)
{
	m_values.set(_monitorUpdate, _thiefJump, _period, _endTime, _value);
}

void BoxPlotFile::add(int _monitorUpdate, int _thiefJump, int _period, int _endTime, std::vector<double> const& _values)
{
	m_values.set(_monitorUpdate, _thiefJump, _period, _endTime, _values);
}