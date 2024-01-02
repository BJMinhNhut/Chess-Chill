//
// Created by MINH NHUT on 12/28/2023.
//

#include "OldGames.hpp"
#include "Game/GameSaver.hpp"

#include <filesystem>

OldGames::OldGames() : mIndex(-1), mPaths() {
	load();
}

OldGames::~OldGames() = default;

void OldGames::load() {
	for (const auto& entry : std::filesystem::directory_iterator(GameSaver::SAVE_PATH)) {
		mPaths.push_back(entry.path().string());
	}
}

void OldGames::setIndex(int index) {
	mIndex = index;
}

void OldGames::resetIndex() {
	mIndex = -1;
}

size_t OldGames::getSize() const {
	return mPaths.size();
}

bool OldGames::pathChosen() const {
	return mIndex >= 0 && mIndex < mPaths.size();
}

std::string OldGames::getPath() const {
	return mPaths[mIndex];
}