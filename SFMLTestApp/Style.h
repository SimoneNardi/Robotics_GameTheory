#pragma once

#include "Identifier.h"
#include "InputControl.h"
#include <SFML/Graphics.hpp>

#include <memory>

//////////////////////////////////////////////////////////////////////////
enum Type
{
	Window,
	Gamer,
	Target,
	Board,
	Sink
};

enum PlayerType
{
	Guard,
	Thief,
	UnknownPlayer
};

class WindowStyle;
class PlayerStyle;
class TargetStyle;
class GridStyle;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class Style : public std::enable_shared_from_this<Style>
{
protected:
	std::shared_ptr<sf::Vector2i> m_position;

	Identifier m_owner;
public:
	///
	Style();

	///
	Style(std::shared_ptr<sf::Vector2i> _pos, Identifier _owner = 0);

	///
	virtual Type getType() = 0;

	///
	std::shared_ptr<sf::Vector2i> getPosition();

	///
	bool isUsed();

	///
	void setOwner(Identifier _owner);

	///
	Identifier getOwner();

	///
	std::shared_ptr<WindowStyle> toWindowStyle();
	std::shared_ptr<PlayerStyle> toPlayerStyle();
	std::shared_ptr<TargetStyle> toTargetStyle();
	std::shared_ptr<GridStyle> toGridStyle();

	virtual void print(std::ostream& _stream) = 0;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class WindowStyle : public Style
{
protected:
	std::shared_ptr<sf::Vector2u> m_size;
	std::string m_name;
public:
	///
	WindowStyle();

	///
	WindowStyle(std::shared_ptr<sf::Vector2i> _pos, std::shared_ptr<sf::Vector2u> _size, std::string _name, Identifier _owner = 0);

	///
	Type getType();

	///
	std::shared_ptr<sf::Vector2u> getSize();

	///
	std::string getName();

	///
	virtual void print(std::ostream& _stream);
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class SymbolStyle
{
protected:
	std::shared_ptr<sf::CircleShape> m_symbol;
	sf::Vector2i m_step;
	
	std::shared_ptr<sf::Texture> m_texture;

public:
	///
	SymbolStyle();

	///
	SymbolStyle(std::shared_ptr<sf::CircleShape> _symbol);

	///
	SymbolStyle(std::string _symbolResourceName);

	///
	std::shared_ptr<sf::CircleShape> getSymbol();

	///
	void bindTexture( const std::string & _symbolResourceName);

	///
	void bindPlayerTexture(PlayerType _pType);

	///
	void bindTargetTexture();

	///
	void bindSinkTexture();

	///
	void updateSymbol(std::shared_ptr<GridStyle> _grid);

	///
	sf::Vector2i getStep() {return m_step;}

	///
	void setWinner();

	///
	void resetWinner();

};

extern int g_available_joystick;
extern int g_available_keyboard;
extern bool g_inputControlInitialized;
bool initGlobalInputControl();

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class PlayerStyle : public Style, public SymbolStyle
{
public:
	PlayerType m_type;
	
	InputControlStyle m_control;

public:
	///
	PlayerStyle();

	///
	PlayerStyle(std::shared_ptr<sf::Vector2i> _pos, std::shared_ptr<sf::CircleShape> _symbol, PlayerType _pType= UnknownPlayer, Identifier _owner = 0);

	///
	PlayerStyle(std::shared_ptr<sf::Vector2i> _pos, std::string _symbolResourceName, PlayerType _pType= UnknownPlayer, Identifier _owner = 0);

	///
	PlayerStyle(std::shared_ptr<sf::Vector2i> _pos, PlayerType _pType= UnknownPlayer, Identifier _owner = 0);

	///
	Type getType();

	///
	PlayerType getPlayerType();

	///
	virtual void print(std::ostream& _stream);

	///
	InputControlStyle getControl();
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class TargetStyle : public Style, public SymbolStyle
{
public:
	///
	TargetStyle(std::shared_ptr<sf::Vector2i> _pos, std::shared_ptr<sf::CircleShape> _symbol, Identifier _owner = 0);

	///
	TargetStyle(std::shared_ptr<sf::Vector2i> _pos, std::string _symbolResourceName, Identifier _owner = 0);

	///
	TargetStyle(std::shared_ptr<sf::Vector2i> _pos, Identifier _owner = 0);
	///
	TargetStyle();

	///
	Type getType();

	///
	std::shared_ptr<sf::CircleShape> getSymbol();

	///
	virtual void print(std::ostream& _stream);
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class SinkStyle : public Style, public SymbolStyle
{
public:
	///
	SinkStyle (std::shared_ptr<sf::Vector2i> _pos, std::shared_ptr<sf::CircleShape> _symbol, Identifier _owner = 0);

	///
	SinkStyle (std::shared_ptr<sf::Vector2i> _pos, std::string _symbolResourceName, Identifier _owner = 0);

	///
	SinkStyle (std::shared_ptr<sf::Vector2i> _pos, Identifier _owner = 0);
	///
	SinkStyle ();

	///
	Type getType();

	///
	std::shared_ptr<sf::CircleShape> getSymbol();

	///
	virtual void print(std::ostream& _stream);
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class GridStyle : public Style
{
protected:
	int m_row;
	int m_col;

	std::vector<bool> m_validate;
public:
	///
	GridStyle();

	///
	GridStyle(int _row, int _col, std::vector<bool> const& _validate = std::vector<bool>());

	///
	Type getType();

	///
	int getCol();

	///
	int getRow();

	///
	void setRow(int _row);

	///
	void setCol(int _col);

	///
	virtual void print(std::ostream& _stream);
};

std::ostream& operator<<(std::ostream& _stream, std::shared_ptr<Style>& _style);
