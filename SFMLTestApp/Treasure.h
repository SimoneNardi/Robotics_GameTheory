#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

class TargetStyle;
class WindowEventHandler;

class Treasure
{
protected:
	///
	std::shared_ptr<TargetStyle> m_style;
	
	std::shared_ptr<sf::CircleShape> m_symbol;

public:
	///
	Treasure( std::shared_ptr<TargetStyle> _style );

	///
	void bindTexture();

	///
	void draw(std::shared_ptr<WindowEventHandler> _window);

	///
	sf::Vector2f getPosition();
};