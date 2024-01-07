//
// Created by MINH NHUT on 1/6/2024.
//

#ifndef CHESS_CHILL_PUZZLESTATE_HPP
#define CHESS_CHILL_PUZZLESTATE_HPP

#include "GUI/Container.hpp"
#include "Game/GameHandler.hpp"
#include "Template/State.hpp"
#include "Game/HumanPlayer.hpp"

class PuzzleState : public State {
   public:
	const static sf::Vector2f BOARD_POSITION;

   public:
	PuzzleState(StateStack& stack, Context context);

	void draw() override;
	bool update(sf::Time dt) override;
	bool handleEvent(const sf::Event& event) override;

   private:
	void loadBasicGUI();
	void loadFinishGUI();

   private:
	GUI::Container mGUIContainer, mFinishContainer;
	GameHandler mGame;
	HumanPlayer mPlayer;
	sf::Time mCoolDown;
	int currentMove;
};

#endif  //CHESS_CHILL_PUZZLESTATE_HPP
