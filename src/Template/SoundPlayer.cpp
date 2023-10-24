//
// Created by MINH NHUT on 10/24/2023.
//

#include "SoundPlayer.hpp"
#include "Constants.hpp"

SoundPlayer::SoundPlayer() : mSoundBuffers(), mSounds() {
	mSoundBuffers.load(SoundEffect::Move, Constants::dataPrefix + "resources/music/standard/Move.mp3");
	mSoundBuffers.load(SoundEffect::Capture, Constants::dataPrefix + "resources/music/standard/Capture.mp3");
}

void SoundPlayer::play(SoundEffect::ID effect) {
	mSounds.emplace_back(mSoundBuffers.get(effect));
	mSounds.back().play();
}

void SoundPlayer::removeStoppedSounds() {
	mSounds.remove_if([](const sf::Sound& sound){
		return sound.getStatus() == sf::Sound::Stopped;
	});
}