//
// Created by MINH NHUT on 10/24/2023.
//

#include "SoundPlayer.hpp"
#include "Constants.hpp"

SoundPlayer::SoundPlayer() : mSoundBuffers(), mSounds() {
	mSoundBuffers.load(SoundEffect::Move, Constants::DATA_PREFIX + "resources/music/standard/Move.mp3");
	mSoundBuffers.load(SoundEffect::Capture, Constants::DATA_PREFIX + "resources/music/standard/Capture.mp3");
	mSoundBuffers.load(SoundEffect::Check, Constants::DATA_PREFIX + "resources/music/standard/Check.mp3");
	mSoundBuffers.load(SoundEffect::Castling, Constants::DATA_PREFIX + "resources/music/standard/Castling.mp3");
	mSoundBuffers.load(SoundEffect::Promotion, Constants::DATA_PREFIX + "resources/music/standard/Promotion.mp3");
	mSoundBuffers.load(SoundEffect::OutOfBound, Constants::DATA_PREFIX + "resources/music/standard/OutOfBound.mp3");
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