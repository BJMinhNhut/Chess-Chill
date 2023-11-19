//
// Created by MINH NHUT on 7/4/2023.
//

#include "Settings.hpp"
#include "Constants.hpp"

#include <fstream>
#include <iostream>

const std::vector<std::string> Settings::PIECESET_NAMES = {
    "default", "alpha", "anarcandy", "cardinal", "chessnut", "kiwen-suwi", "maestro", "tatiana"};
const std::string Settings::PIECESET_PATH = Constants::DATA_PREFIX + "resources/images/pieces/";

const std::vector<std::string> Settings::BOARD_NAMES = {
    "default", "blackwhite", "blue",         "green_plastic", "maple",
    "metal",   "newspaper",  "pink_pyramid", "purple_diag",   "wood"};
const std::string Settings::BOARD_PATH = Constants::DATA_PREFIX + "resources/images/boards/";

const std::vector<std::string> Settings::SOUND_OPTIONS = {"Off", "On"};

const std::string Settings::SETTINGS_FILE = "settings.bin";

Settings::Settings() : mPieceSetID(0), mBoardID(0), mSound(true) {
	load();
}

void Settings::nextPieceSet() {
	mPieceSetID = (mPieceSetID + 1) % PIECESET_NAMES.size();
	std::cout << "Next piece set: " << PIECESET_NAMES[mPieceSetID] << "\n";
}

void Settings::previousPieceSet() {
	mPieceSetID = (mPieceSetID - 1 + PIECESET_NAMES.size()) % PIECESET_NAMES.size();
	std::cout << "Previous piece set: " << PIECESET_NAMES[mPieceSetID] << "\n";
}

void Settings::nextBoard() {
	mBoardID = (mBoardID + 1) % BOARD_NAMES.size();
	std::cout << "Next board: " << BOARD_NAMES[mBoardID] << "\n";
}

void Settings::previousBoard() {
	mBoardID = (mBoardID - 1 + BOARD_NAMES.size()) % BOARD_NAMES.size();
	std::cout << "Previous board: " << BOARD_NAMES[mBoardID] << "\n";
}

void Settings::toggleSound() {
	mSound = !mSound;
	std::cout << "Sound: " << SOUND_OPTIONS[mSound] << "\n";
}

void Settings::print() {
	std::cout << "==Settings==\n";
	std::cout << "Piece set: ";
	std::cout << PIECESET_NAMES[mPieceSetID] << "\n";
	std::cout << "Board: ";
	std::cout << BOARD_NAMES[mBoardID] << "\n";
	std::cout << "==End==\n";
}

bool Settings::operator==(const Settings& settings) const {
	if (mPieceSetID != settings.mPieceSetID)
		return false;
	return true;
}

bool Settings::operator!=(const Settings& settings) const {
	return !((*this) == settings);
}

void Settings::load() {
	std::ifstream settingsFile(SETTINGS_FILE, std::ios::in | std::ios::binary);
	settingsFile.seekg(0);

	if (!settingsFile.read((char*)&(*this), sizeof(*this))) {
		std::cerr << "Settings file doesn't have correct format. "
		             "Init default settings.\n";
		save();
	}
	settingsFile.close();
	print();
}

void Settings::save() {
	std::ofstream settingsFile(SETTINGS_FILE, std::ios::out | std::ios::binary);
	settingsFile.seekp(0);
	settingsFile.write((char*)&(*this), sizeof(*this));
	settingsFile.close();
}

std::string Settings::getPieceSetPath() const {
	return PIECESET_PATH + PIECESET_NAMES[mPieceSetID] + ".png";
}

std::string Settings::getBoardPath() const {
	return BOARD_PATH + BOARD_NAMES[mBoardID] + ".png";
}

std::string Settings::getSoundLabel() const {
	return SOUND_OPTIONS[mSound];
}

bool Settings::useSound() const {
	return mSound;
}