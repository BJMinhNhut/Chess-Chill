//
// Created by MINH NHUT on 11/20/2023.
//

#include "Player.hpp"

Player::Player(GameHandler& gameHandler, int color)
    : mGameHandler(gameHandler), mColor(color), mName() {}

int Player::getColor() const {
	return mColor;
}

std::string Player::getName() const {
	return mName;
}