//
// Created by MINH NHUT on 1/7/2024.
//

#ifndef CHESS_CHILL_PUZZLELIST_HPP
#define CHESS_CHILL_PUZZLELIST_HPP

#include "Puzzle.hpp"

class PuzzleList {
   public:
	static const std::string SOURCE_PATH, RESULT_PATH;

   public:
	PuzzleList();

	void load();
	void save() const;

	void nextPuzzle();
	void prevPuzzle();
	void setPuzzle(int index);

	[[nodiscard]] Puzzle& getChosenPuzzle();
	[[nodiscard]] const Puzzle& getChosenPuzzle() const;

	[[nodiscard]] Puzzle& getPuzzle(int index);
	[[nodiscard]] const Puzzle& getPuzzle(int index) const;

	[[nodiscard]] size_t size() const;

   private:
	void loadSource();
	void loadResult();
	void initResultFile() const;

   private:
	std::vector<Puzzle> mPuzzles;
	int mIndex;
};

#endif  //CHESS_CHILL_PUZZLELIST_HPP
