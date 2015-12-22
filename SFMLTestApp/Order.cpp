#include "Order.h"
#include "Player.h"
#include "Report.h"

//////////////////////////////////////////////////////////////////////////
bool PlayerOrder::isAble(std::shared_ptr<Player> _player)
{
	bool res = m_moved.find(_player) != m_moved.end();
	if(res)
		return false;
	else
		m_moved.insert(_player);
	return true;
}

//////////////////////////////////////////////////////////////////////////
void PlayerOrder::reset(int num, std::shared_ptr<Report> _report)
{
	if(num <= m_moved.size())
	{
		for(std::set< std::shared_ptr<Player> >::iterator it = m_moved.begin(); it != m_moved.end(); ++it)
		{
			std::shared_ptr<Player> l_player = *it;
			l_player->updatePosition();
			_report->update(l_player);
		}		
		m_moved.clear();
	}
}

//////////////////////////////////////////////////////////////////////////
void PlayerOrder::showUnavailable()
{
	for(std::set< std::shared_ptr<Player> >::iterator it = m_moved.begin(); it != m_moved.end(); ++it)
	{
		std::shared_ptr<Player> l_player = *it;
		l_player->setAvailable(false);
	}
}
