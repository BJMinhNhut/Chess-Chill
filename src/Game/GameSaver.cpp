//
// Created by MINH NHUT on 12/25/2023.
//

#include "GameSaver.hpp"

#include <ctime>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

const std::string GameSaver::SAVE_PATH = "saved_data/";

void GameSaver::saveGame(const std::vector<GameHandler::SnapShot>& snapShots) {
	if (fs::is_directory(SAVE_PATH)) {
		std::cout << "Directory " << SAVE_PATH << " already exists\n";
	} else {
		std::cout << "Creating directory " << SAVE_PATH << '\n';
		fs::create_directory(SAVE_PATH);
	}

	std::string fileName = getNewFileName();
	std::ofstream file(SAVE_PATH + fileName, std::ios::out | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("Cannot open file " + fileName);
	}
	file.seekp(0);
	file.write((char*)&(snapShots), sizeof(snapShots));
	file.close();
	std::cout << "New game saved to " << fileName << '\n';
}

std::string GameSaver::getNewFileName() {
	// file name format: YYYY-MM-DD-HH-MM-SS.bin
	std::string fileName;
	time_t now = time(nullptr);
	tm* ltm = localtime(&now);
	fileName += std::to_string(1900 + ltm->tm_year) + "-";
	fileName += std::to_string(1 + ltm->tm_mon) + "-";
	fileName += std::to_string(ltm->tm_mday) + "-";
	fileName += std::to_string(ltm->tm_hour) + "-";
	fileName += std::to_string(ltm->tm_min) + "-";
	fileName += std::to_string(ltm->tm_sec) + ".sv";
	return fileName;
}