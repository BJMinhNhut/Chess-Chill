//
// Created by MINH NHUT on 12/17/2023.
//

#ifndef CHESS_CHILL_CHESS960LOGIC_HPP
#define CHESS_CHILL_CHESS960LOGIC_HPP

#include "GameLogic.hpp"

class Chess960Logic: public GameLogic {
   public:
	explicit Chess960Logic(GameHandler* handler);
	Chess960Logic(const Chess960Logic& other, GameHandler* handler);
	[[nodiscard]] Chess960Logic* clone() const override;
	~Chess960Logic() override;

   private:
	void updateStatus() override;

	[[nodiscard]] GameOptions::Type getType() const override;
	[[nodiscard]] int getRook(bool color, bool side) const override;
	[[nodiscard]] bool getRookSide(int square) const override;
	[[nodiscard]] bool isLegalCastling(int from, int to) const override;

   private:
	int8_t aRookFile, hRookFile;
};

#endif  //CHESS_CHILL_CHESS960LOGIC_HPP
