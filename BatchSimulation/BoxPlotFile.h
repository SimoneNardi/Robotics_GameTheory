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

struct ThiefJump_Period
{
	std::map<int, Period_EndTime> m_thiefJump;

	void set(int _jump, int _period, int endTime, std::vector<double> const& _values);
	void set(int _jump, int _period, int endTime, double _value);
	std::vector<double> get(int _jump, int _period, int _endTime);
};

struct MonitorUpdate_ThiefJump
{
	std::map<int, ThiefJump_Period> m_monitorUpdate;
	void set(int _monitor, int _jump, int _period, int endTime, std::vector<double> const& _values);
	void set(int _monitor, int _jump, int _period, int endTime, double _value);
	std::vector<double> get(int _monitor, int _jump, int _period, int _endTime);
};

class BoxPlotFile
{
public:
	MonitorUpdate_ThiefJump m_values;
public:

	void add(int _monitorUpdate, int _thiefJump, int _period, int _endTime, double _value);
	void add(int _monitorUpdate, int _thiefJump, int _period, int _endTime, std::vector<double> const& _values);

	void printOnFile(std::string const& _filename);

	void printOnNewFile(std::string const& filename, std::string const& filenameVec);
};

#endif