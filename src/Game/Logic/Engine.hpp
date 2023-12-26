//
// Created by MINH NHUT on 11/28/2023.
//

#ifndef CHESS_CHILL_ENGINE_HPP
#define CHESS_CHILL_ENGINE_HPP

#include "Game/Logic/GameLogic.hpp"

#include "SFML/System/Time.hpp"

#include <tuple>
#include <vector>

class Engine {
   public:
	//	static std::vector<std::pair<int, int>> getMovesAndScores(const Board& board, int color, int depth = 3);
	static Move getBestMove(const GameLogic& board, sf::Time limit, bool& flag, int depth = 3,
	                        int extra = 3);

   private:
	static int alphaBeta(const GameLogic& board, int depth, int extra, int alpha, int beta,
	                     bool maximizer, bool& flag);
	static void sortMoves(const GameLogic& board, std::vector<Move>& moves);
	static int getMoveScore(const GameLogic& board, Move move);
	static void adjustSettings(sf::Time limit, sf::Time time, int processed, int total, int& depth, int &extra);
};

#endif  //CHESS_CHILL_ENGINE_HPP
