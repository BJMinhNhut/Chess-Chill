//
// Created by MINH NHUT on 10/24/2023.
//

#ifndef CHESS_SOUNDPLAYER_HPP
#define CHESS_SOUNDPLAYER_HPP

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/System/NonCopyable.hpp>
//#include <SFML/System/Vector2.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <list>

class SoundPlayer : private sf::NonCopyable {
   public:
	SoundPlayer();
	void play(SoundEffect::ID effect);
	//	void play(SoundEffect::ID effect,
	//	          sf::Vector2f position);

	void removeStoppedSounds();
	//	void setListenerPosition(sf::Vector2f position);
	//	sf::Vector2f getListenerPosition() const;

   private:
	SoundBufferHolder mSoundBuffers;
	std::list<sf::Sound> mSounds;
};

#endif  //CHESS_SOUNDPLAYER_HPP
