//
// Created by MINH NHUT on 6/11/2023.
//

#include "StateStack.hpp"

#include <cassert>

StateStack::StateStack(State::Context context)
    : mStack(), mPendingList(), mContext(context), mFactories() {}

void StateStack::update(sf::Time dt) {
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) {
		if (!(*itr)->update(dt))
			break;
	}

	applyPendingChanges();
}

void StateStack::draw() {
	for (State::Ptr& state : mStack)
		state->draw();
}

/*
iterate the active stack from the end to the beginning, if any of the
states returns false in its handleEvent() method, the loop is immediately ended.
*/
void StateStack::handleEvent(const sf::Event& event) {
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) {
		if (!(*itr)->handleEvent(event))
			break;
	}

	applyPendingChanges();
}

void StateStack::pushState(States::ID stateID) {
	mPendingList.emplace_back(Push, stateID);
}

void StateStack::popState() {
	mPendingList.emplace_back(Pop);
}

void StateStack::clearStates() {
	mPendingList.emplace_back(Clear);
}

bool StateStack::isEmpty() const {
	return mStack.empty();
}

// create new state on the registered states
State::Ptr StateStack::createState(States::ID stateID) {
	auto found = mFactories.find(stateID);
	assert(found != mFactories.end());

	return found->second();
}

void StateStack::applyPendingChanges() {
	for (PendingChange change : mPendingList) {
		switch (change.action) {
			case Push:
				mStack.push_back(createState(change.stateID));
				break;

			case Pop:
				mStack.pop_back();
				break;

			case Clear:
				mStack.clear();
				break;
		}
	}

	mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
    : action(action), stateID(stateID) {}