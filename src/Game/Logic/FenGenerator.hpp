//
// Created by MINH NHUT on 12/17/2023.
//

#ifndef CHESS_CHILL_FENGENERATOR_HPP
#define CHESS_CHILL_FENGENERATOR_HPP

#include <string>

class FenGenerator {
   public:
	const static std::string START_FEN;
	const static std::string ONLY_KINGS_FEN, PROMOTE_FEN_W, PROMOTE_FEN_B, PROMOTE_MATE;

   public:
	static std::string getRandom960();

   private:
	static void format960(std::string& fen);

   private:
	const static std::string FEN_960_DIR;
};

#endif  //CHESS_CHILL_FENGENERATOR_HPP
