//
// Created by MINH NHUT on 11/19/2023.
//

#include "Clock.hpp"

Clock::Clock(): mActive(false), mTime(), mIncrement() {
	mTime[0] = mTime[1] = sf::Time::Zero;
	mIncrement = sf::Time::Zero;
}

void Clock::update(bool turn, sf::Time dt) {
	if (mActive && mTime[turn] > sf::seconds(0))
		mTime[turn] -= dt;
}

void Clock::set(sf::Time time, sf::Time increment) {
	mActive = true;
	mTime[0] = mTime[1] = time;
	mIncrement = increment;
}

void Clock::increment(bool turn) {
	mTime[turn] += mIncrement;
}

bool Clock::isTimeOut(bool turn) const {
	return mActive && mTime[turn] <= sf::seconds(0);
}

float Clock::get(bool turn) const {
	if (!mActive)
		return 9999.f;
	return mTime[turn].asSeconds();
}