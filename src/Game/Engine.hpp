//
// Created by MINH NHUT on 11/28/2023.
//

#ifndef CHESS_CHILL_ENGINE_HPP
#define CHESS_CHILL_ENGINE_HPP

#include "GameLogic.hpp"

#include <SFML/System/Time.hpp>

#include <vector>
#include <tuple>

class Engine {
   public:
//	static std::vector<std::pair<int, int>> getMovesAndScores(const Board& board, int color, int depth = 3);
	static int getBestMove(const GameLogic& board, int depth = 3);
   private:
	static int alphaBeta(const GameLogic& board, int depth, int alpha, int beta, bool inTurn);
	static void sortMoves(const GameLogic& board, std::vector<int>& moves);
};

#endif  //CHESS_CHILL_ENGINE_HPP
