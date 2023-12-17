//
// Created by MINH NHUT on 12/17/2023.
//

#ifndef CHESS_CHILL_STANDARDLOGIC_HPP
#define CHESS_CHILL_STANDARDLOGIC_HPP

#include "GameLogic.hpp"
class StandardLogic: public GameLogic {
   public:
	explicit StandardLogic(GameHandler* handler);
	StandardLogic(const StandardLogic& other, GameHandler* handler);
	[[nodiscard]] StandardLogic* clone() const override;
	~StandardLogic() override;

   private:
	void updateStatus() override;

	[[nodiscard]] bool isLegalCastling(int from, int to) const override;
	[[nodiscard]] GameOptions::Type getType() const override;
};

#endif  //CHESS_CHILL_STANDARDLOGIC_HPP
