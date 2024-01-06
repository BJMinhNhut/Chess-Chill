//
// Created by MINH NHUT on 1/5/2024.
//

#include "Puzzle.hpp"
#include "Move.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Puzzle::Puzzle(int id, std::string fen, const std::string& solution, Status status)
    : mFen(std::move(fen)), mSolution(), mStatus(status), mId(id) {
	std::string move;
	for (int i = 0; i < solution.size(); ++i) {
		if (solution[i] == ' ' || i + 1 == solution.size()) {
			mSolution.emplace_back(move);
			move.clear();
		} else {
			move += solution[i];
		}
	}
}

Puzzle::Puzzle() : mFen(), mSolution(), mStatus(Unsolved), mId() {}

std::string Puzzle::getFen() const {
	return mFen;
}

Move Puzzle::getMove(int index) const {
	return mSolution[index];
}

Puzzle::Status Puzzle::getStatus() const {
	return mStatus;
}

int8_t Puzzle::getId() const {
	return mId;
}

size_t Puzzle::getSolutionSize() const {
	return mSolution.size();
}

std::vector<Puzzle> Puzzle::loadPuzzles(const std::string& path) {
	std::vector<Puzzle> puzzles;
	std::ifstream file(path, std::ios::in);
	if (!file.is_open()) {
		std::cout << "Failed to open file: " << path << std::endl;
		return puzzles;
	}
	std::string fen, solution, status;
	std::string line, word;
	int count = 0;
	file.seekg(3);
	while (std::getline(file, line)) {
		std::stringstream ss(line);

		fen.clear();
		solution.clear();
		status.clear();

		while (std::getline(ss, word, ',')) {
			if (fen.empty()) {
				fen = word;
			} else if (solution.empty()) {
				solution = word;
			} else {
				status = word;
			}
		}
		std::cout << fen << " " << solution << " " << status << std::endl;
		puzzles.emplace_back(++count, fen, solution, static_cast<Status>(std::stoi(status)));
	}
	std::cout << puzzles.size() << " puzzles loaded\n";
	return puzzles;
}
