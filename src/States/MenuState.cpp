//
// Created by MINH NHUT on 6/27/2023.
//

#include "MenuState.hpp"
#include "GUI/Button.hpp"
#include "Template/ResourceHolder.hpp"
#include "Template/Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

MenuState::MenuState(StateStack& stack, Context context)
    : State(stack, context),
      mGUIContainer(),
      mBackgroundSprite(context.textures->get(Textures::Background)),
      mTitle(context.textures->get(Textures::Title)) {

	Utility::centerOrigin(mTitle);
	mTitle.setPosition(context.window->getSize().x / 2.f, context.window->getSize().y / 4.f);

	auto startButton =
	    std::make_shared<GUI::Button>(GUI::Button::Menu, *context.fonts, *context.textures);
	startButton->setPosition(587.f + 166.f / 2.f, 526.f + 25.f);
	startButton->setText("New Game");
	startButton->setCallback([this]() { requestStackPush(States::GameOptions); });

	auto historyButton =
	    std::make_shared<GUI::Button>(GUI::Button::Menu, *context.fonts, *context.textures);
	historyButton->setPosition(587.f + 166.f / 2.f, 594.f + 25.f);
	historyButton->setText("History");
	historyButton->setCallback([this]() { requestStackPush(States::History); });

	auto puzzlesButton =
	    std::make_shared<GUI::Button>(GUI::Button::Menu, *context.fonts, *context.textures);
	puzzlesButton->setPosition(587.f + 166.f / 2.f, 662.f + 25.f);
	puzzlesButton->setText("Puzzles");
	puzzlesButton->setCallback([this]() { requestStackPush(States::PuzzleMenu); });

	auto settingsButton =
	    std::make_shared<GUI::Button>(GUI::Button::Menu, *context.fonts, *context.textures);
	settingsButton->setPosition(846.f + 166.f / 2.f, 526.f + 25.f);
	settingsButton->setText("Settings");
	settingsButton->setCallback([this]() { requestStackPush(States::Settings); });

	auto aboutButton =
	    std::make_shared<GUI::Button>(GUI::Button::Menu, *context.fonts, *context.textures);
	aboutButton->setPosition(846.f + 166.f / 2.f, 594.f + 25.f);
	aboutButton->setText("About");
	aboutButton->setCallback([this]() { requestStackPush(States::About); });

	auto exitButton =
	    std::make_shared<GUI::Button>(GUI::Button::Menu, *context.fonts, *context.textures);
	exitButton->setPosition(846.f + 166.f / 2.f, 662.f + 25.f);
	exitButton->setText("Exit");
	exitButton->setCallback([this]() { requestStackPop(); });

	mGUIContainer.pack(startButton);
	mGUIContainer.pack(historyButton);
	mGUIContainer.pack(puzzlesButton);
	mGUIContainer.pack(exitButton);
	mGUIContainer.pack(aboutButton);
	mGUIContainer.pack(settingsButton);
}

void MenuState::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mTitle);
	window.draw(mGUIContainer);
}

bool MenuState::update(sf::Time dt) {
	return true;
}

bool MenuState::handleEvent(const sf::Event& event) {
	mGUIContainer.handleEvent(event);
	return false;
}