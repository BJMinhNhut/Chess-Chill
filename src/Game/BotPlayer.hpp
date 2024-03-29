//
// Created by MINH NHUT on 11/21/2023.
//

#ifndef CHESS_CHILL_BOTPLAYER_HPP
#define CHESS_CHILL_BOTPLAYER_HPP

#include "Player.hpp"

#include <thread>

class BotPlayer : public Player {
   public:
	enum Status {
		Thinking,
		Finished,
		Waiting,
	};
	BotPlayer(GameHandler & gameHandler, int color);
	~BotPlayer() override;

	void update(sf::Time dt) override;
	void handleEvent(const sf::Event &event) override;

   private:
	void stopEngine();
	void makeMove();
	Move getOptimizeMove();

   private:
	bool mThinkFlag;
	std::thread mThread;
	Status mStatus;
	Move mMove;
};

#endif  //CHESS_CHILL_BOTPLAYER_HPP
