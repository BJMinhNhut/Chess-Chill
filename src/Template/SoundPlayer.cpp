//
// Created by MINH NHUT on 10/24/2023.
//

#include "SoundPlayer.hpp"
#include "Constants.hpp"

SoundPlayer::SoundPlayer() : mSoundBuffers(), mSounds(), mMute(false) {
	mSoundBuffers.load(SoundEffect::Move,
	                   Constants::DATA_PREFIX + "resources/music/standard/Move.mp3");
	mSoundBuffers.load(SoundEffect::Capture,
	                   Constants::DATA_PREFIX + "resources/music/standard/Capture.mp3");
	mSoundBuffers.load(SoundEffect::Check,
	                   Constants::DATA_PREFIX + "resources/music/standard/Check.mp3");
	mSoundBuffers.load(SoundEffect::Castling,
	                   Constants::DATA_PREFIX + "resources/music/standard/Castling.mp3");
	mSoundBuffers.load(SoundEffect::Promotion,
	                   Constants::DATA_PREFIX + "resources/music/standard/Promotion.mp3");
	mSoundBuffers.load(SoundEffect::OutOfBound,
	                   Constants::DATA_PREFIX + "resources/music/standard/OutOfBound.mp3");
	mSoundBuffers.load(SoundEffect::EndGame,
	                   Constants::DATA_PREFIX + "resources/music/standard/EndGame.mp3");
	mSoundBuffers.load(SoundEffect::StartGame,
	                   Constants::DATA_PREFIX + "resources/music/standard/StartGame.mp3");
}

void SoundPlayer::play(SoundEffect::ID effect) {
	if (mMute)
		return;
	mSounds.emplace_back(mSoundBuffers.get(effect));
	mSounds.back().play();
}

void SoundPlayer::setMute(bool mute) {
	mMute = mute;
}

void SoundPlayer::removeStoppedSounds() {
	mSounds.remove_if(
	    [](const sf::Sound& sound) { return sound.getStatus() == sf::Sound::Stopped; });
}