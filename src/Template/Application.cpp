//
// Created by MINH NHUT on 6/8/2023.
//

#include "Application.hpp"
#include "Constants.hpp"
#include "Game/Logic/MoveTable.hpp"
#include "State.hpp"
#include "States/StatesList.hpp"
#include "Utility.hpp"

#include <iostream>

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
    : mWindow(sf::VideoMode(1600, 900), "Chess & Chill", sf::Style::Close,
              sf::ContextSettings(0, 0, 8)),
#ifdef SFML_DEBUG
      mStatisticsText(),
      mStatisticsUpdateTime(),
      mStatisticsNumFrames(0),
#endif
      mTextures(),
      mFonts(),
      mSounds(),
      mSettings(),
      mOptions(),
      mStateStack(State::Context(mWindow, mTextures, mFonts, mSounds, mSettings, mOptions, mOldGames)) {

	mWindow.setFramerateLimit(120);

	loadFonts();
	loadIcon();
	loadImages();

#ifdef SFML_DEBUG
	mStatisticsText.setFont(mFonts.get(Fonts::Main));
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setFillColor(sf::Color::White);
	mStatisticsText.setCharacterSize(13u);
#endif

	registerStates();
	mStateStack.pushState(States::Menu);

	if (!mSettings.useSound())
		mSounds.setMute(true);

	MoveTable::getInstance();
}

void Application::registerStates() {
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<AboutState>(States::About);
	mStateStack.registerState<SettingsState>(States::Settings);
	mStateStack.registerState<GameOptionsState>(States::GameOptions);
	mStateStack.registerState<HistoryState>(States::History);
	mStateStack.registerState<ReviewState>(States::Review);
}

void Application::loadIcon() {
	sf::Image image;
	if (!image.loadFromFile(Constants::DATA_PREFIX + "resources/images/icon.png")) {
		throw std::runtime_error("Icon load unsucessfully!");
	}
	mWindow.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
}

void Application::loadFonts() {
	mFonts.load(Fonts::Main, Constants::DATA_PREFIX + "resources/fonts/Inter-Regular.ttf");
	mFonts.load(Fonts::Bold, Constants::DATA_PREFIX + "resources/fonts/Inter-Bold.ttf");
	mFonts.load(Fonts::Mono, Constants::DATA_PREFIX + "resources/fonts/JetBrainsMono-Bold.ttf");
}

void Application::loadImages() {
	mTextures.load(Textures::Background, Constants::DATA_PREFIX + "resources/images/gui/bg.png");
	mTextures.load(Textures::Title, Constants::DATA_PREFIX + "resources/images/gui/title.png");
	mTextures.load(Textures::TitleBar,
	               Constants::DATA_PREFIX + "resources/images/gui/title_bar.png");
	mTextures.load(Textures::AboutPanel,
	               Constants::DATA_PREFIX + "resources/images/gui/about_panel.png");
	mTextures.load(Textures::SettingsPanel,
	               Constants::DATA_PREFIX + "resources/images/gui/settings_panel.png");
	mTextures.load(Textures::EndGamePanel,
	               Constants::DATA_PREFIX + "resources/images/gui/panel_500_320.png");
	mTextures.load(Textures::BoardIndexWhite,
	               Constants::DATA_PREFIX + "resources/images/gui/board_index_white.png");
	mTextures.load(Textures::BoardIndexBlack,
	               Constants::DATA_PREFIX + "resources/images/gui/board_index_black.png");
	mTextures.load(Textures::GameOptionsPanel,
	               Constants::DATA_PREFIX + "resources/images/gui/game_options_panel.png");
	mTextures.load(Textures::OptionBox,
	               Constants::DATA_PREFIX + "resources/images/gui/option_box.png");

	// Gameplay
	mTextures.load(Textures::PieceSet, mSettings.getPieceSetPath());
	mTextures.load(Textures::Board, mSettings.getBoardPath());
	mTextures.load(Textures::GameGUI, Constants::DATA_PREFIX + "resources/images/gui/game_gui.png");
	mTextures.load(Textures::ChessStandard,
	               Constants::DATA_PREFIX + "resources/images/gui/chess_standard.png");
	mTextures.load(Textures::Chess960,
	               Constants::DATA_PREFIX + "resources/images/gui/chess_960.png");
	mTextures.load(Textures::KingOfTheHill,
	               Constants::DATA_PREFIX + "resources/images/gui/chess_koth.png");
	mTextures.load(Textures::PromoteWindow,
	               Constants::DATA_PREFIX + "resources/images/gui/promote_window.png");

	// Buttons
	mTextures.load(Textures::MenuButtonNormal,
	               Constants::DATA_PREFIX + "resources/images/buttons/menu_normal.png");
	mTextures.load(Textures::MenuButtonSelected,
	               Constants::DATA_PREFIX + "resources/images/buttons/menu_selected.png");
	mTextures.load(Textures::BackButtonNormal,
	               Constants::DATA_PREFIX + "resources/images/buttons/back_normal.png");
	mTextures.load(Textures::BackButtonSelected,
	               Constants::DATA_PREFIX + "resources/images/buttons/back_selected.png");
	mTextures.load(Textures::HomeButtonNormal,
	               Constants::DATA_PREFIX + "resources/images/buttons/home_normal.png");
	mTextures.load(Textures::HomeButtonSelected,
	               Constants::DATA_PREFIX + "resources/images/buttons/home_selected.png");
	mTextures.load(Textures::ForwardButtonNormal,
	               Constants::DATA_PREFIX + "resources/images/buttons/forward_normal.png");
	mTextures.load(Textures::ForwardButtonSelected,
	               Constants::DATA_PREFIX + "resources/images/buttons/forward_selected.png");
	mTextures.load(Textures::RotateButtonNormal,
	               Constants::DATA_PREFIX + "resources/images/buttons/rotate_normal.png");
	mTextures.load(Textures::RotateButtonSelected,
	               Constants::DATA_PREFIX + "resources/images/buttons/rotate_selected.png");
	mTextures.load(Textures::PlayButtonNormal,
	               Constants::DATA_PREFIX + "resources/images/buttons/play_normal.png");
	mTextures.load(Textures::PlayButtonSelected,
	               Constants::DATA_PREFIX + "resources/images/buttons/play_selected.png");
	mTextures.load(Textures::CloseButtonNormal,
	               Constants::DATA_PREFIX + "resources/images/buttons/close_normal.png");
	mTextures.load(Textures::CloseButtonSelected,
	               Constants::DATA_PREFIX + "resources/images/buttons/close_selected.png");

	mTextures.load(Textures::FirstButtonNormal,
	               Constants::DATA_PREFIX + "resources/images/buttons/first_normal.png");
	mTextures.load(Textures::FirstButtonSelected,
	               Constants::DATA_PREFIX + "resources/images/buttons/first_selected.png");
	mTextures.load(Textures::LastButtonNormal,
	               Constants::DATA_PREFIX + "resources/images/buttons/last_normal.png");
	mTextures.load(Textures::LastButtonSelected,
	               Constants::DATA_PREFIX + "resources/images/buttons/last_selected.png");
	mTextures.load(Textures::PreviousButtonNormal,
	               Constants::DATA_PREFIX + "resources/images/buttons/previous_normal.png");
	mTextures.load(Textures::PreviousButtonSelected,
	               Constants::DATA_PREFIX + "resources/images/buttons/previous_selected.png");
	mTextures.load(Textures::NextButtonNormal,
	               Constants::DATA_PREFIX + "resources/images/buttons/next_normal.png");
	mTextures.load(Textures::NextButtonSelected,
	               Constants::DATA_PREFIX + "resources/images/buttons/next_selected.png");
}

void Application::run() {
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen()) {
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame) {
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			if (mStateStack.isEmpty())
				mWindow.close();
		}
#ifdef SFML_DEBUG
		updateStatistics(dt);
#endif
		render();
	}
}

void Application::processInput() {
	sf::Event event{};
	while (mWindow.pollEvent(event)) {
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Application::update(sf::Time dt) {
	mStateStack.update(dt);
}

void Application::render() {
	mWindow.clear();
	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticsText);

	mWindow.display();
}

#ifdef SFML_DEBUG

void Application::updateStatistics(sf::Time dt) {
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;
	if (mStatisticsUpdateTime >= sf::seconds(1.f)) {
		mStatisticsText.setString("FPS: " + std::to_string(mStatisticsNumFrames));

		mStatisticsUpdateTime -= sf::seconds(1.f);
		mStatisticsNumFrames = 0;
	}
}

#endif