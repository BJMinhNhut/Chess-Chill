//
// Created by MINH NHUT on 1/5/2024.
//

#include "PuzzleMenuState.hpp"
#include "GUI/Button.hpp"
#include "GUI/HistoryPanel.hpp"
#include "GUI/Label.hpp"
#include "GUI/Sprite.hpp"
#include "Game/GameSaver.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <filesystem>
#include <iostream>

const int PuzzleMenuState::PAGE_MAX = 6;
//const float PuzzleMenuState::PANEL_INDENT_X = 115.f;

PuzzleMenuState::PuzzleMenuState(StateStack& stack, Context context)
    : State(stack, context), mGUIContainer(), mPage(0), mPageContainer(), mPageLabel(nullptr) {
	context.oldGames->load();
	loadBasicGUI();
//	loadHistoryList();
//	loadCurrentPage();
}

void PuzzleMenuState::draw() {
	auto& window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mGUIContainer);
	window.draw(mPageContainer);
}

bool PuzzleMenuState::update(sf::Time dt) {
	mGUIContainer.update(dt);
	mPageContainer.update(dt);
	return true;
}

bool PuzzleMenuState::handleEvent(const sf::Event& event) {
	mGUIContainer.handleEvent(event);
	mPageContainer.handleEvent(event);
	return false;
}

void PuzzleMenuState::loadBasicGUI() {
	auto context = getContext();
	auto background = std::make_shared<GUI::Sprite>(context.textures->get(Textures::Background));
	mGUIContainer.pack(background);

	auto backButton =
	    std::make_shared<GUI::Button>(GUI::Button::Back, *context.fonts, *context.textures);
	backButton->setPosition(509.f, 53.f);
	backButton->setCallback([this]() {
		requestStackPop();
	});
	mGUIContainer.pack(backButton);

	auto homeButton =
	    std::make_shared<GUI::Button>(GUI::Button::Home, *context.fonts, *context.textures);
	homeButton->setPosition(1063.f + 54.f / 2, 53.f);
	homeButton->setCallback([this]() {
		requestStateClear();
		requestStackPush(States::Menu);
	});
	mGUIContainer.pack(homeButton);

	auto titleBar = std::make_shared<GUI::Sprite>(context.textures->get(Textures::TitleBar));
	titleBar->centerOrigin();
	titleBar->setPosition(800.f, 53.f);
	mGUIContainer.pack(titleBar);

	auto titleLabel = std::make_shared<GUI::Label>(GUI::Label::Bold, "Puzzles", *context.fonts);
	titleLabel->setPosition(titleBar->getPosition());
	titleLabel->alignCenter();
	mGUIContainer.pack(titleLabel);

	auto nextPageButton =
	    std::make_shared<GUI::Button>(GUI::Button::Forward, *context.fonts, *context.textures);
	nextPageButton->setPosition(870.f, 786.f + 54.f / 2);
	nextPageButton->setCallback([this]() { nextPage(); });
	mGUIContainer.pack(nextPageButton);

	auto previousPageButton =
	    std::make_shared<GUI::Button>(GUI::Button::Forward, *context.fonts, *context.textures);
	previousPageButton->setPosition(730.f, 786.f + 54.f / 2);
	previousPageButton->setRotation(180.f);
	previousPageButton->setCallback([this]() { previousPage(); });
	mGUIContainer.pack(previousPageButton);

	mPageLabel = std::make_shared<GUI::Label>(GUI::Label::Main, "1/1", *context.fonts);
	mPageLabel->setPosition(800.f, 786.f + 54.f / 2);
	mPageLabel->alignCenter();
	mGUIContainer.pack(mPageLabel);
}

int PuzzleMenuState::getNumPages() const {
	return 1;
//	return ((int)getContext().oldGames->getSize() + PAGE_MAX - 1) / PAGE_MAX;
}

void PuzzleMenuState::nextPage() {
	if (mPage >= getNumPages() - 1)
		return;
	mPage++;
	loadCurrentPage();
}

void PuzzleMenuState::previousPage() {
	if (mPage <= 0)
		return;
	mPage--;
	loadCurrentPage();
}

//void HistoryState::loadPanel(int id, int pathID, const std::string& path) {
//	GameSaver saver(path);
//	//	std::cout << "Path " << path << '\n';
//
//	GameOptions options = saver.getOptions();
//	//	std::cout << "Game mode: " << options.getStringMode() << '\n';
//	//	std::cout << "Game type: " << options.getStringType() << '\n';
//	//	std::cout << "Game time: " << options.getStringTime() << '\n';
//	//	std::cout << "Game result: " << saver.getResult() << '\n';
//	//	std::cout << "Snapshot size: " << saver.size() << '\n';
//
//	auto historyPanel =
//	    std::make_shared<GUI::HistoryPanel>(*getContext().fonts, *getContext().textures);
//	historyPanel->setMode(options.getStringMode());
//	historyPanel->setType(options.getStringType());
//	historyPanel->setTime(options.getStringTime());
//	historyPanel->setResult(saver.getResult());
//	historyPanel->setDate(saver.getDate());
//	historyPanel->setPosition(482.f, 104.f + (float)id * PANEL_INDENT);
//	historyPanel->setCallback([&, pathID]() {
//		getContext().oldGames->setIndex(pathID);
//		requestStackPush(States::Review);
//	});
//	mPageContainer.pack(historyPanel);
//}

void PuzzleMenuState::loadCurrentPage() {
//	mPageContainer.clear();
//	assert(mPage >= 0 && mPage < getNumPages());
//	int start = mPage * PAGE_MAX;
//	int end = std::min(start + PAGE_MAX, (int)getContext().oldGames->getSize());
//	for (int i = start; i < end; i++) {
//		loadPanel(i - start, i, getContext().oldGames->getPathByID(i));
//	}
//	mPageLabel->setText(std::to_string(mPage + 1) + "/" + std::to_string(getNumPages()));
//	mPageLabel->alignCenter();
}