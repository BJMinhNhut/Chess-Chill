//
// Created by MINH NHUT on 11/28/2023.
//

#include "Engine.hpp"
#include "Evaluator.hpp"
#include "Piece.hpp"
#include "Template/Random.hpp"

#include <SFML/System/Clock.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>

int Engine::getBestMove(const GameLogic& board, int depth) {
	assert(board.status() == GameLogic::OnGoing);
	std::cout << (board.getTurn() ? "White" : "Black") << " to move\n";
	std::vector<int> moves = board.getLegalMoves();
	int bestMove = moves[0];
	int bestScore = -1000000;
	sortMoves(board, moves);
	sf::Time time = sf::seconds(0);
	sf::Clock clock;
	for (int move : moves) {
		int from = move & 0x3F;
		int to = (move >> 6) & 0x3F;
		std::cout << from << ' ' << to << '\n';
		GameLogic newBoard(board);
		newBoard.makeMove(from, to);
		int score = alphaBeta(newBoard, depth - 1, -1000000, 1000000, false);
		if (score > bestScore + 8 ||
		    (score >= bestScore && score <= bestScore + 8 && Random::getInt(0, 2))) {
			bestScore = score;
			bestMove = move;
		}
		time += clock.restart();
		if (time > sf::seconds(10)) {
			break;
		}
	}
	return bestMove;
}

void Engine::sortMoves(const GameLogic& board, std::vector<int>& moves) {
	std::vector<std::pair<int, int>> scores;
	for (int move : moves) {
		int from = move & 0x3F;
		int to = (move >> 6) & 0x3F;
		int pieceFrom = board.getPiece(from);
		int pieceTo = board.getPiece(to);
		int score = 0;
		if (board.getPiece(from) == board.getSecondLastMovePiece())
			score += 4000;
		if (board.getPiece(to) != 0) {
			score += Evaluator::PIECE_MATERIAL[Piece::getType(pieceTo)] -
			         Evaluator::PIECE_MATERIAL[Piece::getType(pieceFrom)] / 10;
			if (to == board.getLastMovePiece())
				score += 1001;
		} else {
			GameLogic newBoard(board);
			newBoard.makeMove(from, to);
			score = Evaluator::evaluateBoard(newBoard, board.getTurn());
			if (newBoard.isChecked())
				score += 2000;
		}
		scores.emplace_back(score, move);
	}
	std::sort(scores.begin(), scores.end(),
	          [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
		          return a.first > b.first;
	          });
	moves.clear();
	for (auto& score : scores) {
		moves.push_back(score.second);
	}
}

int Engine::alphaBeta(const GameLogic& board, int depth, int alpha, int beta, bool inTurn) {
	bool isFinished = (depth <= 0 && !board.isCaptured() && !board.isChecked()) || (depth <= -2 && !board.isCaptured()) ||
	                  (depth <= -4 && !board.isChecked()) || board.isFinished();
	int score = 0;
	if (isFinished) {
		score = Evaluator::evaluateBoard(board, inTurn ? board.getTurn() : !board.getTurn());
	} else {
		std::vector<int> moves = board.getLegalMoves();
		Random::shuffle(moves);
		score = inTurn ? -1000000 : 1000000;
		for (auto move : moves) {
			int from = move & 0x3F;
			int to = (move >> 6) & 0x3F;
			GameLogic newBoard(board);
			newBoard.makeMove(from, to);
			if (inTurn) {
				score = std::max(score, alphaBeta(newBoard, depth - 1, alpha, beta, false));
				alpha = std::max(alpha, score);
				if (beta <= score)
					break;
			} else {
				score = std::min(score, alphaBeta(newBoard, depth - 1, alpha, beta, true));
				beta = std::min(beta, score);
				if (score <= alpha)
					break;
			}
		}
	}

	if (score < 0)
		score -= depth*5;
	else
		score += depth*5;
	return score;
}