//
// Created by MINH NHUT on 11/28/2023.
//

#include "Evaluator.hpp"
#include "Piece.hpp"

// King, Queen, Bishop, Knight, Rook, Pawn
const int Evaluator::PIECE_MATERIAL[6] = {20000, 900, 340, 325, 500, 100};

const int Evaluator::PAWN_SCORE_POSITIONAL[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {10, 10, 10, -2, -2, 10, 10, 10},
    {5, 1, 10, 10, 10, -20, 1, 5},
    {3, 1, 5, 50, 50, -20, 1, 3},
    {0, 0, 0, 10, 10, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {10, 10, 10, 10, 10, 10, 10, 10},
    {10, 10, 10, 10, 10, 10, 10, 10}
};

const int Evaluator::KNIGHT_SCORE_POSITIONAL[8][8] = {
    {-100, -50, -5, -5, -5, -5, -50, -100},
    {-10, 0, 10, 0, 0, 10, 0, -50},
    {-5, 2, 30, 10, 10, 30, 2, -5},
    {0, 5, 20, 50, 50, 20, 5, 0},
    {0, 5, 20, 50, 50, 20, 5, 0},
    {-5, 2, 20, 10, 10, 20, 2, -5},
    {-50, 0, 30, 0, 0, 30, 0, -50},
    {-100, -50, -5, -5, -5, -5, -50, -100}
};

const int Evaluator::BISHOP_SCORE_POSITIONAL[8][8] = {
	{-50, -10, -5, -5, -5, -5, -10, -50},
	{-10, 50, 0, 5, 5, 0, 50, -10},
	{-5, 30, 20, 10, 10, 20, 30, -5},
	{0, 5, 30, 5, 5, 30, 5, 0},
	{0, 20, 10, 0, 0, 10, 20, 0},
	{-5, 0, 0, 0, 0, 0, 0, -5},
	{-10, 0, 0, 0, 0, 0, 0, -10},
	{-50, -10, -5, -5, -5, -5, -10, -50}
};

const int Evaluator::ROOK_SCORE_POSITIONAL[8][8] = {
    {10, 10, 10, 10, 10, 10, 10, 10},
    {5, 0, 0, 0, 0,  0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 5},
    {100, 100, 100, 100, 100, 100, 100, 100},
    {50, 50, 50, 50, 50, 50, 50, 50}
};

int Evaluator::evaluateBoard(const GameLogic& board, int color) {
	int score = 0;
	for (int i = 0; i < 64; i++) {
		int piece = board.getPiece(i);
		if (piece == 0)
			continue;
		if (Piece::getColor(piece) == color) {
			score += evaluatePiece(piece, i);
		} else {
			score -= evaluatePiece(piece, i);
		}
	}
	return score;
}

int Evaluator::evaluatePiece(int piece, int square) {
	int color = Piece::getColor(piece);
	int type = Piece::getType(piece);
	int score = PIECE_MATERIAL[type - 1];
	int rank = square >> 3;
	int file = square & 7;
	if (color) rank = 7 - rank;

	switch (type) {
		case Piece::Pawn:
			score += PAWN_SCORE_POSITIONAL[rank][file];
			break;
		case Piece::Knight:
			score += KNIGHT_SCORE_POSITIONAL[rank][file];
			break;
		case Piece::Bishop:
			score += BISHOP_SCORE_POSITIONAL[rank][file];
			break;
		case Piece::Rook:
			score += ROOK_SCORE_POSITIONAL[rank][file];
			break;
		default:
			break;
			//		case Piece::Rook:
			//			score += 500;
			//			break;
			//		case Piece::Queen:
			//			score += 900;
			//			break;
			//		case Piece::King:
			//			score += 20000;
			//			break;
	}
	return score;
}