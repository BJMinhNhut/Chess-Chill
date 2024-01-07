//
// Created by MINH NHUT on 1/5/2024.
//

#include "PuzzleMenuState.hpp"
#include "GUI/Button.hpp"
#include "GUI/Label.hpp"
#include "GUI/Sprite.hpp"
#include "Game/GameSaver.hpp"
#include "Template/Constants.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <filesystem>

const int PuzzleMenuState::PAGE_MAX = 20;
const float PuzzleMenuState::PANEL_INDENT_X = 190.f;
const float PuzzleMenuState::PANEL_INDENT_Y = 160.f;

PuzzleMenuState::PuzzleMenuState(StateStack& stack, Context context)
    : State(stack, context), mGUIContainer(), mPage(0), mPageContainer(), mPageLabel(nullptr) {
	loadBasicGUI();
	loadCurrentPage();
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
#ifdef SFML_DEBUG
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::X) {
			resetStatus();
			getContext().puzzles->save();
			requestStackPop();
			requestStackPush(States::PuzzleMenu);
		}
	}
#endif
	return false;
}

void PuzzleMenuState::resetStatus() {
	for (int i = 0; i < getContext().puzzles->size(); i++) {
		getContext().puzzles->getPuzzle(i).setStatus(Puzzle::Unsolved);
	}
}

void PuzzleMenuState::loadBasicGUI() {
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
	return ((int)getContext().puzzles->size() + PAGE_MAX - 1) / PAGE_MAX;
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

GUI::Button::Type PuzzleMenuState::getButtonType(Puzzle::Status status) {
	switch (status) {
		case Puzzle::Status::Solved:
			return GUI::Button::PuzzleSolved;
		case Puzzle::Status::Failed:
			return GUI::Button::PuzzleFailed;
		default:
			return GUI::Button::Puzzle;
	}
}

void PuzzleMenuState::loadPanel(int order, int puzzleID) {
	const Puzzle& puzzle = getContext().puzzles->getPuzzle(puzzleID);
	auto button = std::make_shared<GUI::Button>(getButtonType(puzzle.getStatus()),
	                                            *getContext().fonts, *getContext().textures);
	button->setText(std::to_string(puzzleID + 1));

	int row = order / 5, col = order % 5;
	button->setPosition(360.f + 60.f + (float)col * PANEL_INDENT_X,
	                    150.f + 60.f + (float)row * PANEL_INDENT_Y);
	button->setCallback([this, puzzleID]() {
		getContext().puzzles->setPuzzle(puzzleID);
		*getContext().mode = Context::Puzzles;
		requestStackPop();
		requestStackPush(States::Puzzles);
	});
	mPageContainer.pack(button);
}

void PuzzleMenuState::loadCurrentPage() {
	mPageContainer.clear();
	assert(mPage >= 0 && mPage < getNumPages());
	int start = mPage * PAGE_MAX;
	int end = std::min(start + PAGE_MAX, (int)getContext().puzzles->size());
	for (int i = start; i < end; i++) {
		loadPanel(i - start, i);
	}
	mPageLabel->setText(std::to_string(mPage + 1) + "/" + std::to_string(getNumPages()));
	mPageLabel->alignCenter();
}