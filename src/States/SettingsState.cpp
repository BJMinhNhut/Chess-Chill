//
// Created by MINH NHUT on 11/16/2023.
//

#include "SettingsState.hpp"
#include "GUI/Button.hpp"
#include "GUI/Label.hpp"
#include "GUI/Sprite.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

SettingsState::SettingsState(StateStack& stack, Context context)
    : State(stack, context),
      mGUIContainer(),
      mPieceSet(std::make_shared<GUI::Sprite>(context.textures->get(Textures::PieceSet))),
      mBoard(std::make_shared<GUI::Sprite>(context.textures->get(Textures::Board))) {
	loadBasicGUI();
	loadPieceSetGUI();
	loadBoardGUI();
}

void SettingsState::loadBasicGUI() {
	auto context = getContext();
	auto background = std::make_shared<GUI::Sprite>(context.textures->get(Textures::Background));
	mGUIContainer.pack(background);

	auto backButton =
	    std::make_shared<GUI::Button>(GUI::Button::Back, *context.fonts, *context.textures);
	backButton->setPosition(509.f, 53.f);
	backButton->setCallback([this]() {
		mSettings.save();
		requestStackPop();
	});
	mGUIContainer.pack(backButton);

	auto homeButton =
	    std::make_shared<GUI::Button>(GUI::Button::Home, *context.fonts, *context.textures);
	homeButton->setPosition(1063.f + 54.f / 2, 53.f);
	homeButton->setCallback([this]() {
		mSettings.save();
		requestStateClear();
		requestStackPush(States::Menu);
	});
	mGUIContainer.pack(homeButton);

	auto titleBar = std::make_shared<GUI::Sprite>(context.textures->get(Textures::TitleBar));
	titleBar->centerOrigin();
	titleBar->setPosition(800.f, 53.f);
	mGUIContainer.pack(titleBar);

	auto titleLabel = std::make_shared<GUI::Label>(GUI::Label::Bold, "Settings", *context.fonts);
	titleLabel->setPosition(titleBar->getPosition());
	titleLabel->alignCenter();
	mGUIContainer.pack(titleLabel);

	auto settingsPanel =
	    std::make_shared<GUI::Sprite>(context.textures->get(Textures::SettingsPanel));
	settingsPanel->setPosition(482.f, 236.f);
	mGUIContainer.pack(settingsPanel);
}

void SettingsState::loadPieceSetGUI() {
	mPieceSet->setPosition(810.f + 10.f, 301.f+10.f);
	updatePieceSet();
	mGUIContainer.pack(mPieceSet);

	auto pieceSetForward = std::make_shared<GUI::Button>(GUI::Button::Forward, *getContext().fonts,
	                                                     *getContext().textures);
	pieceSetForward->setPosition(1033.f - 20.f, 317.f + 20.f);
	pieceSetForward->setCallback([this]() {
		mSettings.nextPieceSet();
		getContext().textures->load(Textures::PieceSet, mSettings.getPieceSetPath());
		updatePieceSet();
	});
	mGUIContainer.pack(pieceSetForward);

	auto pieceSetBackward = std::make_shared<GUI::Button>(GUI::Button::Forward, *getContext().fonts,
	                                                      *getContext().textures);
	pieceSetBackward->setPosition(763 + 20.f, 317.f + 20.f);
	pieceSetBackward->rotate(180.f);
	pieceSetBackward->setCallback([this]() {
		mSettings.previousPieceSet();
		getContext().textures->load(Textures::PieceSet, mSettings.getPieceSetPath());
		updatePieceSet();
	});
	mGUIContainer.pack(pieceSetBackward);
}

void SettingsState::loadBoardGUI() {
	mBoard->setPosition(816.f, 429.f);
	updateBoard();
	mGUIContainer.pack(mBoard);

	auto boardForward = std::make_shared<GUI::Button>(GUI::Button::Forward, *getContext().fonts,
	                                                  *getContext().textures);
	boardForward->setPosition(1033.f - 20.f, 470.f);
	boardForward->setCallback([this]() {
		mSettings.nextBoard();
		getContext().textures->load(Textures::Board, mSettings.getBoardPath());
		updateBoard();
	});
	mGUIContainer.pack(boardForward);

	auto boardBackward = std::make_shared<GUI::Button>(GUI::Button::Forward, *getContext().fonts,
	                                                   *getContext().textures);
	boardBackward->setPosition(763 + 20.f, 470.f);
	boardBackward->rotate(180.f);
	boardBackward->setCallback([this]() {
		mSettings.previousBoard();
		getContext().textures->load(Textures::Board, mSettings.getBoardPath());
		updateBoard();
	});
	mGUIContainer.pack(boardBackward);
}

void SettingsState::updatePieceSet() {
	mPieceSet->setTexture(getContext().textures->get(Textures::PieceSet));
	mPieceSet->setSize(156.f, 52.f);
}

void SettingsState::updateBoard() {
	mBoard->setTexture(getContext().textures->get(Textures::Board));
	mBoard->crop(85 * 4, 85 * 2);
	mBoard->setSize(41.f * 4, 41.f * 2);
}

void SettingsState::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mGUIContainer);
}

bool SettingsState::update(sf::Time dt) {
	return true;
}

bool SettingsState::handleEvent(const sf::Event& event) {
	mGUIContainer.handleEvent(event);
	if (event.type == sf::Event::Closed)
		mSettings.save();
	return false;
}
