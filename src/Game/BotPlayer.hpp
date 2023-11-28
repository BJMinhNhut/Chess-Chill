//
// Created by MINH NHUT on 11/21/2023.
//

#ifndef CHESS_CHILL_BOTPLAYER_HPP
#define CHESS_CHILL_BOTPLAYER_HPP

#include "Player.hpp"

#include <thread>

class BotPlayer : public Player {
   public:
	BotPlayer(GameHandler & gameHandler, int color);

	void update(sf::Time dt) override;
	void handleEvent(const sf::Event &event) override;

   private:
	void makeMove();

   private:
	std::thread mThread;
	bool mRunning;
};

#endif  //CHESS_CHILL_BOTPLAYER_HPP
