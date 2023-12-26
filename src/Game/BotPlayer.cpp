//
// Created by MINH NHUT on 11/21/2023.
//

#include "BotPlayer.hpp"
#include "Game/Logic/Engine.hpp"
#include "Template/Random.hpp"

#include <SFML/System/Sleep.hpp>

#include <iostream>

BotPlayer::BotPlayer(GameHandler& gameHandler, int color)
    : Player(gameHandler, color), mStatus(Waiting), mMove(0, 0) {
	setName("Computer");
}

BotPlayer::~BotPlayer() {
	if (mStatus == Thinking) {
		mThread.detach();
	}
}
void BotPlayer::update(sf::Time dt) {
	if (mGameHandler.mLogic->status() != GameLogic::OnGoing) return;
	if (mGameHandler.mLogic->getTurn() == getColor()) {
		if (mStatus == Waiting) {
			mThread = std::thread(&BotPlayer::makeMove, this);
		} else if (mStatus == Finished) {
			mThread.join();
			mGameHandler.handleMove(mMove);
			mStatus = Waiting;
		}
	}
}

void BotPlayer::handleEvent(const sf::Event& event) {}

void BotPlayer::makeMove() {
	assert(mStatus != Thinking);
	mStatus = Thinking;
	mMove = getOptimizeMove();
	std::cout << "Best move: " << mMove.from() << ' ' << mMove.to() << '\n';
	mStatus = Finished;
}

Move BotPlayer::getOptimizeMove() {
	int depth = mGameHandler.mLogic->getRemainingTime(getColor()) > 100.f ? 3 : 2;
	int extra = 2;
	GameLogic::Ptr logic(mGameHandler.mLogic->clone());
	Move move = Engine::getBestMove(*logic, depth, extra);
	return move;
}