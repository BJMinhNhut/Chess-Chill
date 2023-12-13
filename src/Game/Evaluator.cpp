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
    {3, 10, 10, -10, -10, 21, 19, 6},
    {4, 4, 5, 15, 15, 5, 0, 6},
    {13, 15, 15, 24, 26, 6, 6, 0},
    {17, 19, 20, 26, 28, 9, 12, 10},
    {21, 24, 25, 30, 30, 25, 24, 21},
    {26, 28, 29, 32, 32, 29, 28, 26},
    {31, 32, 33, 34, 34, 33, 32, 31}};

const int Evaluator::KNIGHT_SCORE_POSITIONAL[8][8] = {{-11, -6, -6, -2, -2, -7, -7, -11},
                                                      {-7, -5, 5, -1, -1, 5, -5, -7},
                                                      {-5, 2, 9, 8, 8, 10, 2, -5},
                                                      {-2, 5, 10, 11, 12, 11, 5, -2},
                                                      {-2, 9, 11, 12, 13, 12, 9, -2},
                                                      {-5, 2, 10, 9, 9, 11, 2, -5},
                                                      {-7, -5, 7, 0, 0, 7, -5, -7},
                                                      {-10, -7, -5, -5, -5, -5, -7, -10}};

const int Evaluator::BISHOP_SCORE_POSITIONAL[8][8] = {{-12, -6, -6, -2, -2, -7, -7, -12},
                                                      {5, 14, 5, 0, 0, 5, 14, 5},
                                                      {-6, 10,  8, -5, -5, 8, 10, -6},
                                                      {-7, 4, 7, 2, 2, 7, 4, -7},
                                                      {-7, 3, 2, 1, 1, 2, 3, -7},
                                                      {-6, 0, 0, 0, 0, 0, 0, -6},
                                                      {-6, 0, 0, 0, 0, 0, 0, -6},
                                                      {-8, -7, -5, -4, -4, -5, -7, -8}};

const int Evaluator::ROOK_SCORE_POSITIONAL[8][8] = {
    {5, 0, 0, 11, 15, 11, 0, 5},      {5, 7, 0, 0, 0, 0, 7, 5},        {5, 7, 0, 0, 0, 0, 7, 5},
    {5, 7, 0, 0, 0, 0, 7, 5},         {5, 7, 0, 0, 0, 0, 7, 5},        {5, 7, 0, 0, 0, 0, 7, 5},
    {20, 20, 20, 20, 20, 20, 20, 20}, {10, 10, 10, 10, 10, 10, 10, 10}};

const int Evaluator::KING_SCORE_POSITIONAL[8][8] = {
    {47,  49, 20, 30, 30, 20, 53, 51},
    {0, 40, 0, -100, -100, 0, 40, 0},
    {50, -50, -200, -200, -200, -200, -50, 50},
    {50, -50, -200, -300, -300, -200, -50, 0},
    {-50, -50, -200, -300, -300, -200, -50, -50},
    {-10, -30, -200, -200, -200, -100, -100, -10},
    {-50, -50, -100, -100, -100, -100, -50, -50},
    {-60, -60, -60, -60, -60, -60, -60, -60}};

int Evaluator::evaluateBoard(const GameLogic& board, int color) {
	// if is king in check, then bonus 60
	if (board.status() == GameLogic::Checkmate) {
		if (board.getTurn() == color)
			return -10000000;
		else
			return 10000000;
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
				bonus = -9 * std::min((std::abs(i - opKing) >> 3), (std::abs(i - opKing) & 7));
				break;
			case Piece::Rook:
				bonus = -4 * std::min(std::abs(i - opKing) >> 3, (std::abs(i - opKing) & 7));
				break;
			case Piece::Knight:
				bonus = -2 * ((std::abs(i - opKing) >> 3) + (std::abs(i - opKing) & 7));
				break;
			default:
				break;
		}
		if (Piece::getColor(piece) == color)
			score += bonus;
		else
			score -= bonus;
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

	}
	return score;
}