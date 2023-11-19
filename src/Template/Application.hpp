//
// Created by MINH NHUT on 6/8/2023.
//

#ifndef NATUREOFCODE_APPLICATION_HPP
#define NATUREOFCODE_APPLICATION_HPP

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SoundPlayer.hpp"
#include "StateStack.hpp"

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Time.hpp"

class Application {
   public:
	Application();
	void run();

   private:
	void processInput();
	void update(sf::Time dt);
	void render();

#ifdef SFML_DEBUG
	void updateStatistics(sf::Time dt);
#endif

	void registerStates();
	void loadIcon();
	void loadImages();
	void loadFonts();

   private:
	static const sf::Time TimePerFrame;

	sf::RenderWindow mWindow;
	TextureHolder mTextures;
	FontHolder mFonts;
	SoundPlayer mSounds;
	Settings mSettings;

	StateStack mStateStack;

	sf::Text mStatisticsText;
	sf::Time mStatisticsUpdateTime;
	std::size_t mStatisticsNumFrames;
};

#endif  //NATUREOFCODE_APPLICATION_HPP
