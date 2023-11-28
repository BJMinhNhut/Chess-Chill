//
// Created by MINH NHUT on 11/28/2023.
//

#include "Engine.hpp"
#include "Evaluator.hpp"
#include "Template/Random.hpp"

#include <iostream>

int Engine::getBestMove(const GameLogic& board, int depth) {
	std::cout << (board.getTurn() ? "White" : "Black") << " to move\n";
	std::vector<int> moves = board.getLegalMoves();
	int bestMove = moves[0];
	int bestScore = -1000000;
	Random::shuffle(moves);
	for (int move : moves) {
		int from = move & 0x3F;
		int to = (move >> 6) & 0x3F;
		std::cout << from << ' ' << to << '\n';
		GameLogic newBoard(board);
		newBoard.makeMove(from, to);
		int score = alphaBeta(newBoard, depth - 1, -1000000, 1000000, false);
		if (score > bestScore) {
			bestScore = score;
			bestMove = move;
		}
	}
	return bestMove;
}

int Engine::alphaBeta(const GameLogic& board, int depth, int alpha, int beta, bool maximize) {
	if (board.status() == GameLogic::Checkmate) {
		return -1000000;
	}
	if (board.status() & (GameLogic::Stalemate|GameLogic::InsufficientMaterial|GameLogic::ThreefoldRepetition)) {
		return 0;
	}
	if (depth == 0) {
		return Evaluator::evaluateBoard(board, board.getTurn());
	}
	std::vector<int> moves = board.getLegalMoves();
	Random::shuffle(moves);
	int score = maximize ? -1000000 : 1000000;
	for (auto move : moves) {
		int from = move & 0x3F;
		int to = (move >> 6) & 0x3F;
		GameLogic newBoard(board);
		newBoard.makeMove(from, to);
		if (maximize) {
			score = std::max(score, alphaBeta(newBoard, depth - 1, alpha, beta, false));
			if (beta < score) {
				break;
			}
			alpha = std::max(alpha, score);
		} else {
			score = std::min(score, alphaBeta(newBoard, depth - 1, alpha, beta, true));
			if (score < alpha) {
				break;
			}
			beta = std::min(beta, score);
		}
	}
	return score;
}