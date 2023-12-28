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

Move Engine::getBestMove(const GameLogic& board, sf::Time limit, bool& flag, int depth, int extra) {
	assert(board.status() == GameLogic::OnGoing);
	std::cout << "\n" << (board.getTurn() ? "Black" : "White") << " - Searching for best move...\n";
	std::vector<Move> moves = board.getLegalMoves();
	if (moves.size() == 1)
		return moves[0];
	Move bestMove = moves[0];
	int bestScore = board.getTurn() ? 1000000 : -1000000;
	sortMoves(board, moves);
	sf::Time time = sf::seconds(0);
	sf::Clock clock;
	int count = 0;
	for (Move move : moves) {
		if (!flag)
			break;
		adjustSettings(limit, time, count, (int)moves.size(), depth, extra);
		++count;
		GameLogic::Ptr newBoard(board.clone());
		newBoard->makeMove(move);
		int score =
		    alphaBeta(*newBoard, depth - 1, extra, -1000000, 1000000, board.getTurn(), flag);

		if ((board.getTurn() ? score < bestScore : score > bestScore) ||
		    (abs(score - bestScore) <= 2 && Random::getInt(0, 1) == 0)) {
			bestScore = score;
			bestMove = move;
		}
		//		int from = move.from();
		//		int to = move.to();
		//		std::cout << from << ' ' << to << ' ' << score << '\n';
		time += clock.restart();
		if (time > limit) {
			std::cerr << "Time out: " + std::to_string(time.asSeconds()) + "s\n";
			break;
		}
	}
	std::cout << "Processed " << count << '/' << moves.size() << " in " << time.asSeconds()
	          << "s, score: " << bestScore << '\n';
	return bestMove;
}

void Engine::adjustSettings(sf::Time limit, sf::Time time, int processed, int total, int& depth,
                            int& extra) {
	if (processed == 0)
		return;
	static const int MIN_DEPTH = 2, MAX_DEPTH = 4;
	float timePerProcessed = (float)time.asMilliseconds() / (float)processed;
	float timePerCandidates =
	    (float)std::max(1, (limit - time).asMilliseconds()) / float(total - processed + 2);
	float ratio = timePerProcessed / timePerCandidates;
//	std::cout << "limit " << limit.asSeconds() << "s, ratio " << ratio << '\n';
	if ((ratio > 3.f || (ratio > 1.7f && extra <= 2)) && depth > MIN_DEPTH) {
		depth--;
//		std::cout << "New settings: depth = " << depth << ", extra = " << extra << '\n';
	} else if (ratio > 1.7f) {
		extra = std::max(extra - 1, 1);
//		std::cout << "New settings: depth = " << depth << ", extra = " << extra << '\n';
	} else if (ratio < 0.08f) {
		depth = std::min(depth + 1, MAX_DEPTH);
		extra = 1;
//		std::cout << "New settings: depth = " << depth << ", extra = " << extra << '\n';
	} else if (ratio < 0.3f) {
		extra = std::min(extra + 1, 5);
//		std::cout << "New settings: depth = " << depth << ", extra = " << extra << '\n';
	}
	assert(depth >= MIN_DEPTH && depth <= MAX_DEPTH);
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
                      bool maximizer, bool& flag) {

	bool isFinished = (depth <= 0 && !board.isCaptured() && !board.isChecked()) ||
	                  depth <= -extra || board.isFinished() || !flag;
	int score = 0;
	if (isFinished) {
		score = Evaluator::evaluateBoard(board);
		if (score > 0)
			score += depth + 5;
		if (score < 0)
			score -= depth + 5;
	} else {
		std::vector<Move> moves = board.getLegalMoves();
		if (moves.size() == 1) {
			if (depth > 0)
				depth++;
			else
				extra++;
		}

		sortMoves(board, moves);
		score = maximizer ? -1000000 : 1000000;
		for (auto move : moves) {
			GameLogic::Ptr newBoard(board.clone());
			newBoard->makeMove(move);
			if (maximizer) {
				score = std::max(score,
				                 alphaBeta(*newBoard, depth - 1, extra, alpha, beta, false, flag));
				alpha = std::max(alpha, score);
				if (beta <= score)
					break;
			} else {
				score = std::min(score,
				                 alphaBeta(*newBoard, depth - 1, extra, alpha, beta, true, flag));
				beta = std::min(beta, score);
				if (score <= alpha)
					break;
			}
		}
	}
	return score;
}