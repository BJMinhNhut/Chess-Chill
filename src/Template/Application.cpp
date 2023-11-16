//
// Created by MINH NHUT on 6/8/2023.
//

#include "Application.hpp"
#include "Constants.hpp"
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
      mColors(),
      mSounds(),
      mStateStack(State::Context(mWindow, mTextures, mFonts, mColors, mSounds)) {

	mWindow.setFramerateLimit(60);

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
}

void Application::registerStates() {
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<AboutState>(States::About);
	mStateStack.registerState<SettingsState>(States::Settings);
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
	mFonts.load(Fonts::Mono,
	            Constants::DATA_PREFIX + "resources/fonts/intelone-mono-font-family-regular.ttf");
}

void Application::loadImages() {
	mTextures.load(Textures::Background, Constants::DATA_PREFIX + "resources/images/bg.png");
	mTextures.load(Textures::Title, Constants::DATA_PREFIX + "resources/images/title.png");
	mTextures.load(Textures::TitleBar, Constants::DATA_PREFIX + "resources/images/title_bar.png");
	mTextures.load(Textures::AboutPanel,
	               Constants::DATA_PREFIX + "resources/images/about_panel.png");
	mTextures.load(Textures::SettingsPanel,
	               Constants::DATA_PREFIX + "resources/images/settings_panel.png");

	// Gameplay
	Settings settings;
	mTextures.load(Textures::PieceSet, settings.getPieceSetPath());
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