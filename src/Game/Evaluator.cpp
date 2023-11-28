//
// Created by MINH NHUT on 11/28/2023.
//

#include "Evaluator.hpp"
#include "Piece.hpp"

// King, Queen, Bishop, Knight, Rook, Pawn
const int Evaluator::PIECE_MATERIAL[6] = {20000, 900, 330, 320, 500, 100};

int Evaluator::evaluate(const Board &board, int color) {
	int score = 0;
	for (int i = 0; i < 64; i++) {
		int piece = board.get(i);
		if (piece == 0) continue;
		if (Piece::getColor(piece) == color) {
			score += evaluatePiece(piece);
		} else {
			score -= evaluatePiece(piece);
		}
	}
	return score;
}

int Evaluator::evaluatePiece(int piece) {
	int color = Piece::getColor(piece);
	int type = Piece::getType(piece);
	int score = PIECE_MATERIAL[type - 1];
	return score;
}