#include "WindowEventHandler.h"
#include "ConfigurationFile.h"
#include "Style.h"
#include "Challenge.h"
#include <SFML/Graphics.hpp>

//////////////////////////////////////////////////////////////////////////
WindowEventHandler::WindowEventHandler(std::string name, Identifier _index) 
	: m_window(nullptr), m_brain(nullptr), m_running(false), m_name(name), m_position(nullptr), m_created(false), m_index(_index), m_size(nullptr)
{}

//////////////////////////////////////////////////////////////////////////
void WindowEventHandler::setWindowStyle_fromConfigurationFile()
{
	std::shared_ptr<WindowStyle> l_style = g_configurationFile->getWindowStyle();
	if(l_style)
	{
		m_position = l_style->getPosition();
		m_size = l_style->getSize();
		m_name = l_style->getName();
		l_style->setOwner(m_index);
	}
}

//////////////////////////////////////////////////////////////////////////
void WindowEventHandler::Initialize()
{
	if(!m_window)
		return;

	setWindowStyle_fromConfigurationFile();

	if(m_position)
		m_window->setPosition(*m_position);

	if(m_size)
		m_window->setSize(*m_size);

	if( !m_name.empty() )
		m_window->setTitle(m_name);

	m_window->setVerticalSyncEnabled(true);
}

//////////////////////////////////////////////////////////////////////////
void WindowEventHandler::Create()
{
	m_created = false;
	if(!m_window)
	{
		sf::ContextSettings settings;
		settings.antialiasingLevel = 8;

		m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(_WINDOW_DIM, _WINDOW_DIM), m_name, sf::Style::Default, settings);
		Initialize();
	}
	m_created = true;
}

//////////////////////////////////////////////////////////////////////////
void WindowEventHandler::EventLoop()
{
	Create();

	m_running = true;
	while (m_window->isOpen())
	{
		sf::Event event;
		while (m_window->pollEvent(event))
		{
			switch(event.type)
			{
			case sf::Event::Closed:
				this->close();
				break;
			case sf::Event::KeyReleased:
			case sf::Event::JoystickMoved:
			case sf::Event::JoystickButtonReleased:
				this->m_brain->update(m_index, event);
				break;
			case sf::Event::JoystickConnected:
			case sf::Event::JoystickDisconnected:
				//initGlobalInputControl();
			default:
				break;
			}
		}

		m_window->clear();

		Draw();

		m_window->display();
	}
	m_running=false;
}

//////////////////////////////////////////////////////////////////////////
void WindowEventHandler::close()
{
	// save position and size in the configuration file:
	std::shared_ptr<WindowStyle> l_style = std::make_shared<WindowStyle>(
		std::make_shared<sf::Vector2i>( m_window->getPosition() ),
		std::make_shared<sf::Vector2u>( m_window->getSize() ),
		m_name,
		m_index);

	g_configurationFile->updateStyle(l_style);

	m_window->close();
}

//////////////////////////////////////////////////////////////////////////
void WindowEventHandler::Draw()
{
	static bool l_background = false;
	static sf::VertexArray l_back(sf::Quads, 4);
	if(!l_background)
	{
		sf::Vector2u l_size = m_window->getSize();
		l_back.setPrimitiveType(sf::Quads);
		l_back[0].position = sf::Vector2f(0,0);
		l_back[1].position = sf::Vector2f(_WINDOW_DIM,0);
		l_back[2].position = sf::Vector2f(_WINDOW_DIM,_WINDOW_DIM);
		l_back[3].position = sf::Vector2f(0,_WINDOW_DIM);

		// define the color of the triangle's points
		sf::Color up  (181, 230, 29);
		sf::Color down(0, 255, 128);

		l_back[0].color = up;
		l_back[1].color = up;
		l_back[2].color = down;
		l_back[2].color = down;

		l_background = true;
		this->m_window->draw(l_back);
	}
	else
	{
#if 0
		sf::Vector2u l_size = m_window->getSize();
		static sf::VertexArray l_test(sf::Quads, 4);
		l_test.setPrimitiveType(sf::Quads);
		l_test[0].position = sf::Vector2f(_WINDOW_DIM/2.-_WINDOW_DIM/4.,_WINDOW_DIM/2.-_WINDOW_DIM/4.);
		l_test[1].position = sf::Vector2f(_WINDOW_DIM/2.+_WINDOW_DIM/4.,_WINDOW_DIM/2.-_WINDOW_DIM/4.);
		l_test[2].position = sf::Vector2f(_WINDOW_DIM/2.+_WINDOW_DIM/4.,_WINDOW_DIM/2.+_WINDOW_DIM/4.);
		l_test[3].position = sf::Vector2f(_WINDOW_DIM/2.-_WINDOW_DIM/4.,_WINDOW_DIM/2.+_WINDOW_DIM/4.);

		// define the color of the triangle's points
		sf::Color up  (0,0, 255);

		l_test[0].color = up;
		l_test[1].color = up;
		l_test[2].color = up;
		l_test[2].color = up;

		this->m_window->draw(l_back);
		this->m_window->draw(l_test);
#else
		this->m_window->draw(l_back);
#endif
	}

	if(m_brain)
		m_brain->draw( this->shared_from_this() );
}

//////////////////////////////////////////////////////////////////////////
void WindowEventHandler::setBrain(std::shared_ptr<Brain> _brain)
{
	m_brain = _brain;
}