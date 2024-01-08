//
// Created by MINH NHUT on 1/7/2024.
//

#include "PuzzleList.hpp"
#include "Template/Constants.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

const std::string PuzzleList::SOURCE_PATH = Constants::DATA_PREFIX + "resources/puzzles.csv";
const std::string PuzzleList::RESULT_PATH = "puzzles_results.bin";

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
	std::fstream file(RESULT_PATH, std::ios::out | std::ios::in);
	if (!file.is_open()) {
		std::cout << "Failed to open file: " << RESULT_PATH << std::endl;
		return;
	}

	for (const Puzzle& puzzle : mPuzzles) {
		Puzzle::Status status = puzzle.getStatus();
		file.write((char*)&status, sizeof(Puzzle::Status));
	}
	file.close();
	std::cout << "Puzzles saved\n";
}

void PuzzleList::initResultFile() const {
	std::ofstream file(RESULT_PATH, std::ios::out | std::ios::binary);
	if (!file.is_open()) {
		throw std::invalid_argument("Failed to open file: " + RESULT_PATH);
	}
	std::cout << "Init result file\n";
	Puzzle::Status unsolved = Puzzle::Unsolved;
	for (int i = 0; i < mPuzzles.size(); i++) {
		file.write((char*)&unsolved, sizeof(Puzzle::Status));
	}
	file.close();
}

void PuzzleList::loadSource() {
	std::ifstream sourceFile(SOURCE_PATH, std::ios::in);
	if (!sourceFile.is_open()) {
		std::cout << "Failed to open sourceFile: " << SOURCE_PATH << std::endl;
		return;
	}
	std::vector<Puzzle>().swap(mPuzzles);
	std::string fen, solution;
	std::string line, word;
	std::string result; // 0: unsolved, 1: solved, 2: failed
	char status;
	int count = 0;
	sourceFile.seekg(3);
	while (std::getline(sourceFile, line)) {
		std::stringstream ss(line);

		fen.clear();
		solution.clear();

		while (std::getline(ss, word, ',')) {
			if (fen.empty()) {
				fen = word;
			} else if (solution.empty()) {
				solution = word;
			}
		}

		std::cout << fen << " " << solution << " " << std::endl;
		mPuzzles.emplace_back(++count, fen, solution, Puzzle::Unsolved);
		//		std::cout << puzzles.back().getSolutionSize() << std::endl;
	}
	sourceFile.close();
	std::cout << mPuzzles.size() << " puzzles loaded\n";
}

void PuzzleList::loadResult() {
	std::ifstream resultFile(RESULT_PATH, std::ios::in | std::ios::binary);
	if (!resultFile.is_open()) {
		std::cout << "Failed to open result: " << RESULT_PATH << std::endl;
		resultFile.close();
		initResultFile();
		resultFile.open(RESULT_PATH, std::ios::in | std::ios::binary);
	}
	for (Puzzle & mPuzzle : mPuzzles) {
		Puzzle::Status status;
		resultFile.read((char*)&status, sizeof(Puzzle::Status));
		mPuzzle.setStatus(status);
	}
	resultFile.close();
	assert(resultFile.good());
	std::cout << "Puzzle results loaded\n";
}

void PuzzleList::load() {
	loadSource();
	loadResult();
}