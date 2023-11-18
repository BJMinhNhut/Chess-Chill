//
// Created by MINH NHUT on 11/19/2023.
//

#include "Clock.hpp"

Clock::Clock(): mActive(false), mTime(sf::seconds(0)) {}

void Clock::update(sf::Time dt) {
	if (mActive && mTime > sf::seconds(0))
		mTime -= dt;
}

void Clock::set(sf::Time time) {
	mActive = true;
	mTime = time;
}

bool Clock::isTimeOut() const {
	return mActive && mTime <= sf::seconds(0);
}

float Clock::get() const {
	if (!mActive)
		return 9999.f;
	return mTime.asSeconds();
}