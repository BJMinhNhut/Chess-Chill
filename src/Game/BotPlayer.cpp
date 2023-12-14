//
// Created by MINH NHUT on 11/21/2023.
//

#include "BotPlayer.hpp"
#include "Engine.hpp"
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
	if (mGameHandler.mLogic.getTurn() == getColor()) {
		if (mStatus == Waiting) {
			mStatus = Thinking;
			mThread = std::thread(&BotPlayer::makeMove, this);
		} else if (mStatus == Finished) {
			mGameHandler.handleMove(mMove);
			mStatus = Waiting;
			mThread.detach();
		}
	}
}

void BotPlayer::handleEvent(const sf::Event& event) {}

void BotPlayer::makeMove() {
	if (mGameHandler.mLogic.status() != GameLogic::OnGoing) {
		mThread.detach();
		mStatus = Waiting;
		return;
	}
	std::cout << "\nSearching for best move...\n";
	mMove = getOptimizeMove();
	std::cout << "Best move: " << mMove.from() << ' ' << mMove.to() << '\n';
	mStatus = Finished;
}

Move BotPlayer::getOptimizeMove() {
	int depth = mGameHandler.mLogic.getRemainingTime(getColor()) > 100.f ? 3 : 2;
	int extra = mGameHandler.mLogic.getRemainingTime(getColor()) > 50.f ? 2 : 1;
	Move move = Engine::getBestMove(mGameHandler.cloneLogic(), depth, extra);
	return move;
}