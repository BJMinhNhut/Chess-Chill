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

GameSaver::SnapShot::SnapShot(const Board& board, int lastMove, std::string notation,
                              SoundEffect::ID sound, int8_t checkMate)
    : board(board),
      move(lastMove),
      notation(std::move(notation)),
      sound(sound),
      checkMate(checkMate) {}

GameSaver::SnapShot GameSaver::get(int index) const {
	if (index < 0 || index >= mSnapShots.size()) {
		throw std::runtime_error("GameSaver - Index out of range");
	}
	return mSnapShots[index];
}

unsigned int GameSaver::size() const {
	return mSnapShots.size();
}

void GameSaver::capture(const GameHandler& gameHandler) {
	SoundEffect::ID sound;
	GameLogic *logic = gameHandler.mLogic.get();
	if (logic->isChecked())
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
	mSnapShots.emplace_back(logic->getBoard(), lastMove, notation, sound, checkMate);
}

void GameSaver::save() {
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
	file.write((char*)&(*this), sizeof(*this));
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