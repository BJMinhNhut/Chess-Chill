//
// Created by MINH NHUT on 1/6/2024.
//

#ifndef CHESS_CHILL_PUZZLESTATE_HPP
#define CHESS_CHILL_PUZZLESTATE_HPP

#include "GUI/Container.hpp"
#include "Template/State.hpp"

class PuzzleState : public State {
   public:
	PuzzleState(StateStack& stack, Context context);

	void draw() override;
	bool update(sf::Time dt) override;
	bool handleEvent(const sf::Event& event) override;

   private:
	void loadBasicGUI();

   private:
	GUI::Container mGUIContainer;
};

#endif  //CHESS_CHILL_PUZZLESTATE_HPP
