//
// Created by MINH NHUT on 12/28/2023.
//

#include "HistoryState.hpp"
#include "GUI/Button.hpp"
#include "GUI/Label.hpp"
#include "GUI/Sprite.hpp"
#include "Game/GameSaver.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <filesystem>
#include <iostream>

HistoryState::HistoryState(StateStack& stack, Context context)
    : State(stack, context), mGUIContainer(), mList(), mPage(0) {
	loadBasicGUI();
	loadHistoryList();
	loadHistoryGUI();
}

void HistoryState::draw() {
	auto& window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mGUIContainer);
}

bool HistoryState::update(sf::Time dt) {
	return true;
}

bool HistoryState::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::C) {
			requestStackPush(States::Review);
		}
	}
	mGUIContainer.handleEvent(event);
	return false;
}

void HistoryState::loadBasicGUI() {
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

	auto titleLabel = std::make_shared<GUI::Label>(GUI::Label::Bold, "Old games", *context.fonts);
	titleLabel->setPosition(titleBar->getPosition());
	titleLabel->alignCenter();
	mGUIContainer.pack(titleLabel);
}

void HistoryState::loadHistoryList() {
	getContext().oldGames->load();
	std::cout << "History list size: " << getContext().oldGames->getSize() << '\n';
}

void HistoryState::loadHistoryGUI() {
	getContext().oldGames->setIndex(0);
	std::string path = getContext().oldGames->getPath();
	GameSaver saver(path);
	std::cout << "Path " << path << '\n';

	GameOptions options = saver.getOptions();
	std::cout << "Game mode: " << options.getStringMode() << '\n';
	std::cout << "Game type: " << options.getStringType() << '\n';
	std::cout << "Game time: " << options.getStringTime() << '\n';
	std::cout << "Game result: " << saver.getResult() << '\n';
	std::cout << "Snapshot size: " << saver.size() << '\n';
}