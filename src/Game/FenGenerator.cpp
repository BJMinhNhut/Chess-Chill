//
// Created by MINH NHUT on 12/17/2023.
//

#include "FenGenerator.hpp"
#include "Template/Random.hpp"
#include "Template/Constants.hpp"

#include <fstream>
#include <iostream>

const std::string FenGenerator::START_FEN =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const std::string FenGenerator::ONLY_KINGS_FEN = "k7/8/8/8/8/8/8/7K w - - 0 1";
const std::string FenGenerator::PROMOTE_FEN_W = "8/7P/8/k7/7K/8/8/8 w - - 0 1";
const std::string FenGenerator::PROMOTE_FEN_B = "8/8/8/7k/K7/8/7p/8 b - - 0 1";
const std::string FenGenerator::PROMOTE_MATE = "8/5P1k/8/3B3K/3B4/8/8/8 w - - 0 1";

const std::string FenGenerator::FEN_960_DIR = Constants::DATA_PREFIX + "resources/chess960_start_positions.txt";

std::string FenGenerator::getRandom960() {
	std::ifstream file(FEN_960_DIR);
	std::string fen;
	int random = Random::getInt(0, 959);
	for (int i = 0; i < random; ++i) {
		std::getline(file, fen);
	}
	format960(fen);
	std::cout << "960 Fen: " << fen << '\n';
	return fen;
}

void FenGenerator::format960(std::string& fen) {
	while (std::isdigit(fen.back())) fen.pop_back();
	fen += " 0 1";
}
