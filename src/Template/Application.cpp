//
// Created by MINH NHUT on 6/8/2023.
//

#include "Application.hpp"
#include "Constants.hpp"
//#include "Settings.hpp"
#include "State.hpp"
#include "States/StatesList.hpp"
#include "Utility.hpp"

#include <iostream>

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
    : mWindow(sf::VideoMode(1600, 900), "Chess", sf::Style::Close,
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

	mFonts.load(Fonts::Main, Constants::dataPrefix + "resources/fonts/PublicSans-Regular.ttf");
	mFonts.load(Fonts::Bold, Constants::dataPrefix + "resources/fonts/PublicSans-Bold.ttf");
	mFonts.load(Fonts::Mono,
	            Constants::dataPrefix + "resources/fonts/intelone-mono-font-family-regular.ttf");

//	loadIcon();

//	if (getSettings().theme == Settings::Themes::Light)
//		Utility::loadLightTheme(mTextures, mColors);
//	else
//		Utility::loadDarkTheme(mTextures, mColors);

#ifdef SFML_DEBUG
	mStatisticsText.setFont(mFonts.get(Fonts::Main));
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setFillColor(sf::Color::Black);
	mStatisticsText.setCharacterSize(13u);
#endif

	registerStates();
	mStateStack.pushState(States::Game);
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

void Application::registerStates() {
//	mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<GameState>(States::Game);
}

void Application::loadIcon() {
	sf::Image image;
	if (!image.loadFromFile(Constants::dataPrefix + "resources/images/icon.png")) {
		throw std::runtime_error("Icon load unsucessfully!");
	}
	mWindow.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
}