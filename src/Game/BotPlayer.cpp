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

BotPlayer::~BotPlayer() {
	if (mRunning) {
		mThread.detach();
		mRunning = false;
	}
}
void BotPlayer::update(sf::Time dt) {
	if (mGameHandler.getTurn() == getColor() && mGameHandler.status() == GameLogic::OnGoing &&
	    !mRunning) {
		mRunning = true;
		mThread = std::thread(&BotPlayer::makeMove, this);
	}
}

void BotPlayer::handleEvent(const sf::Event& event) {}

void BotPlayer::makeMove() {
	Move move = getOptimizeMove();
	std::cout << "Best move: " << move.from() << ' ' << move.to() << '\n';
	if (!mRunning || mGameHandler.status() != GameLogic::OnGoing)
		return;
	mGameHandler.handleMove(move);
	mRunning = false;
	mThread.detach();
}

Move BotPlayer::getOptimizeMove() {
	int depth = mGameHandler.getRemainingTime(getColor()) > 100.f ? 3 : 2;
	int extra = mGameHandler.getRemainingTime(getColor()) > 30.f ? 3 : 2;
	Move move = Engine::getBestMove(static_cast<GameLogic>(mGameHandler), depth, extra);
	return move;
}