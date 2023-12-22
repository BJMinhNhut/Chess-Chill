//
// Created by MINH NHUT on 11/28/2023.
//

#ifndef CHESS_CHILL_EVALUATOR_HPP
#define CHESS_CHILL_EVALUATOR_HPP

#include "Game/Logic/GameLogic.hpp"

class Evaluator {
   public:
	static const int PIECE_MATERIAL[6];
	static const int PAWN_SCORE_POSITIONAL[8][8];
	static const int KNIGHT_SCORE_POSITIONAL[8][8];
	static const int BISHOP_SCORE_POSITIONAL[8][8];
	static const int ROOK_SCORE_POSITIONAL[8][8];
	static const int KING_SCORE_POSITIONAL[8][8];

   public:
	static int evaluateBoard(const GameLogic& board);

   private:
	static int evaluatePiece(int piece, int square);
	static int distanceToKing(const GameLogic& board);
	static int checkCastling(const GameLogic& board);
};

#endif  //CHESS_CHILL_EVALUATOR_HPP
