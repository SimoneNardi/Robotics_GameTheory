#include "Style.h"
#include "Identifier.h"
#include "ConfigurationFile.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <memory>

//////////////////////////////////////////////////////////////////////////
std::string getPlayerSymbolResourceName(PlayerType _pType)
{
	std::string filename = g_configFolder;
	switch(_pType)
	{
	case Guard:
		filename += "icons/guard.png";
		break;
	case Thief:
		filename += "icons/Spy.png";
		break;
	case UnknownPlayer:
		filename += "icons/unknown.png";
		break;
	default:
		throw std::exception("Unable to find Symbol Resource Name For Player Symbol!");
	}
	return filename;
}

//////////////////////////////////////////////////////////////////////////
std::string getTargetSymbolResourceName()
{
	std::string filename = g_configFolder;
	filename += "icons/Treasure.png";
	return filename;
}

//////////////////////////////////////////////////////////////////////////
std::string getSinkSymbolResourceName()
{
	std::string filename = g_configFolder;
	filename += "icons/Sink.png";
	return filename;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
Style::Style() : m_owner(), m_position(nullptr) {}

//////////////////////////////////////////////////////////////////////////
Style::Style(std::shared_ptr<sf::Vector2i> _pos, Identifier _owner) : m_owner(_owner), m_position(_pos) {}

//////////////////////////////////////////////////////////////////////////
std::shared_ptr<sf::Vector2i> Style::getPosition() {return m_position;}

///
bool Style::isUsed() {return m_owner.isValid();}

///
void Style::setOwner(Identifier _owner) {m_owner = _owner;}

///
Identifier Style::getOwner() {return m_owner;}

///
std::shared_ptr<WindowStyle> Style::toWindowStyle() { return std::dynamic_pointer_cast<WindowStyle>( this->shared_from_this() ); }
std::shared_ptr<PlayerStyle> Style::toPlayerStyle() { return std::dynamic_pointer_cast<PlayerStyle>( this->shared_from_this() ); }
std::shared_ptr<TargetStyle> Style::toTargetStyle() { return std::dynamic_pointer_cast<TargetStyle>( this->shared_from_this() ); }
std::shared_ptr<GridStyle> Style::toGridStyle() { return std::dynamic_pointer_cast<GridStyle>( this->shared_from_this() ); }
<<<<<<< HEAD
std::shared_ptr<SinkStyle> Style::toSinkStyle() { return std::dynamic_pointer_cast<SinkStyle>( this->shared_from_this() ); }
=======
>>>>>>> origin/Pareto_Efficient

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

///
WindowStyle::WindowStyle() : Style(), m_size(nullptr), m_name("") {}

///
WindowStyle::WindowStyle(std::shared_ptr<sf::Vector2i> _pos, std::shared_ptr<sf::Vector2u> _size, std::string _name, Identifier _owner)
	: Style(_pos, _owner), m_size(_size), m_name(_name) {}

///
Type WindowStyle::getType() {return Window;}

///
std::shared_ptr<sf::Vector2u> WindowStyle::getSize() {return m_size;}

///
std::string WindowStyle::getName() {return m_name;}

///
void WindowStyle::print(std::ostream& _stream)
{
	_stream << "Window" << "\t";
	if(m_position)
	{
		_stream << m_position->x << "\t" << m_position->y << "\t";
		if(m_size)
		{
			_stream << m_size->x << "\t" << m_size->y << "\t";
			if(!m_name.empty())
			{
				_stream << m_name;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
SymbolStyle::SymbolStyle(std::shared_ptr<sf::CircleShape> _symbol) : m_symbol(_symbol) {}

//////////////////////////////////////////////////////////////////////////
SymbolStyle::SymbolStyle() : m_symbol(nullptr) 
{}

//////////////////////////////////////////////////////////////////////////
SymbolStyle::SymbolStyle(std::string _symbolResourceName)
{
	std::shared_ptr<sf::CircleShape> shape = std::make_shared<sf::CircleShape>(50);

	if(g_configurationFile)
	{
		std::shared_ptr<GridStyle> l_grid = g_configurationFile->getGridStyle();
		if( l_grid )
		{
			int l_col = _WINDOW_DIM / l_grid->getCol();
			int l_row = _WINDOW_DIM / l_grid->getRow();

			shape = std::make_shared<sf::CircleShape>(l_row < l_col ? l_row : l_col);
		}
	}
	shape->setFillColor(sf::Color::Transparent);
	m_symbol = shape;

	//this->bindTexture(_symbolResourceName);
}


//////////////////////////////////////////////////////////////////////////
void SymbolStyle::updateSymbol(std::shared_ptr<GridStyle> _grid)
{
	if(_grid)
	{
		int l_col = _WINDOW_DIM / (2.*_grid->getCol());
		int l_row = _WINDOW_DIM / (2.*_grid->getRow());
		
		std::shared_ptr<sf::CircleShape> shape = std::make_shared<sf::CircleShape>(l_row < l_col ? l_row : l_col);
		shape->setFillColor(sf::Color::Transparent);
		m_symbol = shape;

		m_step.x = l_col*2.;
		m_step.y = l_row*2.;
	}
}

//////////////////////////////////////////////////////////////////////////
void SymbolStyle::setWinner()
{
	float l_col = float(m_step.x)/4.;
	float l_row = float(m_step.y)/4.;

	// set a 10-pixel wide orange outline
	m_symbol->setOutlineThickness(l_row < l_col ? l_row : l_col);
	m_symbol->setOutlineColor(sf::Color(250, 150, 100));
}

//////////////////////////////////////////////////////////////////////////
void SymbolStyle::resetWinner()
{
	// set a 10-pixel wide orange outline
	m_symbol->setOutlineThickness(0);
}


//////////////////////////////////////////////////////////////////////////
void SymbolStyle::bindPlayerTexture(PlayerType _pType)
{
	std::string l_symbolResourceName = getPlayerSymbolResourceName(_pType);
	return this->bindTexture(l_symbolResourceName);
<<<<<<< HEAD
=======

>>>>>>> origin/Pareto_Efficient
}

//////////////////////////////////////////////////////////////////////////
void SymbolStyle::bindTargetTexture()
{
	std::string l_symbolResourceName = getTargetSymbolResourceName();
	return this->bindTexture(l_symbolResourceName);
}

//////////////////////////////////////////////////////////////////////////
void SymbolStyle::bindSinkTexture()
{
	std::string l_symbolResourceName = getSinkSymbolResourceName();
	return this->bindTexture(l_symbolResourceName);
}

<<<<<<< HEAD
=======

>>>>>>> origin/Pareto_Efficient
//////////////////////////////////////////////////////////////////////////
void SymbolStyle::bindTexture(const std::string &  _symbolResourceName)
{
	m_texture = std::make_shared<sf::Texture>();

	if ( !m_texture->loadFromFile(_symbolResourceName) )
	{
		throw std::exception("No texture file has been found!");
	}

	m_texture->setSmooth(true);

	m_symbol->setFillColor(sf::Color::White);

	// map a textured rectangle to the shape
	sf::Vector2u l_size = m_texture->getSize();
	m_symbol->setTexture( m_texture.get() ); // texture is a sf::Texture
	m_symbol->setTextureRect(sf::IntRect(0, 0, l_size.x, l_size.y));
}

std::shared_ptr<sf::CircleShape> SymbolStyle::getSymbol() {return m_symbol;}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
int updateNumberOfPlayer()
{
	static int l_number_of_player = 0;
	return ++l_number_of_player;
}

///
PlayerStyle::PlayerStyle() : Style(), SymbolStyle(), m_type(UnknownPlayer), m_control(updateNumberOfPlayer()) {}

///
PlayerStyle::PlayerStyle(std::shared_ptr<sf::Vector2i> _pos, std::shared_ptr<sf::CircleShape> _symbol, PlayerType _pType, Identifier _owner)
	: Style(_pos, _owner), SymbolStyle(_symbol), m_type(_pType), m_control(updateNumberOfPlayer()) {}

///
PlayerStyle::PlayerStyle(std::shared_ptr<sf::Vector2i> _pos, std::string _symbolResourceName, PlayerType _pType, Identifier _owner)
	: Style(_pos, _owner), SymbolStyle(_symbolResourceName), m_type(_pType), m_control(updateNumberOfPlayer()) {}

///
PlayerStyle::PlayerStyle(std::shared_ptr<sf::Vector2i> _pos, PlayerType _pType, Identifier _owner)
	: Style(_pos, _owner), SymbolStyle( getPlayerSymbolResourceName(_pType) ), m_type(_pType), m_control(updateNumberOfPlayer()) {}

///
Type PlayerStyle::getType() {return Gamer;}

///
PlayerType PlayerStyle::getPlayerType() 
{
	return m_type;
}

///
void PlayerStyle::print(std::ostream& _stream)
{
	_stream << "Player" << "\t";
	if(m_position)
	{
		switch(m_type)
		{
		case Guard:
			_stream << "G" << "\t";
			break;
		case Thief:
			_stream << "T" << "\t";
			break;
		case UnknownPlayer:
			_stream << "U" << "\t";
			break;
		default:
			std::exception("Unable to serialize player style!");
		}

		_stream << m_position->x << "\t" << m_position->y;
	}
}

//////////////////////////////////////////////////////////////////////////
InputControlStyle PlayerStyle::getControl()
{
	return m_control;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

///
TargetStyle::TargetStyle(std::shared_ptr<sf::Vector2i> _pos, std::shared_ptr<sf::CircleShape> _symbol, Identifier _owner)
	: Style(_pos, _owner), SymbolStyle(_symbol) {}

///
TargetStyle::TargetStyle(std::shared_ptr<sf::Vector2i> _pos, std::string _symbolResourceName, Identifier _owner)
	: Style(_pos, _owner), SymbolStyle(_symbolResourceName) {}

///
TargetStyle::TargetStyle(std::shared_ptr<sf::Vector2i> _pos, Identifier _owner)
	: Style(_pos, _owner), SymbolStyle( getTargetSymbolResourceName() ) {}
///
TargetStyle::TargetStyle() : Style(), SymbolStyle() {}

///
Type TargetStyle::getType() {return Target;}

///
std::shared_ptr<sf::CircleShape> TargetStyle::getSymbol() {return m_symbol;}

///
std::shared_ptr<sf::CircleShape> getWinnerSymbol();

///
void TargetStyle::print(std::ostream& _stream)
{
	_stream << "Target" << "\t";
	if(m_position)
	{
		_stream << m_position->x << "\t" << m_position->y;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

///
SinkStyle::SinkStyle(std::shared_ptr<sf::Vector2i> _pos, std::shared_ptr<sf::CircleShape> _symbol, Identifier _owner)
	: Style(_pos, _owner), SymbolStyle(_symbol) {}

///
SinkStyle::SinkStyle(std::shared_ptr<sf::Vector2i> _pos, std::string _symbolResourceName, Identifier _owner)
	: Style(_pos, _owner), SymbolStyle(_symbolResourceName) {}

///
SinkStyle::SinkStyle(std::shared_ptr<sf::Vector2i> _pos, Identifier _owner)
	: Style(_pos, _owner), SymbolStyle( getSinkSymbolResourceName() ) {}
///
SinkStyle::SinkStyle() : Style(), SymbolStyle() {}

///
Type SinkStyle::getType() {return Sink;}

///
std::shared_ptr<sf::CircleShape> SinkStyle::getSymbol() {return m_symbol;}

///
std::shared_ptr<sf::CircleShape> getWinnerSymbol();

///
void SinkStyle::print(std::ostream& _stream)
{
	_stream << "Sink" << "\t";
	if(m_position)
	{
		_stream << m_position->x << "\t" << m_position->y;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///
GridStyle::GridStyle() : Style(), m_row(0), m_col(0), m_validate() {}

///
GridStyle::GridStyle(int _row, int _col, std::vector<bool> const& _validate) : Style(), m_row(_row), m_col(_col), m_validate(_validate) {}

///
Type GridStyle::getType() {return Board;}

///
int GridStyle::getCol() {return m_col;}
///
int GridStyle::getRow() {return m_row;}

///
void GridStyle::setRow(int _row) {m_row = _row;}
///
void GridStyle::setCol(int _col) {m_col = _col;}

///
void GridStyle::print(std::ostream& _stream)
{
	_stream << "Grid" << "\t";
	if(m_col > 0 && m_row > 0)
	{
		_stream << m_row << "\t" << m_col;
	}
}

//////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& _stream, std::shared_ptr<Style>& _style)
{
	_style->print(_stream);
	return _stream;
}
