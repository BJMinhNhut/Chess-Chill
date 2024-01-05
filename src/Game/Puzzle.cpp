//
// Created by MINH NHUT on 1/5/2024.
//

#include "Puzzle.hpp"
#include "Move.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

Puzzle::Puzzle(std::string fen, const std::string& solution, Status status)
	: mFen(std::move(fen)), mSolution(), mStatus(status) {
	std::string move;
	for (int i = 0; i < solution.size(); ++i) {
		if (solution[i] == ' ' || i+1 == solution.size()) {
			mSolution.emplace_back(move);
			move.clear();
		} else {
			move += solution[i];
		}
	}
}

std::string Puzzle::getFen() const {
	return mFen;
}

Move Puzzle::getMove(int index) const {
	return mSolution[index];
}

Puzzle::Status Puzzle::getStatus() const {
	return mStatus;
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
//		std::cout << fen << " " << solution << " " << status << std::endl;
		puzzles.emplace_back(fen, solution, static_cast<Status>(std::stoi(status)));
	}
	std::cout << puzzles.size() << " puzzles loaded\n";
	return puzzles;
}