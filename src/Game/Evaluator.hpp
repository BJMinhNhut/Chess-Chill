//
// Created by MINH NHUT on 11/28/2023.
//

#ifndef CHESS_CHILL_EVALUATOR_HPP
#define CHESS_CHILL_EVALUATOR_HPP

#include "Board.hpp"

class Evaluator {
   public:
	static const int PIECE_MATERIAL[6];
//	static const int PIECE_POSITION[6][64];

   public:
	static int evaluate(const Board& board, int color = 0);
	static int evaluatePiece(int piece);
};

#endif  //CHESS_CHILL_EVALUATOR_HPP
