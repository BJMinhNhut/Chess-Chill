//
// Created by MINH NHUT on 12/9/2023.
//

#ifndef CHESS_CHILL_MOVE_HPP
#define CHESS_CHILL_MOVE_HPP

#include "Piece.hpp"

class Move {
   public:
	Move(int from, int to, int promotion = Piece::Type::None);
	int from() const;
	int to() const;
	int promote() const;

   private:
	short mMove;
	int mPromote;
};

#endif  //CHESS_CHILL_MOVE_HPP
