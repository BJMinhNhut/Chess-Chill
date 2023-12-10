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
#include <SFML/Graphics/View.hpp>

#include <iostream>

const sf::Vector2f GameState::BOARD_POSITION(262.f, 105.f);

GameState::GameState(StateStack& stack, Context context)
    : State(stack, context),
      mGame(*context.window, *context.textures, *context.fonts, *context.sounds,
            BOARD_POSITION + sf::Vector2f(25.f, 25.f)),
      mGUIContainer(),
      mWinner(nullptr),
      mDescription(nullptr),
      mPlayers(),
      mClock(),
      mEndGameContainer(),
      mReviewMode(false),
      mEvaluation(nullptr) {

	mGame.setClock(0, sf::seconds(context.options->getTime()),
	               sf::seconds(context.options->getIncrement()));
	mGame.setClock(1, sf::seconds(context.options->getTime()),
	               sf::seconds(context.options->getIncrement()));

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

	mEvaluation = std::make_shared<GUI::Label>(
	    GUI::Label::Main, std::to_string(mGame.getEvaluation()), *context.fonts);
	mEvaluation->setPosition(200.f, 450.f);
	mEvaluation->alignCenter();
	mGUIContainer.pack(mEvaluation);
}

void GameState::loadGameGUI() {
	auto context = getContext();
	auto panels = std::make_shared<GUI::Sprite>(context.textures->get(Textures::GameGUI));
	panels->setPosition(BOARD_POSITION);
	mGUIContainer.pack(panels);

	auto whitePlayer =
	    std::make_shared<GUI::Label>(GUI::Label::Main, mPlayers[0]->getName(), *context.fonts);
	whitePlayer->setPosition(1040.f, 696.f + 8.f);
	mGUIContainer.pack(whitePlayer);

	auto blackPlayer =
	    std::make_shared<GUI::Label>(GUI::Label::Main, mPlayers[1]->getName(), *context.fonts);
	blackPlayer->setPosition(1040.f, 217.f + 8.f);
	mGUIContainer.pack(blackPlayer);

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
}

void GameState::loadControllerGUI() {
	auto rotateButton = std::make_shared<GUI::Button>(GUI::Button::Rotate, *getContext().fonts,
	                                                  *getContext().textures);
	rotateButton->setPosition(1269.f + 50.f / 2, 366.f + 40.f / 2);
	rotateButton->setCallback([this]() { mGame.rotateBoard(); });
	mGUIContainer.pack(rotateButton);
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
	if (mGame.status() == GameLogic::Checkmate) {
		mWinner->setText(mGame.getWinner() + " wins");
		mDescription->setText("by checkmate");
	} else if (mGame.status() == GameLogic::Resign) {
		mWinner->setText(mGame.getWinner() + " wins");
		mDescription->setText("by resignation");
	} else if (mGame.status() == GameLogic::Timeout) {
		mWinner->setText(mGame.getWinner() + " wins");
		mDescription->setText("by timeout");
	} else if (mGame.status() == GameLogic::Stalemate) {
		mWinner->setText("Draw");
		mDescription->setText("by stalemate");
	} else if (mGame.status() == GameLogic::InsufficientMaterial) {
		mWinner->setText("Draw");
		mDescription->setText("by insufficient material");
	} else if (mGame.status() == GameLogic::ThreefoldRepetition) {
		mWinner->setText("Draw");
		mDescription->setText("by threefold repetition");
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
	mClock[0]->setText(getClockString(mGame.getRemainingTime(true)));
	mClock[1]->setText(getClockString(mGame.getRemainingTime(false)));
}

void GameState::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mGUIContainer);
	mGame.draw();
	if (mGame.isFinished()) {
		if (mWinner->isEmpty()) {
			getContext().sounds->play(SoundEffect::EndGame);
			loadResult();
		}
		if (!mReviewMode)
			window.draw(mEndGameContainer);
	}
}

bool GameState::update(sf::Time dt) {
	if (mGame.isFinished()) {
		if (!mReviewMode)
			mEndGameContainer.update(dt);
		else
			mGUIContainer.update(dt);
	} else {
		mGUIContainer.update(dt);
		mPlayers[mGame.getTurn()]->update(dt);
		mGame.update(dt);
		mEvaluation->setText(std::to_string(mGame.getEvaluation()));
		mEvaluation->alignCenter();
		updateClock();
	}
	return false;
}

bool GameState::handleEvent(const sf::Event& event) {
	if (mGame.isFinished()) {
		if (!mReviewMode)
			mEndGameContainer.handleEvent(event);
		else
			mGUIContainer.handleEvent(event);
	} else {
		mGUIContainer.handleEvent(event);
		mPlayers[mGame.getTurn()]->handleEvent(event);
		mGame.handleEvent(event);
	}
	return false;
}