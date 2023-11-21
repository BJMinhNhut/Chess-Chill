//
// Created by MINH NHUT on 11/19/2023.
//

#include "GameOptions.hpp"
#include "ResourceIdentifiers.hpp"

const std::vector<std::string> GameOptions::MODE_NAMES = {
	"PvP",
	"PvAI",
	"AIvAI",
};

const std::vector<std::pair<std::string, int>> GameOptions::TYPE_NAMES = {
	std::make_pair("Standard", Textures::ChessStandard),
	std::make_pair("Chess960", Textures::Chess960),
};

const std::vector<std::pair<int, int>> GameOptions::TIMES = {
    {0, 0},
	{1, 0},
    {1, 1},
    {2, 1},
    {3, 0},
    {3, 2},
    {5, 0},
    {10, 0},
    {15, 10},
    {30, 0}
};

GameOptions::GameOptions() : mMode(PvAI), mType(Standard), mTimeID(6) {}

std::string GameOptions::getStringTime() const {
	int mTime = TIMES[mTimeID].first;
	int mIncrement = TIMES[mTimeID].second;
	if (mTime == 0) return "Unlimited";
	if (mIncrement)
		return std::to_string(mTime) + " | " + std::to_string(mIncrement);
	return std::to_string(mTime) + " min";
}

std::string GameOptions::getStringMode() const {
	return MODE_NAMES[mMode];
}

std::string GameOptions::getStringType() const {
	return TYPE_NAMES[mType].first;
}

int GameOptions::getTypeDescriptionID() const {
	return TYPE_NAMES[mType].second;
}

int GameOptions::getTime() const {
	return TIMES[mTimeID].first * 60;
}

int GameOptions::getIncrement() const {
	return TIMES[mTimeID].second;
}

GameOptions::Mode GameOptions::getMode() const {
	return mMode;
}

GameOptions::Type GameOptions::getType() const {
	return mType;
}

void GameOptions::nextMode() {
	mMode = static_cast<Mode>((mMode + 1) % NumModes);
}

void GameOptions::nextType() {
	mType = static_cast<Type>((mType + 1) % NumTypes);
}

void GameOptions::nextTime() {
	mTimeID = (mTimeID + 1) % TIMES.size();
}

void GameOptions::prevMode() {
	mMode = static_cast<Mode>((mMode + NumModes - 1) % NumModes);
}

void GameOptions::prevType() {
	mType = static_cast<Type>((mType + NumTypes - 1) % NumTypes);
}

void GameOptions::prevTime() {
	mTimeID = (mTimeID + TIMES.size() - 1) % TIMES.size();
}

