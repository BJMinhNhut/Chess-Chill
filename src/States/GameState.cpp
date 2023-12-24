//
// Created by MINH NHUT on 10/23/2023.
//

#include "GameState.hpp"
#include "GUI/Button.hpp"
#include "GUI/Label.hpp"
#include "GUI/Sprite.hpp"
#include "Game/BotPlayer.hpp"
#include "Game/HumanPlayer.hpp"
#include "Template/Constants.hpp"
#include "Template/ResourceHolder.hpp"
#include "Template/Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Sleep.hpp>

#include <iostream>

const sf::Vector2f GameState::BOARD_POSITION(262.f, 105.f);

GameState::GameState(StateStack& stack, Context context)
    : State(stack, context),
      mGame(context, BOARD_POSITION + sf::Vector2f(25.f, 25.f)),
      mGUIContainer(),
      mWinner(nullptr),
      mDescription(nullptr),
      mPlayers(),
      mClock(),
      mPlayerLabel(),
      mEndGameContainer(),
      mReviewContainer(),
      mReviewMode(false),
      mCoolDown(sf::Time::Zero) {

	if (context.options->getTime() > 0) {
		mGame.mLogic->setClock(sf::seconds(context.options->getTime()),
		                       sf::seconds(context.options->getIncrement()));
	}

	if (context.options->getMode() != GameOptions::AIvAI)
		mPlayers[0] = std::unique_ptr<Player>(new HumanPlayer(mGame, Player::White));
	else
		mPlayers[0] = std::unique_ptr<Player>(new BotPlayer(mGame, Player::White));
	if (context.options->getMode() != GameOptions::PvP)
		mPlayers[1] = std::unique_ptr<Player>(new BotPlayer(mGame, Player::Black));
	else
		mPlayers[1] = std::unique_ptr<Player>(new HumanPlayer(mGame, Player::Black));

	getContext().sounds->play(SoundEffect::StartGame);
	loadBasicGUI();
	loadGameGUI();
	loadControllerGUI();
	loadEndGameGUI();
}

void GameState::loadBasicGUI() {
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

void GameState::loadGameGUI() {
	auto context = getContext();
	auto panels = std::make_shared<GUI::Sprite>(context.textures->get(Textures::GameGUI));
	panels->setPosition(BOARD_POSITION);
	mGUIContainer.pack(panels);

	mPlayerLabel[0] =
	    std::make_shared<GUI::Label>(GUI::Label::Main, mPlayers[0]->getName(), *context.fonts);
	mPlayerLabel[0]->setPosition(1040.f, 696.f + 8.f);
	mGUIContainer.pack(mPlayerLabel[0]);

	mPlayerLabel[1] =
	    std::make_shared<GUI::Label>(GUI::Label::Main, mPlayers[1]->getName(), *context.fonts);
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

	mEvaluation = std::make_shared<GUI::Label>(GUI::Label::Main, "", *context.fonts);
	mEvaluation->setPosition(50.f, 450.f);
	mEvaluation->alignCenter();
	mGUIContainer.pack(mEvaluation);
}

void GameState::rotateBoard() {
	mGame.rotateBoard();

	sf::Vector2f clockPosition = mClock[0]->getPosition();
	mClock[0]->setPosition(mClock[1]->getPosition());
	mClock[1]->setPosition(clockPosition);

	sf::Vector2f playerLabelPosition = mPlayerLabel[0]->getPosition();
	mPlayerLabel[0]->setPosition(mPlayerLabel[1]->getPosition());
	mPlayerLabel[1]->setPosition(playerLabelPosition);
}

void GameState::loadControllerGUI() {
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

void GameState::loadEndGameGUI() {
	auto panel = std::make_shared<GUI::Sprite>(getContext().textures->get(Textures::EndGamePanel));
	panel->setPosition(377.f, 310.f);
	mEndGameContainer.pack(panel);

	auto closeButton = std::make_shared<GUI::Button>(GUI::Button::Close, *getContext().fonts,
	                                                 *getContext().textures);
	closeButton->setPosition(818.f + 50.f / 2, 317.f + 50.f / 2);
	closeButton->setCallback([this]() { mReviewMode = true; });
	mEndGameContainer.pack(closeButton);

	auto newGameButton = std::make_shared<GUI::Button>(GUI::Button::Menu, *getContext().fonts,
	                                                   *getContext().textures);
	newGameButton->setPosition(547.f + 166.f / 2, 476.f + 48.f / 2);
	newGameButton->setText("New Game");
	newGameButton->setCallback([this]() { requestStackPop(); });
	mEndGameContainer.pack(newGameButton);

	auto homeButton = std::make_shared<GUI::Button>(GUI::Button::Menu, *getContext().fonts,
	                                                *getContext().textures);
	homeButton->setPosition(547.f + 166.f / 2, 540.f + 48.f / 2);
	homeButton->setText("Home");
	homeButton->setCallback([this]() {
		requestStateClear();
		requestStackPush(States::Menu);
	});
	mEndGameContainer.pack(homeButton);

	mWinner = std::make_shared<GUI::Label>(GUI::Label::Title, "", *getContext().fonts);
	mWinner->setPosition(474.f + 310.f / 2, 331.f + 102.f / 2);
	mEndGameContainer.pack(mWinner);

	mDescription = std::make_shared<GUI::Label>(GUI::Label::Small, "", *getContext().fonts);
	mDescription->setPosition(563.f + 132.f / 2, 418.f + 32.f / 2);
	mEndGameContainer.pack(mDescription);
}

void GameState::loadResult() {
	if (mGame.mLogic->status() == GameLogic::Checkmate) {
		mWinner->setText(mGame.mLogic->getWinner() + " wins");
		mDescription->setText("by checkmate");
	} else if (mGame.mLogic->status() == GameLogic::Resign) {
		mWinner->setText(mGame.mLogic->getWinner() + " wins");
		mDescription->setText("by resignation");
	} else if (mGame.mLogic->status() == GameLogic::Timeout) {
		mWinner->setText(mGame.mLogic->getWinner() + " wins");
		mDescription->setText("by timeout");
	} else if (mGame.mLogic->status() == GameLogic::Stalemate) {
		mWinner->setText("Draw");
		mDescription->setText("by stalemate");
	} else if (mGame.mLogic->status() == GameLogic::InsufficientMaterial) {
		mWinner->setText("Draw");
		mDescription->setText("by insufficient material");
	} else if (mGame.mLogic->status() == GameLogic::ThreefoldRepetition) {
		mWinner->setText("Draw");
		mDescription->setText("by threefold repetition");
	} else if (mGame.mLogic->status() == GameLogic::FiftyMoveRule) {
		mWinner->setText("Draw");
		mDescription->setText("by fifty move rule");
	} else if (mGame.mLogic->status() == GameLogic::TopOfTheHill) {
		mWinner->setText(mGame.mLogic->getWinner() + " wins");
		mDescription->setText("king reach top of the hill");
	}
	mWinner->alignCenter();
	mDescription->alignCenter();
}

std::string GameState::getClockString(float time) {
	int minutes = static_cast<int>(time) / 60;
	int seconds = static_cast<int>(time) % 60;
	if (minutes * 60 + seconds > 3600)
		return "--:--";
	return (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") +
	       std::to_string(seconds);
}

std::string GameState::getTitle() const {
	return getContext().options->getStringType() + " - " + getContext().options->getStringMode();
}

void GameState::updateClock() {
	mClock[0]->setText(getClockString(mGame.mLogic->getRemainingTime(true)));
	mClock[1]->setText(getClockString(mGame.mLogic->getRemainingTime(false)));
}

void GameState::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mGUIContainer);
	window.draw(mReviewContainer);
	mGame.draw();
	if (mGame.mLogic->isFinished()) {
		if (mWinner->isEmpty()) {
			getContext().sounds->play(SoundEffect::EndGame);
			mCoolDown = sf::milliseconds(2000);
			loadResult();
		}
		if (!mReviewMode && mCoolDown < sf::milliseconds(100))
			window.draw(mEndGameContainer);
	}
}

bool GameState::update(sf::Time dt) {
	if (mGame.mLogic->isFinished()) {
		if (!mReviewMode) {
			if (mCoolDown < sf::milliseconds(100))
				mEndGameContainer.update(dt);
			else
				mCoolDown -= dt;
		} else {
			mGUIContainer.update(dt);
			mReviewContainer.update(dt);
		}
	} else {
		mGame.update(dt);
		updateClock();
		mPlayers[mGame.mLogic->getTurn()]->update(dt);
		mGUIContainer.update(dt);
		mEvaluation->setText(std::to_string(mGame.mLogic->getEvaluation()));
	}
	return false;
}

bool GameState::handleEvent(const sf::Event& event) {
	if (mGame.mLogic->isFinished()) {
		if (!mReviewMode) {
			if (mCoolDown < sf::milliseconds(100))
				mEndGameContainer.handleEvent(event);
		} else {
			mGUIContainer.handleEvent(event);
			mReviewContainer.handleEvent(event);
		}
	} else {
		mPlayers[mGame.mLogic->getTurn()]->handleEvent(event);
		mGUIContainer.handleEvent(event);
	}
	return false;
}