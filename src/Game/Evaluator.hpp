//
// Created by MINH NHUT on 11/28/2023.
//

#ifndef CHESS_CHILL_EVALUATOR_HPP
#define CHESS_CHILL_EVALUATOR_HPP

#include "GameLogic.hpp"

class Evaluator {
   public:
	static const int PIECE_MATERIAL[6];
	static const int PAWN_SCORE_POSITIONAL[8][8];
	static const int KNIGHT_SCORE_POSITIONAL[8][8];
	static const int BISHOP_SCORE_POSITIONAL[8][8];
	static const int ROOK_SCORE_POSITIONAL[8][8];

   public:
	static int evaluateBoard(const GameLogic& board, int color = 0);
	static int evaluatePiece(int piece, int square);
};

#endif  //CHESS_CHILL_EVALUATOR_HPP
