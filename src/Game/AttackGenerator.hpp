//
// Created by MINH NHUT on 11/13/2023.
//

#ifndef CHESS_ATTACKGENERATOR_HPP
#define CHESS_ATTACKGENERATOR_HPP

#include "Board.hpp"

#include <cstdint>

namespace AttackGenerator {
int64_t generatePawnAttacks(int square, bool color);

int64_t generateKnightAttacks(int square);

int64_t generateBishopAttacks(int square, const Board& board);

int64_t generateRookAttacks(int square, const Board& board);

int64_t generateQueenAttacks(int square, const Board& board);

int64_t generateKingAttacks(int square);
}  // namespace AttackGenerator

#endif  //CHESS_ATTACKGENERATOR_HPP
