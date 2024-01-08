//
// Created by MINH NHUT on 12/25/2023.
//

#include "GameSaver.hpp"
#include "GameHandler.hpp"

#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <utility>

namespace fs = std::filesystem;

const std::string GameSaver::SAVE_PATH = "saved_data/";

GameSaver::GameSaver(GameOptions options) : mOptions(options), mTime(), mResult() {}

GameSaver::GameSaver(const std::string& path) : mOptions(), mTime(), mResult() {
	load(path);
}

GameSaver::SnapShot::SnapShot() : checkMate(-1), move(-1), notation(), sound() {}

GameSaver::SnapShot::SnapShot(const Board& board, int lastMove, const char notation[10],
                              SoundEffect::ID sound, int8_t checkMate)
    : board(board), move(lastMove), sound(sound), checkMate(checkMate) {
	memcpy(this->notation, notation, sizeof(this->notation));
}

GameSaver::SnapShot GameSaver::getSnapShot(int index) const {
	if (index < 0 || index >= mSnapShots.size()) {
		throw std::runtime_error("GameSaver - Index out of range");
	}
	return mSnapShots[index];
}

GameOptions GameSaver::getOptions() const {
	return mOptions;
}

std::string GameSaver::getResult() const {
	switch (mResult) {
		case GameLogic::Draw:
			return "Draw";
		case GameLogic::WhiteWin:
			return "White win";
		case GameLogic::BlackWin:
			return "Black win";
		default:
			return "Unknown";
	}
}

std::string GameSaver::getDate() const {
	return std::string(std::ctime(&mTime));
}

unsigned int GameSaver::size() const {
	return mSnapShots.size();
}

void GameSaver::undo() {
	if (mSnapShots.empty()) {
		throw std::runtime_error("GameSaver - Cannot undo");
	}
	mSnapShots.pop_back();
}

void GameSaver::capture(const GameHandler& gameHandler) {
	SoundEffect::ID sound;
	GameLogic* logic = gameHandler.mLogic.get();
	if (logic->isFinished())
		sound = SoundEffect::EndGame;
	else if (logic->isChecked())
		sound = SoundEffect::Check;
	else if (logic->isCaptured()) {
		sound = SoundEffect::Capture;
	} else if (logic->isCastled())
		sound = SoundEffect::Castling;
	else if (logic->isPromoted())
		sound = SoundEffect::Promotion;
	else
		sound = SoundEffect::Move;

	int checkMate =
	    (logic->status() == GameLogic::Checkmate) ? logic->getKing(logic->getTurn()) : -1;

	int lastMove = gameHandler.getLastMove();
	std::string notation;
	if (lastMove != -1) {
		int from = lastMove & 0x3f;
		int to = lastMove >> 6;
		if (logic->isCastled()) {
			if (Board::getFile(to) == 6)
				notation = "O-O";
			else
				notation = "O-O-O";
		} else {
			if (!logic->isPromoted())
				notation += Piece::getPieceName(Piece::getType(logic->getPiece(to)));
			if (logic->isCaptured()) {
				if (Piece::getType(logic->getPiece(to)) == Piece::Pawn)
					notation += Board::getSquareName(from)[0];
				notation += "x";
			}
			notation += Board::getSquareName(to);
			if (logic->isPromoted())
				notation += "=" + Piece::getPieceName(Piece::getType(logic->getPiece(to)));
			if (logic->status() == GameLogic::Checkmate) {
				notation += "#";
			} else if (logic->isChecked()) {
				notation += "+";
			}
		}
	}
	mSnapShots.emplace_back(logic->getBoard(), lastMove, &notation[0], sound, checkMate);
}

void GameSaver::load(const std::string& path) {
	if (!fs::exists(path)) {
		throw std::runtime_error("File " + path + " does not exist");
	}
	std::ifstream file(path, std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error("Cannot open file " + path);
	}
	file.seekg(0);
	file.read((char*)&(mOptions), sizeof(mOptions));
	file.read((char*)&(mTime), sizeof(mTime));
	file.read((char*)&(mResult), sizeof(mResult));
	unsigned int mSize;
	file.read((char*)&(mSize), sizeof(mSize));
	mSnapShots.resize(mSize);
	file.read((char*)&(mSnapShots[0]), sizeof(SnapShot) * mSize);
	file.close();
//	std::cout << "Game loaded from " << path << '\n';
}

void GameSaver::save(GameLogic::Result result) {
	// create directory if not exist
	if (fs::is_directory(SAVE_PATH)) {
		std::cout << "Directory " << SAVE_PATH << " already exists\n";
	} else {
		std::cout << "Creating directory " << SAVE_PATH << '\n';
		if (!fs::create_directory(SAVE_PATH)) {
			throw std::runtime_error("Cannot create directory " + SAVE_PATH);
		}
	}

	// save time and result
	mTime = std::time(nullptr);
	mResult = result;
	unsigned int mSize = mSnapShots.size();

	// save to file
	std::string fileName = getFileName(mTime);
	std::ofstream file(SAVE_PATH + fileName, std::ios::out | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("Cannot open file " + fileName);
	}
	file.seekp(0);
	file.write((char*)&(mOptions), sizeof(mOptions));
	file.write((char*)&(mTime), sizeof(mTime));
	file.write((char*)&(mResult), sizeof(mResult));
	file.write((char*)&(mSize), sizeof(mSize));
	file.write((char*)&(mSnapShots[0]), sizeof(SnapShot) * mSize);
	file.close();
	std::cout << "New game saved to " << fileName << '\n';
}

std::string GameSaver::getFileName(time_t time) {
	// file name format: YYYY-MM-DD_HH-MM-SS.sv
	std::string fileName;
	tm* ltm = localtime(&time);
	fileName += std::to_string(1900 + ltm->tm_year) + "-";
	fileName += std::to_string(1 + ltm->tm_mon) + "-";
	fileName += std::to_string(ltm->tm_mday) + "_";
	fileName += std::to_string(ltm->tm_hour) + "-";
	fileName += std::to_string(ltm->tm_min) + "-";
	fileName += std::to_string(ltm->tm_sec) + ".sv";
	return fileName;
}