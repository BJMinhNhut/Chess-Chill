//
// Created by MINH NHUT on 12/28/2023.
//

#include "ReviewState.hpp"

#include "GUI/Button.hpp"
#include "GUI/Label.hpp"
#include "GUI/Sprite.hpp"
#include "Game/GameHandler.hpp"
#include "Template/Constants.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <filesystem>
#include <iostream>

const sf::Vector2f ReviewState::BOARD_POSITION(262.f, 105.f);

ReviewState::ReviewState(StateStack& stack, Context context)
    : State(stack, context),
      mGUIContainer(),
      mReviewContainer(),
      mWinner(nullptr),
      mDescription(nullptr),
      mClock(),
      mPlayerLabel(),
      mMoveList(nullptr),
      mGame(context, BOARD_POSITION + sf::Vector2f(25.f, 25.f)) {
//	if (context.options->getTime() > 0) {
//		mGame.mLogic->setClock(sf::seconds(context.options->getTime()),
//		                       sf::seconds(context.options->getIncrement()));
//	}

	loadBasicGUI();
	loadGameGUI();
	loadControllerGUI();
}

void ReviewState::draw() {
	auto& window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mGUIContainer);
	window.draw(mReviewContainer);
	mGame.draw();
}

bool ReviewState::update(sf::Time dt) {
	//	mGame.update(dt);
	updateMoveList();
	return true;
}

bool ReviewState::handleEvent(const sf::Event& event) {
	mGUIContainer.handleEvent(event);
	mReviewContainer.handleEvent(event);
	return false;
}

void ReviewState::loadBasicGUI() {
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
		requestStateClear();
		requestStackPush(States::Menu);
	});
	mGUIContainer.pack(homeButton);

	auto titleBar = std::make_shared<GUI::Sprite>(context.textures->get(Textures::TitleBar));
	titleBar->centerOrigin();
	titleBar->setPosition(800.f, 53.f);
	mGUIContainer.pack(titleBar);

	auto titleLabel = std::make_shared<GUI::Label>(GUI::Label::Bold, getTitle(), *context.fonts);
	titleLabel->setPosition(titleBar->getPosition());
	titleLabel->alignCenter();
	mGUIContainer.pack(titleLabel);
}

void ReviewState::loadGameGUI() {
	auto context = getContext();
	auto panels = std::make_shared<GUI::Sprite>(context.textures->get(Textures::GameGUI));
	panels->setPosition(BOARD_POSITION);
	mGUIContainer.pack(panels);

	mPlayerLabel[0] = std::make_shared<GUI::Label>(GUI::Label::Main, "White", *context.fonts);
	mPlayerLabel[0]->setPosition(1040.f, 696.f + 8.f);
	mGUIContainer.pack(mPlayerLabel[0]);

	mPlayerLabel[1] = std::make_shared<GUI::Label>(GUI::Label::Main, "Black", *context.fonts);
	mPlayerLabel[1]->setPosition(1040.f, 217.f + 8.f);
	mGUIContainer.pack(mPlayerLabel[1]);

	mClock[0] =
	    std::make_shared<GUI::Label>(GUI::Label::Clock, "--:--", *context.fonts, Constants::mBlack);
	mClock[0]->setPosition(1015.f + 320.f / 2, 259.f + 80.f / 2);
	mClock[0]->alignCenter();
	mGUIContainer.pack(mClock[0]);

	mClock[1] =
	    std::make_shared<GUI::Label>(GUI::Label::Clock, "--:--", *context.fonts, Constants::mBlack);
	mClock[1]->setPosition(1015.f + 320.f / 2, 605.f + 80.f / 2);
	mClock[1]->alignCenter();
	mGUIContainer.pack(mClock[1]);

	mMoveList = std::make_shared<GUI::Label>(GUI::Label::Mono, "", *context.fonts);
	mMoveList->setPosition(1040.f, 424.f);
	mGUIContainer.pack(mMoveList);
}

void ReviewState::rotateBoard() {
	mGame.rotateBoard();

	sf::Vector2f clockPosition = mClock[0]->getPosition();
	mClock[0]->setPosition(mClock[1]->getPosition());
	mClock[1]->setPosition(clockPosition);

	sf::Vector2f playerLabelPosition = mPlayerLabel[0]->getPosition();
	mPlayerLabel[0]->setPosition(mPlayerLabel[1]->getPosition());
	mPlayerLabel[1]->setPosition(playerLabelPosition);
}

void ReviewState::loadControllerGUI() {
	auto rotateButton = std::make_shared<GUI::Button>(GUI::Button::Rotate, *getContext().fonts,
	                                                  *getContext().textures);
	rotateButton->setPosition(1269.f + 50.f / 2, 366.f + 40.f / 2);
	rotateButton->setCallback([this]() { rotateBoard(); });
	mGUIContainer.pack(rotateButton);

	auto firstButton = std::make_shared<GUI::Button>(GUI::Button::First, *getContext().fonts,
	                                                 *getContext().textures);
	firstButton->setPosition(1036.f + 40.f / 2, 366.f + 40.f / 2);
	firstButton->setCallback([this]() { mGame.loadFirstMove(); });
	mReviewContainer.pack(firstButton);

	auto previousButton = std::make_shared<GUI::Button>(GUI::Button::Previous, *getContext().fonts,
	                                                    *getContext().textures);
	previousButton->setPosition(1076.f + 40.f / 2, 366.f + 40.f / 2);
	previousButton->setCallback([this]() { mGame.loadPreviousMove(); });
	mReviewContainer.pack(previousButton);

	auto nextButton = std::make_shared<GUI::Button>(GUI::Button::Next, *getContext().fonts,
	                                                *getContext().textures);
	nextButton->setPosition(1116.f + 40.f / 2, 366.f + 40.f / 2);
	nextButton->setCallback([this]() { mGame.loadNextMove(); });
	mReviewContainer.pack(nextButton);

	auto lastButton = std::make_shared<GUI::Button>(GUI::Button::Last, *getContext().fonts,
	                                                *getContext().textures);
	lastButton->setPosition(1156.f + 40.f / 2, 366.f + 40.f / 2);
	lastButton->setCallback([this]() { mGame.loadLastMove(); });
	mReviewContainer.pack(lastButton);
}

void ReviewState::updateMoveList() {
	int start_id;
	std::vector<std::string> list = mGame.getLatestMoves(6, start_id);
	std::stringstream display;
	std::string line;
	for (int i = 0; i < list.size(); i++) {
		if (i % 2 == 0)
			line += std::to_string((start_id + i) / 2 + 1) + ". ";
		while (line.length() < 4)
			line += " ";
		line += list[i];
		while (line.length() < 16)
			line += " ";
		if (i % 2 == 1) {
			line += "\n";
			display << line;
			line.clear();
		}
	}
	if (!line.empty())
		display << line;
	mMoveList->setText(display.str());
}

std::string ReviewState::getClockString(float time) {
	int minutes = static_cast<int>(time) / 60;
	int seconds = static_cast<int>(time) % 60;
	if (minutes * 60 + seconds > 3600)
		return "--:--";
	return (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") +
	       std::to_string(seconds);
}

std::string ReviewState::getTitle() const {
	return "Review - " + getContext().options->getStringType() + " - " + getContext().options->getStringMode();

}
