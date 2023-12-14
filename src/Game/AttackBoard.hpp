//
// Created by MINH NHUT on 11/19/2023.
//

#ifndef CHESS_CHILL_ATTACKBOARD_HPP
#define CHESS_CHILL_ATTACKBOARD_HPP

#include "Board.hpp"

class AttackBoard {
   public:
	explicit AttackBoard(Board* board);

	[[nodiscard]] bool isAttacked(int square) const;
	[[nodiscard]] bool isAttacked(int square, bool turn) const;
	[[nodiscard]] bool isKingInCheck() const;
	[[nodiscard]] bool isKingInCheck(bool turn) const;

	void update();

   private:
	void update(bool turn);

   private:
	Board* mBoard;
	int64_t mAttackBoard[2]{};  // 0: white, 1: black
};

#endif  //CHESS_CHILL_ATTACKBOARD_HPP