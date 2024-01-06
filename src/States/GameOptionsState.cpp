//
// Created by MINH NHUT on 11/19/2023.
//

#include "GameOptionsState.hpp"
#include "GUI/Button.hpp"
#include "GUI/Label.hpp"
#include "GUI/Sprite.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

GameOptionsState::GameOptionsState(StateStack& stack, Context context)
    : State(stack, context),
      mGUIContainer(), mType(nullptr), mMode(nullptr), mTime(nullptr), mDescription(nullptr) {
	loadBasicGUI();
	loadDescriptionGUI();
	loadGameModeGUI();
	loadTypeGUI();
	loadTimeGUI();
}

void GameOptionsState::loadBasicGUI() {
	auto context = getContext();
	auto background = std::make_shared<GUI::Sprite>(context.textures->get(Textures::Background));
	mGUIContainer.pack(background);

	auto backButton =
	    std::make_shared<GUI::Button>(GUI::Button::Back, *context.fonts, *context.textures);
	backButton->setPosition(509.f, 53.f);
	backButton->setCallback([this]() {
		getContext().settings->save();
		getContext().oldGames->resetIndex();
		requestStackPop();
	});
	mGUIContainer.pack(backButton);

	auto homeButton =
	    std::make_shared<GUI::Button>(GUI::Button::Home, *context.fonts, *context.textures);
	homeButton->setPosition(1063.f + 54.f / 2, 53.f);
	homeButton->setCallback([this]() {
		getContext().settings->save();
		requestStackClear();
		requestStackPush(States::Menu);
	});
	mGUIContainer.pack(homeButton);

	auto titleBar = std::make_shared<GUI::Sprite>(context.textures->get(Textures::TitleBar));
	titleBar->centerOrigin();
	titleBar->setPosition(800.f, 53.f);
	mGUIContainer.pack(titleBar);

	auto titleLabel = std::make_shared<GUI::Label>(GUI::Label::Bold, "Game Options", *context.fonts);
	titleLabel->setPosition(titleBar->getPosition());
	titleLabel->alignCenter();
	mGUIContainer.pack(titleLabel);

	auto optionsPanel =
	    std::make_shared<GUI::Sprite>(context.textures->get(Textures::GameOptionsPanel));
	optionsPanel->setPosition(322.f, 220.f);
	mGUIContainer.pack(optionsPanel);

	auto playButton = std::make_shared<GUI::Button>(GUI::Button::Play, *context.fonts, *context.textures);
	playButton->setCallback([this]() {
		getContext().oldGames->resetIndex();
		*getContext().mode = Context::Normal;
		requestStackPush(States::Game);
	});
	playButton->setPosition(973.f + 306.f/2, 638.f + 82.f/2);
	mGUIContainer.pack(playButton);
}

void GameOptionsState::loadGameModeGUI() {
	auto modeLabel = std::make_shared<GUI::Label>(GUI::Label::Bold, "Game Mode", *getContext().fonts);
	modeLabel->alignRight();
	modeLabel->setPosition(383.f + 163.f, 282.f + 7.f);
	mGUIContainer.pack(modeLabel);

	auto optionBox = std::make_shared<GUI::Sprite>(getContext().textures->get(Textures::OptionBox));
	optionBox->centerOrigin();
	optionBox->setPosition(653.f+180.f/2, 276.f + 40.f/2);
	mGUIContainer.pack(optionBox);

	mMode = std::make_shared<GUI::Label>(GUI::Label::Small, getContext().options->getStringMode(), *getContext().fonts);
	mMode->setPosition(optionBox->getPosition());
	updateMode();
	mGUIContainer.pack(mMode);

	auto modeForward = std::make_shared<GUI::Button>(GUI::Button::Forward, *getContext().fonts, *getContext().textures);
	modeForward->setPosition(878.f - 20.f, 276.f + 20.f);
	modeForward->setCallback([&]() {
		getContext().options->nextMode();
		updateMode();
	});
	mGUIContainer.pack(modeForward);

	auto modeBackward = std::make_shared<GUI::Button>(GUI::Button::Forward, *getContext().fonts, *getContext().textures);
	modeBackward->setPosition(608.f + 20.f, 316.f - 20.f);
	modeBackward->rotate(180.f);
	modeBackward->setCallback([&]() {
		getContext().options->prevMode();
		updateMode();
	});
	mGUIContainer.pack(modeBackward);
}

void GameOptionsState::updateMode() {
	mMode->setText(getContext().options->getStringMode());
	mMode->alignCenter();
}

void GameOptionsState::loadTypeGUI() {
	auto typeLabel = std::make_shared<GUI::Label>(GUI::Label::Bold, "Game Type", *getContext().fonts);
	typeLabel->alignRight();
	typeLabel->setPosition(383.f + 163.f, 369.f + 7.f);
	mGUIContainer.pack(typeLabel);

	auto optionBox = std::make_shared<GUI::Sprite>(getContext().textures->get(Textures::OptionBox));
	optionBox->centerOrigin();
	optionBox->setPosition(653.f+180.f/2, 363.f + 40.f/2);
	mGUIContainer.pack(optionBox);

	mType = std::make_shared<GUI::Label>(GUI::Label::Small, getContext().options->getStringType(), *getContext().fonts);
	mType->setPosition(optionBox->getPosition());
	updateType();
	mGUIContainer.pack(mType);

	auto typeForward = std::make_shared<GUI::Button>(GUI::Button::Forward, *getContext().fonts, *getContext().textures);
	typeForward->setPosition(878.f - 20.f, 363.f + 20.f);
	typeForward->setCallback([&]() {
		getContext().options->nextType();
		updateType();
	});
	mGUIContainer.pack(typeForward);

	auto typeBackward = std::make_shared<GUI::Button>(GUI::Button::Forward, *getContext().fonts, *getContext().textures);
	typeBackward->setPosition(608.f + 20.f, 403.f - 20.f);
	typeBackward->rotate(180.f);
	typeBackward->setCallback([&]() {
		getContext().options->prevType();
		updateType();
	});
	mGUIContainer.pack(typeBackward);
}

void GameOptionsState::updateType() {
	mType->setText(getContext().options->getStringType());
	mType->alignCenter();
	Textures::ID id = static_cast<Textures::ID>(getContext().options->getTypeDescriptionID());
	mDescription->setTexture(getContext().textures->get(id));
}

void GameOptionsState::loadTimeGUI() {
	auto timeLabel = std::make_shared<GUI::Label>(GUI::Label::Bold, "Time", *getContext().fonts);
	timeLabel->alignRight();
	timeLabel->setPosition(383.f + 163.f, 456.f + 7.f);
	mGUIContainer.pack(timeLabel);

	auto optionBox = std::make_shared<GUI::Sprite>(getContext().textures->get(Textures::OptionBox));
	optionBox->centerOrigin();
	optionBox->setPosition(653.f+180.f/2, 450.f + 40.f/2);
	mGUIContainer.pack(optionBox);

	mTime = std::make_shared<GUI::Label>(GUI::Label::Small, getContext().options->getStringTime(), *getContext().fonts);
	mTime->setPosition(optionBox->getPosition());
	updateTime();
	mGUIContainer.pack(mTime);

	auto timeForward = std::make_shared<GUI::Button>(GUI::Button::Forward, *getContext().fonts, *getContext().textures);
	timeForward->setPosition(878.f - 20.f, 450.f + 20.f);
	timeForward->setCallback([&]() {
		getContext().options->nextTime();
		updateTime();
	});
	mGUIContainer.pack(timeForward);

	auto timeBackward = std::make_shared<GUI::Button>(GUI::Button::Forward, *getContext().fonts, *getContext().textures);
	timeBackward->setPosition(608.f + 20.f, 490.f - 20.f);
	timeBackward->rotate(180.f);
	timeBackward->setCallback([&]() {
		getContext().options->prevTime();
		updateTime();
	});
	mGUIContainer.pack(timeBackward);
}

void GameOptionsState::loadDescriptionGUI() {
	Textures::ID id = static_cast<Textures::ID>(getContext().options->getTypeDescriptionID());
	mDescription = std::make_shared<GUI::Sprite>(getContext().textures->get(id));
	mDescription->setPosition(973.f, 220.f);
	mGUIContainer.pack(mDescription);
}

void GameOptionsState::updateTime() {
	mTime->setText(getContext().options->getStringTime());
	mTime->alignCenter();
}

void GameOptionsState::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mGUIContainer);
}

bool GameOptionsState::update(sf::Time dt) {
	return true;
}

bool GameOptionsState::handleEvent(const sf::Event& event) {
	mGUIContainer.handleEvent(event);
	return false;
}
