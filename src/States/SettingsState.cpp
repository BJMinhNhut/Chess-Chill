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
      mPieceSet(std::make_shared<GUI::Sprite>(context.textures->get(Textures::PieceSet))) {
	loadBasicGUI();
	loadSettingsGUI();
}

void SettingsState::loadBasicGUI() {
	auto context = getContext();
	auto background = std::make_shared<GUI::Sprite>(context.textures->get(Textures::Background));
	background->setPosition(800.f, 450.f);
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
	titleBar->setPosition(800.f, 53.f);
	mGUIContainer.pack(titleBar);

	auto titleLabel = std::make_shared<GUI::Label>(GUI::Label::Bold, "Settings", *context.fonts);
	titleLabel->setPosition(titleBar->getPosition());
	titleLabel->alignCenter();
	mGUIContainer.pack(titleLabel);

	auto settingsPanel =
	    std::make_shared<GUI::Sprite>(context.textures->get(Textures::SettingsPanel));
	settingsPanel->setPosition(482.f + 636.f / 2, 236.f + 468.f / 2);
	mGUIContainer.pack(settingsPanel);
}

void SettingsState::loadSettingsGUI() {
	// Piece set settings
	updatePieceSet();
	mGUIContainer.pack(mPieceSet);

	auto pieceSetForward = std::make_shared<GUI::Button>(GUI::Button::Forward, *getContext().fonts,
	                                                     *getContext().textures);
	pieceSetForward->setPosition(1033.f - 20.f, 317.f+20.f);
	pieceSetForward->setCallback([this]() {
		mSettings.nextPieceSet();
		getContext().textures->load(Textures::PieceSet, mSettings.getPieceSetPath());
		updatePieceSet();
	});
	mGUIContainer.pack(pieceSetForward);

	auto pieceSetBackward = std::make_shared<GUI::Button>(GUI::Button::Forward, *getContext().fonts,
	                                                     *getContext().textures);
	pieceSetBackward->setPosition(763 + 20.f, 317.f+20.f);
	pieceSetBackward->rotate(180.f);
	pieceSetBackward->setCallback([this]() {
		mSettings.previousPieceSet();
		getContext().textures->load(Textures::PieceSet, mSettings.getPieceSetPath());
		updatePieceSet();
	});
	mGUIContainer.pack(pieceSetBackward);
}

void SettingsState::updatePieceSet() {
	mPieceSet->setTexture(getContext().textures->get(Textures::PieceSet));
	mPieceSet->setPosition(810.f + 176.f / 2, 301.f + 72.f / 2);
	mPieceSet->setSize(156.f, 52.f);
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
	return false;
}
