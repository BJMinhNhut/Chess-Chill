//
// Created by MINH NHUT on 1/7/2024.
//

#include "PuzzleList.hpp"
#include "Template/Constants.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

const std::string PuzzleList::PATH = Constants::DATA_PREFIX + "resources/puzzles.csv";

PuzzleList::PuzzleList() : mPuzzles(), mIndex(0) {
	load();
}

void PuzzleList::setPuzzle(int index) {
	assert(index >= 0 && index < mPuzzles.size());
	mIndex = index;
}

void PuzzleList::nextPuzzle() {
	assert(mIndex + 1 < mPuzzles.size());
	mIndex++;
}

void PuzzleList::prevPuzzle() {
	assert(mIndex - 1 >= 0);
	mIndex--;
}

Puzzle& PuzzleList::getChosenPuzzle() {
	return mPuzzles[mIndex];
}

const Puzzle& PuzzleList::getChosenPuzzle() const {
	return mPuzzles[mIndex];
}

Puzzle& PuzzleList::getPuzzle(int index) {
	assert(index >= 0 && index < mPuzzles.size());
	return mPuzzles[index];
}

const Puzzle& PuzzleList::getPuzzle(int index) const {
	assert(index >= 0 && index < mPuzzles.size());
	return mPuzzles[index];
}

size_t PuzzleList::size() const {
	return mPuzzles.size();
}

void PuzzleList::save() const {
	std::fstream file(PATH, std::ios::out | std::ios::in);
	if (!file.is_open()) {
		std::cout << "Failed to open file: " << PATH << std::endl;
		return;
	}

	file.seekp(3);
	for (const Puzzle& puzzle : mPuzzles) {
		file << puzzle.getFen() << "," << puzzle.getStringSolution() << "," << static_cast<int>(puzzle.getStatus()) << "\n";
	}
	file.close();
	std::cout << "Puzzles saved\n";
}

void PuzzleList::load() {
	std::ifstream file(PATH, std::ios::in);
	if (!file.is_open()) {
		std::cout << "Failed to open file: " << PATH << std::endl;
		return;
	}
	std::vector<Puzzle>().swap(mPuzzles);
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
		mPuzzles.emplace_back(++count, fen, solution,
		                      static_cast<Puzzle::Status>(std::stoi(status)));
		//		std::cout << puzzles.back().getSolutionSize() << std::endl;
	}
	file.close();
	std::cout << mPuzzles.size() << " puzzles loaded\n";
}