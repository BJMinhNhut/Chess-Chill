//
// Created by MINH NHUT on 11/21/2023.
//

#ifndef CHESS_CHILL_HUMANPLAYER_HPP
#define CHESS_CHILL_HUMANPLAYER_HPP

#include "Player.hpp"

/* HumanPlayer class
 * This class is used to represent a human player
 * This class is inherited from Player class
 */
class HumanPlayer : public Player {
   public:
	HumanPlayer(GameHandler &gameHandler, int color);

	void update(sf::Time dt) override;
	void handleEvent(const sf::Event &event) override;

   private:
	void handleMousePressed(int x, int y);
	void handleMouseReleased(int x, int y);
};

#endif  //CHESS_CHILL_HUMANPLAYER_HPP
