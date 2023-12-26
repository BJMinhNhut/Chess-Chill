//
// Created by MINH NHUT on 11/21/2023.
//

#include "BotPlayer.hpp"
#include "Game/Logic/Engine.hpp"
#include "Template/Random.hpp"

#include <SFML/System/Sleep.hpp>

#include <iostream>

BotPlayer::BotPlayer(GameHandler& gameHandler, int color)
    : Player(gameHandler, color), mStatus(Waiting), mMove(0, 0), mThinkFlag(false) {
	setName("Computer");
}

BotPlayer::~BotPlayer() {
	if (mThinkFlag) stopEngine();
}
void BotPlayer::update(sf::Time dt) {
	if (mGameHandler.mLogic->status() != GameLogic::OnGoing) {
		if (mThinkFlag) stopEngine();
		return;
	}
	if (mGameHandler.mLogic->getTurn() == getColor()) {
		if (mStatus == Waiting) {
			assert(!mThinkFlag);
			mStatus = Thinking;
			mThinkFlag = true;
			mThread = std::thread(&BotPlayer::makeMove, this);
		} else if (mStatus == Finished) {
			mThread.join();
			mGameHandler.handleMove(mMove);
			mStatus = Waiting;
		}
	}
}

void BotPlayer::stopEngine() {
	std::cout << "Stop engine\n";
	mThinkFlag = false;
	sf::sleep(sf::milliseconds(100));
	mThread.join();
}

void BotPlayer::handleEvent(const sf::Event& event) {}

void BotPlayer::makeMove() {
	mMove = getOptimizeMove();
	mThinkFlag = false;
	mStatus = Finished;
}

Move BotPlayer::getOptimizeMove() {
	int depth = mGameHandler.mLogic->getRemainingTime(getColor()) > 100.f ? 3 : 2;
	int extra = 2;
	GameLogic::Ptr logic(mGameHandler.mLogic->clone());
	Move move = Engine::getBestMove(*logic, mThinkFlag, depth, extra);
	std::cout << "Best move: " << move.from() << ' ' << move.to() << '\n';
	return move;
}