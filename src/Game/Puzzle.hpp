//
// Created by MINH NHUT on 1/5/2024.
//

#ifndef CHESS_CHILL_PUZZLE_HPP
#define CHESS_CHILL_PUZZLE_HPP

#include "Move.hpp"

#include <string>
#include <vector>

class Puzzle {
   public:
	enum Status {
		Unsolved,
		Solved,
		Failed,
	};

   public:
	Puzzle();
	Puzzle(int id, std::string fen, const std::string& solution, Status status);

	[[nodiscard]] std::string getFen() const;
	[[nodiscard]] std::string getStringSolution() const;
	[[nodiscard]] Move getMove(int index) const;
	[[nodiscard]] Status getStatus() const;
	[[nodiscard]] int8_t getId() const;
	[[nodiscard]] size_t getSolutionSize() const;

   private:
	int8_t mId;
	std::string mFen, mStringSolution;
	std::vector<Move> mSolution;
	Status mStatus;
};

#endif  //CHESS_CHILL_PUZZLE_HPP
