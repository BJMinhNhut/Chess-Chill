//
// Created by MINH NHUT on 12/23/2023.
//

#ifndef CHESS_CHILL_KOTHLOGIC_HPP
#define CHESS_CHILL_KOTHLOGIC_HPP

#include "StandardLogic.hpp"
class KothLogic: public StandardLogic {
   public:
	explicit KothLogic(GameHandler* handler);
	KothLogic(const KothLogic& other, GameHandler* handler);
	[[nodiscard]] KothLogic* clone() const override;
	~KothLogic() override;

   private:
	[[nodiscard]] bool kingReachCenter(bool color) const;

	void updateStatus() override;
	[[nodiscard]] GameOptions::Type getType() const override;
};

#endif  //CHESS_CHILL_KOTHLOGIC_HPP
