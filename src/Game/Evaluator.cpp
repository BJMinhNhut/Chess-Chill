//
// Created by MINH NHUT on 11/28/2023.
//

#include "Evaluator.hpp"
#include "Piece.hpp"

#include <cassert>

// King, Queen, Bishop, Knight, Rook, Pawn
const int Evaluator::PIECE_MATERIAL[6] = {20000, 900, 340, 325, 500, 100};

const int Evaluator::PAWN_SCORE_POSITIONAL[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {4, 9, 10, 3, 3, 16, 11, 4},
    {4, 5, 8, 16, 16, 0, 0, 4},
    {5, 6, 16, 26, 26, 0, 0, 5},
    {6, 7, 18, 28, 28, 14, 7, 6},
    {7, 10, 22, 29, 29, 22, 10, 7},
    {8, 12, 26, 30, 30, 26, 12, 8},
    {9, 14, 30, 34, 34, 30, 14, 9}};

const int Evaluator::KNIGHT_SCORE_POSITIONAL[8][8] = {{-22, -10, -10, -10, -10, -10, -10, -22},
                                                      {-10, 0, 5, 0, 0, 5, 0, -10},
                                                      {-7, 2, 7, 8, 8, 7, 2, -7},
                                                      {-5, 5, 8, 11, 11, 8, 5, -5},
                                                      {-5, 5, 8, 11, 11, 8, 5, -5},
                                                      {-7, 2, 7, 8, 8, 7, 2, -7},
                                                      {-11, 0, 11, 0, 0, 11, 0, -11},
                                                      {-22, -11, -5, -5, -5, -5, -11, -22}};

const int Evaluator::BISHOP_SCORE_POSITIONAL[8][8] = {
    {-20, -10, -10, -10, -10, -10, -10, -20},
    {-10, 15, 0, 5, 5, 0, 15, -10},
    {-5, 10, 5, 2, 2, 5, 10, -5},
    {-2, 0, 7, 2, 2, 7, 0, -2},
    {-2, 5, 4, 0, 0, 4, 5, -2},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {-10, 0, 0, 0, 0, 0, 0, -10},
    {-20, -10, -5, -5, -5, -5, -10, -20}};

const int Evaluator::ROOK_SCORE_POSITIONAL[8][8] = {
    {5, 0, 0, 11, 15, 11, 0, 5},      {5, 7, 0, 0, 0, 0, 7, 5},        {5, 7, 0, 0, 0, 0, 7, 5},
    {5, 7, 0, 0, 0, 0, 7, 5},         {5, 7, 0, 0, 0, 0, 7, 5},        {5, 7, 0, 0, 0, 0, 7, 5},
    {20, 20, 20, 20, 20, 20, 20, 20}, {10, 10, 10, 10, 10, 10, 10, 10}};

const int Evaluator::KING_SCORE_POSITIONAL[8][8] = {
    {22, 47, 37, 0, 0, 11, 53, 22}, {22, 11, 0, 0, 0, 0, 11, 22}, {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},       {0, 0, 0, 0, 0, 0, 0, 0},     {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},       {0, 0, 0, 0, 0, 0, 0, 0}};
int Evaluator::evaluateBoard(const GameLogic& board, int color) {
	// if is king in check, then bonus 60
	if (board.status() == GameLogic::Checkmate) {
		if (board.getTurn() == color)
			return -1000000;
		else
			return 1000000;
	} else if (board.status() & (GameLogic::Stalemate | GameLogic::ThreefoldRepetition |
	                             GameLogic::InsufficientMaterial)) {
		return 0;
	}

	int score = 0;
	if (board.isChecked()) {
		if (board.getTurn() == color)
			score -= 60;
		else
			score += 60;
	}

	std::vector<int> bishops = {0, 0};
	for (int i = 0; i < 64; i++) {
		int piece = board.getPiece(i);
		if (piece == 0)
			continue;
		if (Piece::getColor(piece) == color) {
			score += evaluatePiece(piece, i);
			if (Piece::getType(piece) == Piece::Bishop)
				bishops[color]++;
		} else {
			score -= evaluatePiece(piece, i);
			if (Piece::getType(piece) == Piece::Bishop)
				bishops[!color]++;
		}
	}
	score += distanceToKing(board, color);
	score += checkCastling(board, color);
	if (bishops[color] == 2)
		score += 20;
	if (bishops[!color] == 2)
		score -= 20;
	return score;
}

int Evaluator::checkCastling(const GameLogic& board, int color) {
	int score = 0;
	int castling = board.getCastling();
	if (castling & 1) {
		score += 12;
	}
	if (castling & 2) {
		score += 8;
	}
	if (castling & 4) {
		score -= 12;
	}
	if (castling & 8) {
		score -= 8;
	}
	return color ? -score : score;
}

int Evaluator::distanceToKing(const GameLogic& board, int color) {
	std::vector<int> king = {board.getKing(color), board.getKing(!color)};
	int score = 0;
	for (int i = 0; i < 64; i++) {
		int piece = board.getPiece(i);
		if (piece == 0 || Piece::getType(piece) == Piece::King)
			continue;
		int bonus = 0;
		int opKing = king[!Piece::getColor(piece)];
		switch (Piece::getType(piece)) {
			case Piece::Queen:
				bonus = 6 * std::min(std::abs(i - opKing) >> 3, (std::abs(i - opKing) & 7));
				break;
			case Piece::Rook:
				bonus = 3 * std::min(std::abs(i - opKing) >> 3, (std::abs(i - opKing) & 7));
				break;
			case Piece::Knight:
				bonus = ((std::abs(i - opKing) >> 3) + (std::abs(i - opKing) & 7));
				break;
			default:
				break;
		}
		if (Piece::getColor(piece) == color)
			score -= bonus;
		else
			score += bonus;
	}
	return score;
}

int Evaluator::evaluatePiece(int piece, int square) {
	int color = Piece::getColor(piece);
	int type = Piece::getType(piece);
	int score = PIECE_MATERIAL[type - 1];
	int rank = square >> 3;
	int file = square & 7;
	if (color)
		rank = 7 - rank;

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
		case Piece::King:
			score += KING_SCORE_POSITIONAL[rank][file];
			break;
		default:
			break;
			//		case Piece::Queen:
			//			score += 900;
			//			break;
	}
	return score;
}