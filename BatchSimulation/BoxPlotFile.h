///////////////////////////////////////////////////////////
//  BoxPlotFile.h
//  Created on:      13-may-2013 10.14.52
//  Original author: s.nardi
///////////////////////////////////////////////////////////
#ifndef BOX_PLOT_FILE_H
#define BOX_PLOT_FILE_H
#pragma once

#include <map>
#include <vector>

struct EndTime_Value
{
	std::map<int, std::vector<double>> m_endTime;

	void set(int endTime, std::vector<double> const& _values);
	void set(int endTime, double _value);
	std::vector<double> get(int _endTime);
};

struct Period_EndTime
{
	std::map<int, EndTime_Value> m_period;

	void set(int _period, int endTime, std::vector<double> const& _values);
	void set(int _period, int endTime, double _value);
	std::vector<double> get(int _period, int _endTime);
};

struct Epsilon_Period
{
	std::map<double, Period_EndTime> m_epsilon;

	void set(double _epsilon, int _period, int endTime, std::vector<double> const& _values);
	void set(double _epsilon, int _period, int endTime, double _value);
	std::vector<double> get(double _epsilon, int _period, int _endTime);
};

struct ThiefJump_Epsilon
{
	std::map<double, Epsilon_Period> m_thiefJump;

	void set(double _jump, double _epsilon, int _period, int endTime, std::vector<double> const& _values);
	void set(double _jump, double _epsilon, int _period, int endTime, double _value);
	std::vector<double> get(double _jump, double _epsilon, int _period, int _endTime);
};

struct MonitorUpdate_ThiefJump
{
	std::map<int, ThiefJump_Epsilon> m_monitorUpdate;
	void set(int _monitor, double _jump, double _epsilon, int _period, int endTime, std::vector<double> const& _values);
	void set(int _monitor, double _jump, double _epsilon, int _period, int endTime, double _value);
	std::vector<double> get(int _monitor, double _jump, double _epsilon, int _period, int _endTime);
};

struct Index_MonitorUpdate
{
	std::map<std::string, MonitorUpdate_ThiefJump> m_index;
	void set(std::string index, int _monitor, double _jump, double _epsilon, int _period, int endTime, std::vector<double> const& _values);
	void set(std::string index, int _monitor, double _jump, double _epsilon, int _period, int endTime, double _value);
	std::vector<double> get(std::string index, int _monitor, double _jump, double _epsilon, int _period, int _endTime);
};

class BoxPlotFile
{
public:
	Index_MonitorUpdate m_values;
public:

	void add(std::string index, int _monitorUpdate, double _thiefJump, double _epsilon, int _period, int _endTime, double _value);
	void add(std::string index, int _monitorUpdate, double _thiefJump, double _epsilon, int _period, int _endTime, std::vector<double> const& _values);

	void printOnFile(std::string const& _filename);

	void printOnNewFile(std::string const& filename, std::string const& filenameVec);
};

#endif