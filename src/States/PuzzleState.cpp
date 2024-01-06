//
// Created by MINH NHUT on 1/6/2024.
//

#include "PuzzleState.hpp"
#include "GUI/Button.hpp"
#include "GUI/Label.hpp"
#include "GUI/Sprite.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

const sf::Vector2f PuzzleState::BOARD_POSITION(262.f, 105.f);

PuzzleState::PuzzleState(StateStack& stack, Context context)
    : State(stack, context),
      mGUIContainer(),
      mGame(context, BOARD_POSITION + sf::Vector2f(25.f, 25.f)) {
	loadBasicGUI();
}

void PuzzleState::draw() {
	auto& window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mGUIContainer);
	mGame.draw();
}

bool PuzzleState::update(sf::Time dt) {
	mGUIContainer.update(dt);
	return true;
}

bool PuzzleState::handleEvent(const sf::Event& event) {
	mGUIContainer.handleEvent(event);
	return false;
}

void PuzzleState::loadBasicGUI() {
	auto context = getContext();
	auto background = std::make_shared<GUI::Sprite>(context.textures->get(Textures::Background));
	mGUIContainer.pack(background);

	auto backButton =
	    std::make_shared<GUI::Button>(GUI::Button::Back, *context.fonts, *context.textures);
	backButton->setPosition(509.f, 53.f);
	backButton->setCallback([this]() { requestStackPop(); });
	mGUIContainer.pack(backButton);

	auto homeButton =
	    std::make_shared<GUI::Button>(GUI::Button::Home, *context.fonts, *context.textures);
	homeButton->setPosition(1063.f + 54.f / 2, 53.f);
	homeButton->setCallback([this]() {
		requestStackClear();
		requestStackPush(States::Menu);
	});
	mGUIContainer.pack(homeButton);

	auto titleBar = std::make_shared<GUI::Sprite>(context.textures->get(Textures::TitleBar));
	titleBar->centerOrigin();
	titleBar->setPosition(800.f, 53.f);
	mGUIContainer.pack(titleBar);

	auto titleLabel = std::make_shared<GUI::Label>(
	    GUI::Label::Bold, "Puzzle - Level " + std::to_string(getContext().puzzle->getId()), *context.fonts);
	titleLabel->setPosition(titleBar->getPosition());
	titleLabel->alignCenter();
	mGUIContainer.pack(titleLabel);
}
