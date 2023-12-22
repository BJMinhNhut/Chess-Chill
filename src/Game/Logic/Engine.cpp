//
// Created by MINH NHUT on 11/28/2023.
//

#include "Engine.hpp"
#include "Evaluator.hpp"
#include "Game/Piece.hpp"
#include "Template/Random.hpp"

#include "SFML/System/Clock.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>

Move Engine::getBestMove(const GameLogic& board, int depth, int extra) {
	assert(board.status() == GameLogic::OnGoing);
	std::cout << "\n" << (board.getTurn() ? "Black" : "White") << " - Searching for best move...\n";
	std::vector<Move> moves = board.getLegalMoves();
	if (moves.size() == 1) return moves[0];
	Move bestMove = moves[0];
	int bestScore = board.getTurn() ? 1000000 : -1000000;
	sortMoves(board, moves);
	sf::Time time = sf::seconds(0);
	sf::Clock clock;
	int called = 0;
	for (Move move : moves) {
		GameLogic::Ptr newBoard(board.clone());
		newBoard->makeMove(move);
		int score = alphaBeta(*newBoard, depth - 1, extra, -1000000, 1000000, board.getTurn(), ++called);
		if ((board.getTurn() ? score < bestScore : score > bestScore) ||
		    (score == bestScore && Random::getInt(0, 1) == 0)) {
			bestScore = score;
			bestMove = move;
		}
//		int from = move.from();
//		int to = move.to();
//		std::cout << from << ' ' << to << ' ' << score << '\n';
		time += clock.restart();
		if (time > sf::seconds(10)) {
			std::cerr << "Time out: " + std::to_string(time.asSeconds()) + "s\n";
			break;
		}
	}
	std::cout << "Processed " << called << ", in " << time.asSeconds() << "s, score: " << bestScore
	          << '\n';
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
	int score = Random::getInt(0, 100);
	if (move.promote() > 0) {
		score += 2000;
	} else if (board.getPiece(to) != 0) {
		score += Evaluator::PIECE_MATERIAL[Piece::getType(pieceTo)] -
		         Evaluator::PIECE_MATERIAL[Piece::getType(pieceFrom)] / 10;
		if (to == board.getLastMovePiece())
			score += 1001;
	}
	return score;
}

int Engine::alphaBeta(const GameLogic& board, int depth, int extra, int alpha, int beta,
                      bool maximizer, int& called) {
	if (called > 0 && (called & (~(~0 << 16))) == 0) {
		std::cout << "Processed " << called << '\n';
	}
	bool isFinished = (depth <= 0 && !board.isCaptured() && !board.isChecked()) ||
	                  depth <= -extra || board.isFinished();
	int score = 0;
	if (isFinished) {
		score = Evaluator::evaluateBoard(board);
	} else {
		std::vector<Move> moves = board.getLegalMoves();
		sortMoves(board, moves);
		score = maximizer ? -1000000 : 1000000;
		for (auto move : moves) {
			GameLogic::Ptr newBoard(board.clone());
			newBoard->makeMove(move);
			if (maximizer) {
				score = std::max(
				    score, alphaBeta(*newBoard, depth - 1, extra, alpha, beta, false, ++called));
				alpha = std::max(alpha, score);
				if (beta <= score)
					break;
			} else {
				score = std::min(
				    score, alphaBeta(*newBoard, depth - 1, extra, alpha, beta, true, ++called));
				beta = std::min(beta, score);
				if (score <= alpha)
					break;
			}

		}
	}

	if (maximizer) {
		if (score > 0) score += (depth << 3);
		if (score < 0) score -= (depth << 3);
	} else {
		if (score > 0) score -= (depth << 3);
		if (score < 0) score += (depth << 3);
	}
	return score;
}