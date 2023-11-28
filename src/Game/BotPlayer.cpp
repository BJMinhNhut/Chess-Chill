//
// Created by MINH NHUT on 11/21/2023.
//

#include "BotPlayer.hpp"
#include "Engine.hpp"
#include "Template/Random.hpp"

#include <SFML/System/Sleep.hpp>

#include <iostream>

BotPlayer::BotPlayer(GameHandler& gameHandler, int color)
    : Player(gameHandler, color), mRunning(false) {
	setName("Computer");
}

void BotPlayer::update(sf::Time dt) {
	if (mGameHandler.getTurn() == getColor()) {
		if (!mRunning) {
			mRunning = true;
			mThread = std::thread(&BotPlayer::makeMove, this);
		}
	}
}

void BotPlayer::handleEvent(const sf::Event& event) {}

void BotPlayer::makeMove() {
	int move = Engine::getBestMove(static_cast<GameLogic>(mGameHandler), 3);
	//		std::vector<int> moves = mGameHandler.getLegalMoves();
	//		int move = moves[Random::getInt(0, moves.size() - 1)];
	int from = move & 0x3F;
	int to = (move >> 6) & 0x3F;
	std::cout << "Best move: " << from << ' ' << to << '\n';
	mGameHandler.handleMove(from, to);
	mRunning = false;
	mThread.detach();
}