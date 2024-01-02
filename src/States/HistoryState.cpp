//
// Created by MINH NHUT on 12/28/2023.
//

#include "HistoryState.hpp"
#include "GUI/Button.hpp"
#include "GUI/HistoryPanel.hpp"
#include "GUI/Label.hpp"
#include "GUI/Sprite.hpp"
#include "Game/GameSaver.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <filesystem>
#include <iostream>

const int HistoryState::PAGE_MAX = 6;
const float HistoryState::PANEL_INDENT = 115.f;

HistoryState::HistoryState(StateStack& stack, Context context)
    : State(stack, context), mGUIContainer(), mPage(0) {
	context.oldGames->load();
	loadBasicGUI();
	loadHistoryList();
	loadCurrentPage();
}

void HistoryState::draw() {
	auto& window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mGUIContainer);
	window.draw(mPageContainer);
}

bool HistoryState::update(sf::Time dt) {
	mGUIContainer.update(dt);
	mPageContainer.update(dt);
	return true;
}

bool HistoryState::handleEvent(const sf::Event& event) {
	mGUIContainer.handleEvent(event);
	mPageContainer.handleEvent(event);
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

int HistoryState::getNumPages() const {
	return ((int)getContext().oldGames->getSize() + PAGE_MAX - 1) / PAGE_MAX;
}

void HistoryState::nextPage() {
	if (mPage >= getNumPages() - 1)
		return;
	mPage++;
	loadCurrentPage();
}

void HistoryState::previousPage() {
	if (mPage <= 0)
		return;
	mPage--;
	loadCurrentPage();
}

void HistoryState::loadPanel(int id, int pathID, const std::string& path) {
	GameSaver saver(path);
	//	std::cout << "Path " << path << '\n';

	GameOptions options = saver.getOptions();
	//	std::cout << "Game mode: " << options.getStringMode() << '\n';
	//	std::cout << "Game type: " << options.getStringType() << '\n';
	//	std::cout << "Game time: " << options.getStringTime() << '\n';
	//	std::cout << "Game result: " << saver.getResult() << '\n';
	//	std::cout << "Snapshot size: " << saver.size() << '\n';

	auto historyPanel =
	    std::make_shared<GUI::HistoryPanel>(*getContext().fonts, *getContext().textures);
	historyPanel->setMode(options.getStringMode());
	historyPanel->setType(options.getStringType());
	historyPanel->setTime(options.getStringTime());
	historyPanel->setResult(saver.getResult());
	historyPanel->setDate(saver.getDate());
	historyPanel->setPosition(473.f, 117.f + (float)id * PANEL_INDENT);
	historyPanel->setCallback([&]() {
		getContext().oldGames->setIndex(pathID);
		requestStackPush(States::Review);
	});
	mPageContainer.pack(historyPanel);
}

void HistoryState::loadCurrentPage() {
	mPageContainer.clear();
	assert(mPage >= 0 && mPage < getNumPages());
	int start = mPage * PAGE_MAX;
	int end = std::min(start + PAGE_MAX, (int)getContext().oldGames->getSize());
	for (int i = start; i < end; i++) {
		loadPanel(i - start, i, getContext().oldGames->getPathByID(i));
	}
}