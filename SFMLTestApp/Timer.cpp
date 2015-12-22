#include "Timer.h"
#include "ConfigurationFile.h"
#include "WindowEventHandler.h"

#include <time.h>

//////////////////////////////////////////
// Timer class implementation:

//////////////////////////////////////////////////////////////////////////
Timer::Timer()
{
	resetted = true;
	running = false;
	beg = 0;
	end = 0;
}

//////////////////////////////////////////////////////////////////////////
void Timer::start()
{
	if(! running)
	{
		if(resetted)
			beg = (unsigned long) clock();
		else
			beg -= end - (unsigned long) clock();
		running = true;
		resetted = false;
	}
}

//////////////////////////////////////////////////////////////////////////
void Timer::stop()
{
	if(running) 
	{
		end = (unsigned long) clock();
		running = false;
	}
}

//////////////////////////////////////////////////////////////////////////
void Timer::reset() 
{
	bool wereRunning = running;
	if(wereRunning)
		stop();
	resetted = true;
	beg = 0;
	end = 0;
	if(wereRunning)
		start();
}

//////////////////////////////////////////////////////////////////////////
bool Timer::isRunning() 
{
	return running;
}

//////////////////////////////////////////////////////////////////////////
unsigned long Timer::getTime() 
{
	if(running)
		return ((unsigned long) clock() - beg) / CLOCKS_PER_SEC;
	else
		return end - beg;
}

//////////////////////////////////////////////////////////////////////////
bool Timer::isOver(unsigned long seconds) 
{
	return seconds >= getTime();
}

sf::Font g_font;

//////////////////////////////////////////////////////////////////////////
void Timer::draw(std::shared_ptr<WindowEventHandler> _window)
{
#if 0
	static bool l_init = false;
	if(!l_init)
	{
		std::string name = "fonts/arial.ttf";
		if (!g_font.loadFromFile(g_configFolder + name))
		{
			return;
		}

		// select the font
		m_text.setFont(g_font); // font is a sf::Font

		// set the character size
		m_text.setCharacterSize(20); // in pixels, not points!
		
		// set the string to display
		m_text.setString("0 sec");

		// set the color
		m_text.setColor(sf::Color::Red);
		l_init = true;
	}

	_window->m_window->draw(m_text);
#endif
	return;
}

//Timer::~Timer()
//{
//	g_font.cleanup();
//}
