//
// Created by MINH NHUT on 12/28/2023.
//

#include "OldGames.hpp"
#include "Game/GameSaver.hpp"

#include <filesystem>
#include <iostream>

OldGames::OldGames() : mIndex(-1), mPaths() {
	load();
}

OldGames::~OldGames() = default;

void OldGames::load() {
	if (!std::filesystem::exists(GameSaver::SAVE_PATH)) {
		std::filesystem::create_directory(GameSaver::SAVE_PATH);
		std::cout << "Created directory " << GameSaver::SAVE_PATH << '\n';
	}
	mPaths.clear();
	for (const auto& entry : std::filesystem::directory_iterator(GameSaver::SAVE_PATH)) {
		mPaths.push_back(entry.path().string());
	}
	std::reverse(mPaths.begin(), mPaths.end());
}

void OldGames::setIndex(int index) {
	assert(index >= 0 && index < mPaths.size());
	mIndex = index;
	std::cout << "OldGames - Set index to " << index << '\n';
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

std::string OldGames::getPathByID(int id) const {
	assert(id >= 0 && id < mPaths.size());
	return mPaths[id];
}