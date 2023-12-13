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

Move Engine::getBestMove(const GameLogic& board, int depth, int extra) {
	assert(board.status() == GameLogic::OnGoing);
//	std::cout << (board.getTurn() ? "White" : "Black") << " to move\n";
	std::vector<Move> moves = board.getLegalMoves();
	Move bestMove = moves[0];
	int bestScore = -1000000;
	sortMoves(board, moves);
	sf::Time time = sf::seconds(0);
	sf::Clock clock;
	for (Move move : moves) {
		int from = move.from();
		int to = move.to();
//		std::cout << from << ' ' << to << '\n';
		GameLogic newBoard(board);
		newBoard.makeMove(move);
		int score = alphaBeta(newBoard, depth - 1, extra, -1000000, 1000000, false);
		if (score > bestScore || (score == bestScore && Random::getInt(0, 1))) {
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

void Engine::sortMoves(const GameLogic& board, std::vector<Move>& moves) {
	std::vector<std::pair<int, Move>> scores;
	for (Move move : moves) {
		int score = getMoveScore(board, move);
		scores.emplace_back(score, move);
	}
	std::sort(scores.begin(), scores.end(),
	          [](const std::pair<int, Move>& a, const std::pair<int, Move>& b) {
		          return a.first > b.first;
	          });
	moves.clear();
	for (auto& score : scores) {
		moves.push_back(score.second);
	}
}

int Engine::getMoveScore(const GameLogic& board, Move move) {
	int from = move.from();
	int to = move.to();
	int pieceFrom = board.getPiece(from);
	int pieceTo = board.getPiece(to);
	int score = 0;
	if (move.promote() > 0) {
		score = 4000;
	} else if (board.getPiece(to) != 0) {
		score += Evaluator::PIECE_MATERIAL[Piece::getType(pieceTo)] -
		         Evaluator::PIECE_MATERIAL[Piece::getType(pieceFrom)] / 10;
		if (to == board.getLastMovePiece())
			score += 1001;
	} else {
		GameLogic newBoard(board);
		newBoard.makeMove(move);
		score = Evaluator::evaluateBoard(newBoard, board.getTurn());
		if (newBoard.isChecked())
			score += 2000;
	}
	return score;
}

int Engine::alphaBeta(const GameLogic& board, int depth, int extra, int alpha, int beta,
                      bool inTurn) {
	bool isFinished = (depth <= 0 && !board.isCaptured() && !board.isChecked()) ||
	                  depth <= -extra || board.isFinished();
	int score = 0;
	if (isFinished) {
		score = Evaluator::evaluateBoard(board, inTurn ? board.getTurn() : !board.getTurn());
	} else {
		std::vector<Move> moves = board.getLegalMoves();
		Random::shuffle(moves);
		score = inTurn ? -1000000 : 1000000;
		for (auto move : moves) {
			GameLogic newBoard(board);
			newBoard.makeMove(move);
			if (inTurn) {
				score = std::max(score, alphaBeta(newBoard, depth - 1, extra, alpha, beta, false));
				alpha = std::max(alpha, score);
				if (beta <= score)
					break;
			} else {
				score = std::min(score, alphaBeta(newBoard, depth - 1, extra, alpha, beta, true));
				beta = std::min(beta, score);
				if (score <= alpha)
					break;
			}
		}
	}

	if (score < 0)
		score -= depth << 2;
	else
		score += depth << 2;
	return score;
}