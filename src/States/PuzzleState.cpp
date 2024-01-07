//
// Created by MINH NHUT on 1/6/2024.
//

#include "PuzzleState.hpp"
#include "GUI/Button.hpp"
#include "GUI/Label.hpp"
#include "GUI/Sprite.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>

const sf::Vector2f PuzzleState::BOARD_POSITION(262.f, 105.f);

PuzzleState::PuzzleState(StateStack& stack, Context context)
    : State(stack, context),
      mGUIContainer(),
      mStatusContainer(),
      mGame(context, BOARD_POSITION + sf::Vector2f(25.f, 25.f)),
      mPlayer(mGame, !mGame.mLogic->getTurn()),
      mCoolDown(sf::seconds(1.f)),
      currentMove(0),
      mStatus(Puzzle::Unsolved),
      mPuzzle(context.puzzles->getChosenPuzzle()) {
	loadBasicGUI();
	loadNormalGUI();
	loadCorrectGUI();
	loadFailGUI();
}

PuzzleState::~PuzzleState() {
	getContext().puzzles->save();
}

void PuzzleState::draw() {
	auto& window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mGUIContainer);
	window.draw(mStatusContainer[mStatus]);
	mGame.draw();
}

bool PuzzleState::update(sf::Time dt) {
	if (isPlayerTurn()) {
		mPlayer.update(dt);
	} else if (mStatus == Puzzle::Unsolved) {
		if (mCoolDown.asSeconds() < 0.f) {
			mCoolDown = sf::seconds(1.f);
			mGame.handleMove(mPuzzle.getMove(currentMove++));
		} else {
			mCoolDown -= dt;
		}
	}
	mGUIContainer.update(dt);
	mStatusContainer[mStatus].update(dt);
	return true;
}

bool PuzzleState::handleEvent(const sf::Event& event) {
	if (isPlayerTurn()) {
		mPlayer.handleEvent(event);
		if (mGame.mLogic->getTurn() != mPlayer.getColor()) {
			currentMove++;
			updateStatus();
		}
	}
	mGUIContainer.handleEvent(event);
	mStatusContainer[mStatus].handleEvent(event);
	return false;
}

bool PuzzleState::isPlayerTurn() const {
	return mStatus == Puzzle::Unsolved && mGame.mLogic->getTurn() == mPlayer.getColor();
}

void PuzzleState::loadBasicGUI() {
	auto context = getContext();
	auto background = std::make_shared<GUI::Sprite>(context.textures->get(Textures::Background));
	mGUIContainer.pack(background);

	auto backButton =
	    std::make_shared<GUI::Button>(GUI::Button::Back, *context.fonts, *context.textures);
	backButton->setPosition(509.f, 53.f);
	backButton->setCallback([this]() {
		requestStackPop();
		requestStackPush(States::PuzzleMenu);
	});
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
	    GUI::Label::Bold, "Puzzle - Level " + std::to_string(mPuzzle.getId()), *context.fonts);
	titleLabel->setPosition(titleBar->getPosition());
	titleLabel->alignCenter();
	mGUIContainer.pack(titleLabel);

	auto sidePanel = std::make_shared<GUI::Sprite>(context.textures->get(Textures::PuzzlePanel));
	sidePanel->setPosition(1015.f, 324.f);
	mGUIContainer.pack(sidePanel);
}

void PuzzleState::loadNormalGUI() {
	std::string player;
	if (mPlayer.getColor() == Piece::White)
		player = "White to move";
	else
		player = "Black to move";

	auto playerLabel =
	    std::make_shared<GUI::Label>(GUI::Label::Puzzle, player, *getContext().fonts);
	playerLabel->setPosition(1048.f, 418.f);
	mStatusContainer[0].pack(playerLabel);

	auto hintButton = std::make_shared<GUI::Button>(GUI::Button::Menu, *getContext().fonts,
	                                                *getContext().textures);
	hintButton->setPosition(1092.f + 166.f / 2.f, 472.f + 50.f / 2.f);
	hintButton->setText("Hint");
	hintButton->setCallback([this]() {
		//		mGame.handleMove(getContext().puzzle->getMove(currentMove));
	});
	mStatusContainer[0].pack(hintButton);
}

void PuzzleState::loadCorrectGUI() {
	auto playerLabel =
	    std::make_shared<GUI::Label>(GUI::Label::Puzzle, "Correct!", *getContext().fonts);
	playerLabel->setPosition(1101.f, 383.f);
	mStatusContainer[1].pack(playerLabel);

	auto nextButton = std::make_shared<GUI::Button>(GUI::Button::Menu, *getContext().fonts,
	                                                *getContext().textures);
	nextButton->setPosition(1092.f + 166.f / 2.f, 508.f + 50.f / 2.f);
	nextButton->setText("Next level");
	nextButton->setCallback([this]() {
		getContext().puzzles->nextPuzzle();
		requestStackPop();
		if (getContext().puzzles->getChosenPuzzle().getId() < 100)
			requestStackPush(States::Puzzles);
	});
	mStatusContainer[1].pack(nextButton);

	auto tryButton = std::make_shared<GUI::Button>(GUI::Button::Menu, *getContext().fonts,
	                                               *getContext().textures);
	tryButton->setPosition(1092.f + 166.f / 2.f, 443.f + 50.f / 2.f);
	tryButton->setText("Try again");
	tryButton->setCallback([this]() {
		requestStackPop();
		requestStackPush(States::Puzzles);
	});
	mStatusContainer[1].pack(tryButton);
}

void PuzzleState::loadFailGUI() {
	auto playerLabel =
	    std::make_shared<GUI::Label>(GUI::Label::Puzzle, "Incorrect!", *getContext().fonts);
	playerLabel->setPosition(1089.f, 418.f);
	mStatusContainer[2].pack(playerLabel);

	auto tryButton = std::make_shared<GUI::Button>(GUI::Button::Menu, *getContext().fonts,
	                                               *getContext().textures);
	tryButton->setPosition(1092.f + 166.f / 2.f, 472.f + 50.f / 2.f);
	tryButton->setText("Try again");
	tryButton->setCallback([this]() {
		//		mGame.handleMove(getContext().puzzle->getMove(currentMove));
	});
	mStatusContainer[2].pack(tryButton);
}

void PuzzleState::updateStatus() {
	assert(mGame.mLogic->getTurn() != mPlayer.getColor());
	if (mGame.getLastMove() == -1)
		return;
	int from = mGame.getLastMove() & 63;
	int to = (mGame.getLastMove() >> 6) & 63;
	Move lastMove(from, to);
	if (lastMove == mPuzzle.getMove(currentMove - 1)) {
		if (currentMove == mPuzzle.getSolutionSize()) {
			mStatus = Puzzle::Solved;
			mPuzzle.setStatus(Puzzle::Solved);
			getContext().sounds->play(SoundEffect::EndGame);
		} else {
			mStatus = Puzzle::Unsolved;
		}
	} else {
		getContext().sounds->play(SoundEffect::OutOfBound);
		if (mPuzzle.getStatus() == Puzzle::Unsolved) {
			mPuzzle.setStatus(Puzzle::Failed);
		}
		mStatus = Puzzle::Failed;
	}
}
