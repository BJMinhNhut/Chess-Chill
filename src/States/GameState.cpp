//
// Created by MINH NHUT on 10/23/2023.
//

#include "GameState.hpp"
#include "GUI/Button.hpp"
#include "GUI/Label.hpp"
#include "GUI/Sprite.hpp"
#include "Template/ResourceHolder.hpp"
#include "Template/Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <iostream>

GameState::GameState(StateStack& stack, Context context)
    : State(stack, context),
      mGame(*context.window, *context.textures, *context.fonts, *context.sounds),
      mGUIContainer(),
      mWinner(nullptr),
      mDescription(nullptr) {
	loadBasicGUI();
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

	auto titleLabel =
	    std::make_shared<GUI::Label>(GUI::Label::Bold, "Player vs Player", *context.fonts);
	titleLabel->setPosition(titleBar->getPosition());
	titleLabel->alignCenter();
	mGUIContainer.pack(titleLabel);
}

void GameState::loadEndGameGUI() {
	auto panel = std::make_shared<GUI::Sprite>(getContext().textures->get(Textures::EndGamePanel));
	panel->setPosition(547.f, 285.f);
	mEndGameContainer.pack(panel);

	auto newGameButton = std::make_shared<GUI::Button>(GUI::Button::Menu, *getContext().fonts,
	                                                   *getContext().textures);
	newGameButton->setPosition(717.f + 166.f / 2, 451.f + 48.f / 2);
	newGameButton->setText("New Game");
	newGameButton->setCallback([this]() {
		requestStackPop();
		requestStackPush(States::Game);
	});
	mEndGameContainer.pack(newGameButton);

	auto homeButton = std::make_shared<GUI::Button>(GUI::Button::Menu, *getContext().fonts,
	                                                *getContext().textures);
	homeButton->setPosition(717.f + 166.f / 2, 515.f + 48.f / 2);
	homeButton->setText("Home");
	homeButton->setCallback([this]() {
		requestStateClear();
		requestStackPush(States::Menu);
	});
	mEndGameContainer.pack(homeButton);

	mWinner = std::make_shared<GUI::Label>(GUI::Label::Title, "", *getContext().fonts);
	mWinner->setPosition(800.f, 306.f + 102.f / 2);
	mEndGameContainer.pack(mWinner);

	mDescription = std::make_shared<GUI::Label>(GUI::Label::Small, "", *getContext().fonts);
	mDescription->setPosition(800.f, 393.f + 32.f / 2);
	mEndGameContainer.pack(mDescription);
}

void GameState::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mGUIContainer);
	mGame.draw();
	if (mGame.isFinished()) {
		if (mWinner->isEmpty()) {
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
			}
			// TODO: implement these
			//			else if (mGame.status() == GameLogic::InsufficientMaterial) {
			//				mWinner->setText("Draw");
			//				mDescription->setText("by insufficient material");
			//			} else if (mGame.status() == GameLogic::FiftyMoveRule) {
			//				mWinner->setText("Draw");
			//				mDescription->setText("by fifty move rule");
			//			} else if (mGame.status() == GameLogic::ThreefoldRepetition) {
			//				mWinner->setText("Draw");
			//				mDescription->setText("by threefold repetition");
			//			} else if (mGame.status() == GameLogic::Draw) {
			//				mWinner->setText("Draw");
			//				mDescription->setText("by agreement");
			//			}
			mWinner->alignCenter();
			mDescription->alignCenter();
		}
		window.draw(mEndGameContainer);
	}
}

bool GameState::update(sf::Time dt) {
	if (mGame.isFinished()) {
		mEndGameContainer.update(dt);
	} else {
		mGame.update(dt);
		mGUIContainer.update(dt);
	}
	return false;
}

bool GameState::handleEvent(const sf::Event& event) {
	if (mGame.isFinished()) {
		mEndGameContainer.handleEvent(event);
	} else {
		mGame.handleEvent(event);
		mGUIContainer.handleEvent(event);
	}
	return false;
}