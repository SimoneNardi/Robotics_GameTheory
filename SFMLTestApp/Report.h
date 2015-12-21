#pragma once
#include <SFML/System.hpp>
#include <vector>
#include <map>
#include <string>
#include "Mutex.h"

struct Trajectory 
{
	std::vector<sf::Vector2i> positions;
};

class Player;
class Identifier;
class AbsModel;

//////////////////////////////////////////////////////////////////////////
class Report
{
protected:
	Mutex m_mutex;

	std::string m_filename;

	std::map<Identifier, Trajectory> m_trajectory;

private:

	//////////////////////////////////////////////////////////////////////////
	void readFromFile( std::string &_filename);

	//////////////////////////////////////////////////////////////////////////
	void writeOnFile(std::string& _filename);
public:
	///
	Report(std::string const &_filename);

	///
	void print();

	///
	void update(std::shared_ptr<Player> _player);
};