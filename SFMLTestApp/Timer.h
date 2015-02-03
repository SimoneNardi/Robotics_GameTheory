#pragma once

#include "time.h"
#include <memory>
#include <SFML/Graphics.hpp>

class WindowEventHandler;

class Timer
{
public:
	Timer();
	void           start();
	void           stop();
	void           reset();
	bool           isRunning();
	unsigned long  getTime();
	bool           isOver(unsigned long seconds);
private:
	bool           resetted;
	bool           running;
	unsigned long  beg;
	unsigned long  end;

	sf::Text m_text;
public:
	void draw(std::shared_ptr<WindowEventHandler> _window);
};