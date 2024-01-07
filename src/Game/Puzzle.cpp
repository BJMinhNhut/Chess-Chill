//
// Created by MINH NHUT on 1/5/2024.
//

#include "Puzzle.hpp"
#include "Move.hpp"

#include <string>

Puzzle::Puzzle(int id, std::string fen, const std::string& solution, Status status)
    : mFen(std::move(fen)), mStringSolution(solution), mSolution(), mStatus(status), mId((int8_t)id) {
	std::string move;
	for (int i = 0; i < solution.size(); ++i) {
		if (solution[i] == ' ') {
			mSolution.emplace_back(move);
			move.clear();
		} else {
			move += solution[i];
		}
	}
	mSolution.emplace_back(move);
}

Puzzle::Puzzle() : mFen(), mSolution(), mStringSolution(), mStatus(Unsolved), mId() {}

std::string Puzzle::getFen() const {
	return mFen;
}

std::string Puzzle::getStringSolution() const {
	return mStringSolution;
}

Move Puzzle::getMove(int index) const {
	return mSolution[index];
}

Puzzle::Status Puzzle::getStatus() const {
	return mStatus;
}

void Puzzle::setStatus(Puzzle::Status status) {
	mStatus = status;
}

int8_t Puzzle::getId() const {
	return mId;
}

size_t Puzzle::getSolutionSize() const {
	return mSolution.size();
}
