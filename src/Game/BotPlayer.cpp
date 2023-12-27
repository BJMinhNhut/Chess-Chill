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
	if (mThinkFlag)
		stopEngine();
}
void BotPlayer::update(sf::Time dt) {
	if (mGameHandler.mLogic->status() != GameLogic::OnGoing) {
		if (mThinkFlag)
			stopEngine();
		return;
	}
	if (mGameHandler.mLogic->getTurn() == getColor()) {
		if (mStatus == Waiting && mThinkFlag == false) {
			mStatus = Thinking;
			mThinkFlag = true;
			mThread = std::thread(&BotPlayer::makeMove, this);
		} else if (mStatus == Finished) {
			mThread.join();
			mGameHandler.handleMove(mMove);
			mThinkFlag = false;
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
	mStatus = Finished;
}

Move BotPlayer::getOptimizeMove() {
	int depth = 3, extra = 3;
	float limit = std::max(.01f, std::min(5.f, mGameHandler.mLogic->getRemainingTime(getColor()) / 50.f));

	GameLogic::Ptr logic(mGameHandler.mLogic->clone());
	Move move = Engine::getBestMove(*logic, sf::seconds(limit), mThinkFlag, depth, extra);
	std::cout << "Best move: " << move.from() << ' ' << move.to() << '\n';
	return move;
}