//
// Created by MINH NHUT on 6/11/2023.
//

#ifndef NATUREOFCODE_STATESTACK_HPP
#define NATUREOFCODE_STATESTACK_HPP

#include "State.hpp"
#include "StateIdentifiers.hpp"

#include "SFML/System/NonCopyable.hpp"
#include "SFML/System/Time.hpp"

#include <functional>
#include <map>
#include <vector>

class StateStack : private sf::NonCopyable {
   public:
	enum Action {
		Push,
		Pop,
		Clear,
	};

   public:
	explicit StateStack(State::Context context);

	template <typename T>
	void registerState(States::ID stateID);

	void update(sf::Time dt);
	void draw();
	void handleEvent(const sf::Event& event);

	void pushState(States::ID stateID);
	void popState();
	void clearStates();

    [[nodiscard]] bool isEmpty() const;

   private:
	State::Ptr createState(States::ID stateID);
	void applyPendingChanges();

   private:
	struct PendingChange {
		Action action;
		States::ID stateID;

		explicit PendingChange(Action action, States::ID stateID = States::None);
	};

   private:
	std::vector<State::Ptr> mStack;
	std::vector<PendingChange> mPendingList;
	State::Context mContext;
	std::map<States::ID, std::function<State::Ptr()>> mFactories;
};

// register state on-demand
template <typename T>
void StateStack::registerState(States::ID stateID) {
	mFactories[stateID] = [this] () {
		return State::Ptr(new T(*this, mContext));
	};
}

#endif  //NATUREOFCODE_STATESTACK_HPP
