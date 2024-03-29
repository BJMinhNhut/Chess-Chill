//
// Created by MINH NHUT on 6/11/2023.
//

#include "State.hpp"
#include "StateStack.hpp"

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts,
                        SoundPlayer& sounds, Settings& settings, GameOptions& options,
                        OldGames& oldGames, PuzzleList& puzzles, Mode& mode)
    : window(&window),
      textures(&textures),
      fonts(&fonts),
      sounds(&sounds),
      settings(&settings),
      options(&options),
      oldGames(&oldGames),
      puzzles(&puzzles),
      mode(&mode) {}

State::State(StateStack& stack, Context context) : mStack(&stack), mContext(context) {}

State::~State() = default;

void State::requestStackPush(States::ID stateID) {
	mStack->pushState(stateID);
}

void State::requestStackPop() {
	mStack->popState();
}

void State::requestStackClear() {
	mStack->clearStates();
}

State::Context State::getContext() const {
	return mContext;
}