#pragma once

#include "Mutex.h"
#include <SFML/System.hpp>
#include <fstream>
#include <iostream>

extern int g_counter;

//////////////////////////////////////////////////////////////////////////
inline int getNextFreeInteger()
{
	g_counter++;
	return g_counter;
}

//////////////////////////////////////////////////////////////////////////
class Identifier
{
	int m_id;

public:
	Identifier(int _id = 0) : m_id(_id)
	{}

	static Identifier getNext()
	{
		return Identifier( getNextFreeInteger() );
	}

	bool isValid()
	{
		return m_id > 0;
	}

	bool equals(Identifier _other)
	{
		return m_id == _other.m_id;
	}

	bool operator<(const Identifier& other) const
	{
		return m_id < other.m_id;
	}

	friend std::ostream& operator<<(std::ostream& out, const Identifier& a);
};

//////////////////////////////////////////////////////////////////////////
inline std::ostream& operator<<(std::ostream& out, const Identifier& a)
{
	out << a.m_id;
	return out ;
}