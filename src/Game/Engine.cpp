//
// Created by MINH NHUT on 11/28/2023.
//

#include "Engine.hpp"
#include "Evaluator.hpp"
#include "Template/Random.hpp"

#include <SFML/System/Clock.hpp>

#include <algorithm>
#include <iostream>

int Engine::getBestMove(const GameLogic& board, int depth) {
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
		if (score > bestScore) {
			bestScore = score;
			bestMove = move;
		}
		time += clock.restart();
		if (time > sf::seconds(3)) {
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
		GameLogic newBoard(board);
		newBoard.makeMove(from, to);
		scores.emplace_back(Evaluator::evaluateBoard(newBoard, board.getTurn()), move);
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
	if (board.status() == GameLogic::Checkmate) {
		return inTurn ? 1000000 : -1000000;
	}
	if (board.status() &
	    (GameLogic::Stalemate | GameLogic::InsufficientMaterial | GameLogic::ThreefoldRepetition)) {
		return 0;
	}
	if (depth == 0) {
		return Evaluator::evaluateBoard(board, inTurn ? board.getTurn() : !board.getTurn());
	}
	std::vector<int> moves = board.getLegalMoves();
	Random::shuffle(moves);
	int score = inTurn ? -1000000 : 1000000;
	for (auto move : moves) {
		int from = move & 0x3F;
		int to = (move >> 6) & 0x3F;
		GameLogic newBoard(board);
		newBoard.makeMove(from, to);
		if (inTurn) {
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
	return score + (inTurn ? (100 - (int)moves.size()) * 10 : -(100 - (int)moves.size()) * 10);
}