//
// Created by MINH NHUT on 10/23/2023.
//

#include "GameState.hpp"
#include "GUI/Button.hpp"
#include "GUI/Sprite.hpp"
#include "Template/ResourceHolder.hpp"
#include "Template/Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

GameState::GameState(StateStack& stack, Context context)
    : State(stack, context),
      mGame(*context.window, *context.textures, *context.fonts, *context.sounds) {}

void GameState::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	mGame.draw();
}

bool GameState::update(sf::Time dt) {
	mGame.update(dt);
	return false;
}

bool GameState::handleEvent(const sf::Event& event) {
	mGame.handleEvent(event);
	if (event.type == sf::Event::KeyReleased) {
		if (event.key.code == sf::Keyboard::Escape) {
			requestStackPop();
		} else if (event.key.code == sf::Keyboard::R) {
			requestStackPop();
			requestStackPush(States::Game);
		}
	}
	return false;
}