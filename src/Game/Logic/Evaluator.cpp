//
// Created by MINH NHUT on 11/28/2023.
//

#include "Evaluator.hpp"
#include "Game/Piece.hpp"

#include <cassert>

// King, Queen, Bishop, Knight, Rook, Pawn
const int Evaluator::PIECE_MATERIAL[6] = {20000, 900, 340, 325, 500, 100};

const int Evaluator::PAWN_SCORE_POSITIONAL[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},         {3, 10, 15, -5, -5, 15, 19, 6},
    {7, 4, 5, 15, 15, 5, 0, 6},       {6, 15, 15, 24, 26, 6, 6, 0},
    {17, 19, 20, 26, 28, 9, 12, 10},  {21, 24, 25, 30, 30, 25, 24, 21},
    {26, 28, 29, 32, 32, 29, 28, 26}, {31, 32, 33, 34, 34, 33, 32, 31}};

const int Evaluator::KNIGHT_SCORE_POSITIONAL[8][8] = {
    {-11, -9, -9, -4, -4, -10, -10, -11}, {-7, -5, 5, -1, -1, 5, -5, -7},
    {-5, 2, 13, 8, 8, 15, 2, -5},         {-2, 3, 11, 15, 16, 12, 3, -2},
    {-2, 4, 12, 16, 17, 13, 4, -2},       {-5, 2, 13, 9, 9, 14, 2, -5},
    {-7, -5, 0, 2, 2, 0, -5, -7},         {-10, -7, -5, -5, -5, -5, -7, -10}};

const int Evaluator::BISHOP_SCORE_POSITIONAL[8][8] = {
    {-11, -9, -9, -2, -2, -10, -10, -11}, {5, 14, 5, 3, 3, 5, 14, 5},
    {7, 10, 8, -2, -2, 8, 10, 7},         {-3, 4, 7, 2, 2, 7, 4, -3},
    {-3, 2, 1, 1, 1, 1, 2, -3},           {-4, 0, 0, 0, 0, 0, 0, -4},
    {-6, 0, 0, 0, 0, 0, 0, -6},           {-8, -7, -5, -4, -4, -5, -7, -8}};

const int Evaluator::ROOK_SCORE_POSITIONAL[8][8] = {
    {5, 0, 0, 11, 15, 11, 0, 5},      {5, 7, 0, 0, 0, 0, 7, 5},        {5, 7, 0, 0, 0, 0, 7, 5},
    {5, 7, 0, 0, 0, 0, 7, 5},         {5, 7, 0, 0, 0, 0, 7, 5},        {5, 7, 0, 0, 0, 0, 7, 5},
    {20, 20, 20, 20, 20, 20, 20, 20}, {10, 10, 10, 10, 10, 10, 10, 10}};

const int Evaluator::KING_SCORE_POSITIONAL[8][8] = {{47, 49, 5, 10, 10, 5, 53, 51},
                                                    {0, 40, 0, -100, -100, 0, 40, 0},
                                                    {50, -50, -200, -200, -200, -200, -50, 50},
                                                    {50, -50, -200, -300, -300, -200, -50, 0},
                                                    {-50, -50, -200, -300, -300, -200, -50, -50},
                                                    {-10, -30, -200, -200, -200, -100, -100, -10},
                                                    {-50, -50, -100, -100, -100, -100, -50, -50},
                                                    {-60, -60, -60, -60, -60, -60, -60, -60}};

int Evaluator::evaluateBoard(const GameLogic& board) {
	// if is king in check, then bonus 60
	if (board.status() == GameLogic::Checkmate) {
		return board.getTurn() ? 1000000 : -1000000;
	} else if (board.status() & (GameLogic::Stalemate | GameLogic::ThreefoldRepetition |
	                             GameLogic::InsufficientMaterial | GameLogic::FiftyMoveRule)) {
		return 0;
	}

	int score = 0;
	if (board.isChecked()) {
		score += board.getTurn() ? -60 : 60;
	}

	std::vector<int> bishops = {0, 0};
	for (int i = 0; i < 64; i++) {
		int piece = board.getPiece(i);
		if (piece == 0)
			continue;

		if (Piece::getColor(piece))
			score -= evaluatePiece(piece, i);
		else
			score += evaluatePiece(piece, i);

		if (Piece::getType(piece) == Piece::Bishop)
			bishops[Piece::getColor(piece)]++;
	}
	score += distanceToKing(board);
	score += checkCastling(board);
	if (bishops[0] == 2)
		score += 20;
	if (bishops[1] == 2)
		score -= 20;
	return score;
}

int Evaluator::checkCastling(const GameLogic& board) {
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
	return score;
}

int Evaluator::distanceToKing(const GameLogic& board) {
	std::vector<int> king = {board.getKing(0), board.getKing(1)};
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
		if (Piece::getColor(piece))
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
	}
	return score;
}