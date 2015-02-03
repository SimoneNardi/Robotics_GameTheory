#include "ConfigurationFile.h"
#include "Style.h"
#include "Utility.h"
#include <SFML/System.hpp>

#include <iostream>
#include <fstream>

#include <memory>

const std::string g_configFolder = "./Configuration/";

int g_counter = 0;
int _WINDOW_DIM = 700;

std::shared_ptr<ConfigurationFile> g_configurationFile = ConfigurationFile::open("config.dat");

//////////////////////////////////////////////////////////////////////////
void ConfigurationFile::addStyle(std::shared_ptr<Style> _style)
{
	Lock lock(m_mutex); // mutex.lock()
	m_style.push_back(_style);
} // mutex.unlock()

//////////////////////////////////////////////////////////////////////////
void ConfigurationFile::clean()
{
	std::vector< std::shared_ptr<Style> > l_styleList;
	for(size_t i = 0; i < m_style.size(); ++i)
	{
		std::shared_ptr<WindowStyle> l_style = m_style[i]->toWindowStyle();
		if(l_style)
			l_styleList.push_back(l_style);
	}
	m_style = l_styleList;
}

bool ConfigurationFile::areThereWindowStyle()
{
	for(size_t i = 0; i < m_style.size(); ++i)
	{
		std::shared_ptr<WindowStyle> l_style = m_style[i]->toWindowStyle();
		if(l_style)
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
void ConfigurationFile::readFromPhotoFile( std::string &_filename)
{
	Lock lock(m_mutex); // mutex.lock()

	m_filename = _filename;

	this->clean();

	std::string l_sep("\t ");
	std::fstream file;
	file.open(_filename);
	if( file.is_open() )
	{
		if( !this->areThereWindowStyle() )
		{
			std::shared_ptr<sf::Vector2i> l_position = nullptr;
			std::shared_ptr<sf::Vector2u> l_size = nullptr;
			std::string l_name = "";
			int l_xpos = 118;
			int l_ypos = 107;
			l_position = std::make_shared<sf::Vector2i>(l_xpos, l_ypos);
			int l_xsize = 500;
			int l_ysize = 437;
			l_size = std::make_shared<sf::Vector2u>(l_xsize, l_ysize);
			l_name = "Photo Configuration";
			std::shared_ptr<Style> l_wstyle  = std::make_shared<WindowStyle>(l_position, l_size, l_name);
			this->addStyle(l_wstyle);
		}

		std::string l_line;
		while (std::getline(file, l_line))
		{
			std::vector<std::string> l_token;
			tokenize(l_line, l_sep, l_token);
			if(l_token.empty())
				continue;

			std::shared_ptr<Style> l_style = nullptr;
			
			if(l_token[0] == "T")
			{
				std::shared_ptr<sf::Vector2i> l_position = nullptr;
				PlayerType l_pType = UnknownPlayer;

				l_pType = Thief;
				int l_xpos = atoi(l_token[1].c_str());
				int l_ypos = atoi(l_token[2].c_str());
				l_position = std::make_shared<sf::Vector2i>(l_xpos, l_ypos);
				l_style = std::make_shared<PlayerStyle>(l_position, l_pType);
			}
			if(l_token[0] == "G")
			{
				std::shared_ptr<sf::Vector2i> l_position = nullptr;
				PlayerType l_pType = UnknownPlayer;

				l_pType = Guard;
				int l_xpos = atoi(l_token[1].c_str());
				int l_ypos = atoi(l_token[2].c_str());
				l_position = std::make_shared<sf::Vector2i>(l_xpos, l_ypos);
				l_style = std::make_shared<PlayerStyle>(l_position, l_pType);
			}
			else if(l_token[0] == "col")
			{
				int l_col(atoi(l_token[1].c_str()));

				std::getline(file, l_line);
				tokenize(l_line, l_sep, l_token);
				if(l_token.empty())
					continue;

				int l_row(atoi(l_token[1].c_str()));

				std::getline(file, l_line);
				tokenize(l_line, l_sep, l_token);
				if(l_token.empty())
					continue;

				if(l_row == l_col)
					_WINDOW_DIM = l_row * 10;

				std::vector<bool> l_validation;
				for(auto j = 0; j < l_token.size(); ++j)
					l_validation.push_back( atoi(l_token[j].c_str()) > 0 ? true : false);
								
				l_style = std::make_shared<GridStyle>(l_row, l_col, l_validation);
			}

			if(l_style)
				this->addStyle(l_style);
		}
	}
	file.close();

	this->initSymbols();
}

//////////////////////////////////////////////////////////////////////////
void ConfigurationFile::readFromFile( std::string &_filename)
{
	Lock lock(m_mutex); // mutex.lock()

	std::string l_sep("\t");
	std::fstream file;
	file.open(_filename);
	if( file.is_open() )
	{
		std::string l_line;
		while (std::getline(file, l_line))
		{
			std::vector<std::string> l_token;
			tokenize(l_line, l_sep, l_token);
			if(l_token.empty())
				continue;

			std::shared_ptr<Style> l_style = nullptr;

			if(l_token[0] == "Window")
				// Window Position, Size and Name
			{
				std::shared_ptr<sf::Vector2i> l_position = nullptr;
				std::shared_ptr<sf::Vector2u> l_size = nullptr;
				std::string l_name = "";

				if(l_token.size() > 2)
				{
					int l_xpos = atoi(l_token[1].c_str());
					int l_ypos = atoi(l_token[2].c_str());

					l_position = std::make_shared<sf::Vector2i>(l_xpos, l_ypos);
				}

				if(l_token.size() > 4)
				{

					int l_xsize = atoi(l_token[3].c_str());
					int l_ysize = atoi(l_token[4].c_str());

					l_size = std::make_shared<sf::Vector2u>(l_xsize, l_ysize);
				}

				if(l_token.size() > 5)
					l_name = l_token[5];

				l_style = std::make_shared<WindowStyle>(l_position, l_size, l_name);

			}
			else if(l_token[0] == "Target")
			{
				std::shared_ptr<sf::Vector2i> l_position = nullptr;

				if(l_token.size() > 2)
				{
					int l_xpos = atoi(l_token[1].c_str());
					int l_ypos = atoi(l_token[2].c_str());

					l_position = std::make_shared<sf::Vector2i>(l_xpos, l_ypos);
				}

				l_style = std::make_shared<TargetStyle>(l_position);
			}
			else if(l_token[0] == "Player")
			{
				std::shared_ptr<sf::Vector2i> l_position = nullptr;
				PlayerType l_pType = UnknownPlayer;

				if(l_token.size() > 1)
				{
					if(l_token[1] == "G")
						l_pType = Guard;
					else if(l_token[1] == "T")
						l_pType = Thief;
				}

				if(l_token.size() > 3)
				{
					int l_xpos = atoi(l_token[2].c_str());
					int l_ypos = atoi(l_token[3].c_str());

					l_position = std::make_shared<sf::Vector2i>(l_xpos, l_ypos);
				}

				l_style = std::make_shared<PlayerStyle>(l_position, l_pType);
			}
			else if(l_token[0] == "Grid")
			{
				int l_row(0), l_col(0);

				if(l_token.size() > 2)
				{
					l_row = atoi(l_token[1].c_str());
					l_col = atoi(l_token[2].c_str());
				}

				if(l_row == l_col)
					_WINDOW_DIM = l_row * 10;

				l_style = std::make_shared<GridStyle>(l_row, l_col);
			}

			if(l_style)
				this->addStyle(l_style);
		}
	}
	file.close();

	this->initSymbols();

} // mutex.unlock()

//////////////////////////////////////////////////////////////////////////
void ConfigurationFile::initSymbols()
{
	std::shared_ptr<GridStyle> l_grid = this->getGridStyle();
	if(l_grid)
	{
		for(size_t i = 0; i < m_style.size(); ++i)
		{
			switch(m_style[i]->getType())
			{
			case Gamer:
				m_style[i]->toPlayerStyle()->updateSymbol(l_grid);
				break;
			case Target:
				m_style[i]->toTargetStyle()->updateSymbol(l_grid);
				break;
			default:
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void ConfigurationFile::writeOnFile(std::string& _filename)
{
	Lock lock(m_mutex); // mutex.lock()
	std::ofstream l_outfile(_filename);
	for(size_t i = 0; i < m_style.size(); ++i)
	{
		//if(!m_style[i]->isUsed())
		//	continue;

		l_outfile << m_style[i] << std::endl;
		l_outfile.flush();
	}
	l_outfile.close();
}// mutex.unlock()

//////////////////////////////////////////////////////////////////////////
ConfigurationFile::ConfigurationFile(std::string const &_filename)
	: m_style(), m_filename(g_configFolder + _filename), m_mutex()
{}

//////////////////////////////////////////////////////////////////////////
std::string ConfigurationFile::getFileName()
{
	return m_filename;
}

//////////////////////////////////////////////////////////////////////////
ConfigurationFile::~ConfigurationFile()
{
	this->writeOnFile(m_filename);
}

//////////////////////////////////////////////////////////////////////////
std::shared_ptr<ConfigurationFile> ConfigurationFile::open(std::string const &filename)
{
	if(!g_inputControlInitialized)
		g_inputControlInitialized = initGlobalInputControl();

	static std::shared_ptr<ConfigurationFile> l_file = std::make_shared<ConfigurationFile>(filename);
	if(l_file->m_style.empty())
	{
		l_file->readFromFile(l_file->getFileName());
	}
	return l_file;
}

//////////////////////////////////////////////////////////////////////////
std::shared_ptr<WindowStyle> ConfigurationFile::getWindowStyle()
{
	for(size_t i = 0; i < m_style.size(); ++i)
	{
		if(m_style[i]->isUsed())
			continue;

		std::shared_ptr<WindowStyle> l_style = m_style[i]->toWindowStyle();
		if(l_style)
			return l_style;
	}
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
void ConfigurationFile::updateWindowStyle(Identifier _id, std::shared_ptr<WindowStyle> _style)
{
	for(size_t i = 0; i < m_style.size(); ++i)
	{
		if( m_style[i]->getOwner().equals(_id) )
		{
			_style->setOwner(_id);
			m_style[i] = _style;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
int ConfigurationFile::getNumberPlayerStyle()
{
	int tot = 0;
	for(size_t i = 0; i < m_style.size(); ++i)
	{
		std::shared_ptr<PlayerStyle> l_style = m_style[i]->toPlayerStyle();
		if(l_style)
			tot +=1;
	}
	return tot;
}

//////////////////////////////////////////////////////////////////////////
std::shared_ptr<PlayerStyle> ConfigurationFile::getPlayerStyle()
{
	for(size_t i = 0; i < m_style.size(); ++i)
	{
		if(m_style[i]->isUsed())
			continue;

		std::shared_ptr<PlayerStyle> l_style = m_style[i]->toPlayerStyle();
		if(l_style)
			return l_style;
	}
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
void ConfigurationFile::updatePlayerStyle(Identifier _id, std::shared_ptr<PlayerStyle> _style)
{
	for(size_t i = 0; i < m_style.size(); ++i)
	{
		if( m_style[i]->getOwner().equals(_id) )
		{
			_style->setOwner(_id);
			m_style[i] = _style;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
std::shared_ptr<TargetStyle> ConfigurationFile::getTargetStyle()
{
	for(size_t i = 0; i < m_style.size(); ++i)
	{
		if(m_style[i]->isUsed())
			continue;

		std::shared_ptr<TargetStyle> l_style = m_style[i]->toTargetStyle();
		if(l_style)
			return l_style;
	}
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
void ConfigurationFile::updateTargetStyle(Identifier _id, std::shared_ptr<TargetStyle> _style)
{
	for(size_t i = 0; i < m_style.size(); ++i)
	{
		if( m_style[i]->getOwner().equals(_id) )
		{
			_style->setOwner(_id);
			m_style[i] = _style;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
std::shared_ptr<GridStyle> ConfigurationFile::getGridStyle()
{
	for(size_t i = 0; i < m_style.size(); ++i)
	{
		if(m_style[i]->isUsed())
			continue;

		std::shared_ptr<GridStyle> l_style = m_style[i]->toGridStyle();
		if(l_style)
			return l_style;
	}
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
void ConfigurationFile::updateGridStyle(Identifier _id, std::shared_ptr<GridStyle> _style)
{
	for(size_t i = 0; i < m_style.size(); ++i)
	{
		if( m_style[i]->getOwner().equals(_id) )
		{
			_style->setOwner(_id);
			m_style[i] = _style;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void ConfigurationFile::updateStyle(std::shared_ptr<Style> _style)
{
	if(!_style->isUsed())
		this->addStyle(_style);
	else
	{
		for(size_t i = 0; i < m_style.size(); ++i)
		{
			if( m_style[i]->getOwner().equals( _style->getOwner() ) )
			{
				m_style[i] = _style;
			}
		}
	}
}