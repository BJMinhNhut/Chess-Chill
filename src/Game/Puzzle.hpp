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
	Puzzle(std::string fen, const std::string& solution, Status status);

	[[nodiscard]] std::string getFen() const;
	[[nodiscard]] Move getMove(int index) const;
	[[nodiscard]] Status getStatus() const;

	static std::vector<Puzzle> loadPuzzles(const std::string& path);

   private:
	std::string mFen;
	std::vector<Move> mSolution;
	Status mStatus;
};

#endif  //CHESS_CHILL_PUZZLE_HPP
