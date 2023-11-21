//
// Created by MINH NHUT on 11/21/2023.
//

#include "BotPlayer.hpp"
#include "Template/Random.hpp"

#include <SFML/System/Sleep.hpp>

BotPlayer::BotPlayer(GameHandler & gameHandler, int color) : Player(gameHandler, color) {
	setName("Computer");
}

void BotPlayer::update(sf::Time dt) {
	if (mGameHandler.getTurn() == getColor()) {
		auto list = mGameHandler.getLegalMoves();
		assert(list.size() > 0);
		int move = list[Random::getInt(0, list.size() - 1)];
		mGameHandler.handleMove(move & 63, move >> 6);
	}
}

void BotPlayer::handleEvent(const sf::Event &event) {

}