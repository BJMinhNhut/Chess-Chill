//
// Created by MINH NHUT on 11/19/2023.
//

#include "GameOptionsState.hpp"
#include "GUI/Button.hpp"
#include "GUI/Label.hpp"
#include "GUI/Sprite.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

GameOptionsState::GameOptionsState(StateStack& stack, Context context)
    : State(stack, context),
      mGUIContainer() {
	loadBasicGUI();
}

void GameOptionsState::loadBasicGUI() {
	auto context = getContext();
	auto background = std::make_shared<GUI::Sprite>(context.textures->get(Textures::Background));
	mGUIContainer.pack(background);

	auto backButton =
	    std::make_shared<GUI::Button>(GUI::Button::Back, *context.fonts, *context.textures);
	backButton->setPosition(509.f, 53.f);
	backButton->setCallback([this]() {
		getContext().settings->save();
		requestStackPop();
	});
	mGUIContainer.pack(backButton);

	auto homeButton =
	    std::make_shared<GUI::Button>(GUI::Button::Home, *context.fonts, *context.textures);
	homeButton->setPosition(1063.f + 54.f / 2, 53.f);
	homeButton->setCallback([this]() {
		getContext().settings->save();
		requestStateClear();
		requestStackPush(States::Menu);
	});
	mGUIContainer.pack(homeButton);

	auto titleBar = std::make_shared<GUI::Sprite>(context.textures->get(Textures::TitleBar));
	titleBar->centerOrigin();
	titleBar->setPosition(800.f, 53.f);
	mGUIContainer.pack(titleBar);

	auto titleLabel = std::make_shared<GUI::Label>(GUI::Label::Bold, "Game Options", *context.fonts);
	titleLabel->setPosition(titleBar->getPosition());
	titleLabel->alignCenter();
	mGUIContainer.pack(titleLabel);

	auto optionsPanel =
	    std::make_shared<GUI::Sprite>(context.textures->get(Textures::GameOptionsPanel));
	optionsPanel->setPosition(322.f, 220.f);
	mGUIContainer.pack(optionsPanel);

	auto playButton = std::make_shared<GUI::Button>(GUI::Button::Play, *context.fonts, *context.textures);
	playButton->setCallback([this]() {
		requestStackPush(States::Game);
	});
	playButton->setPosition(973.f + 306.f/2, 638.f + 82.f/2);
	mGUIContainer.pack(playButton);
}

void GameOptionsState::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mGUIContainer);
}

bool GameOptionsState::update(sf::Time dt) {
	return true;
}

bool GameOptionsState::handleEvent(const sf::Event& event) {
	mGUIContainer.handleEvent(event);
	return false;
}
