#pragma once
#include <set>

class Player;
class Report;

class PlayerOrder
{
	std::set< std::shared_ptr<Player> > m_moved;

public:
	bool isAble(std::shared_ptr<Player> _player);
	void reset(int num, std::shared_ptr<Report> _report);
	void showUnavailable();
};

extern const int g_NumberOfPlayer;
extern PlayerOrder g_playerOrder;