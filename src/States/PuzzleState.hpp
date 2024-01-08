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
	~PuzzleState() override;

   public:

	void draw() override;
	bool update(sf::Time dt) override;
	bool handleEvent(const sf::Event& event) override;

   private:
	void loadBasicGUI();
	void loadNormalGUI();
	void loadCorrectGUI();
	void loadFailGUI();
	void updateStatus();

	bool isPlayerTurn() const;

   private:
	GUI::Container mGUIContainer, mStatusContainer[3];
	Puzzle &mPuzzle;
	GameHandler mGame;
	HumanPlayer mPlayer;
	sf::Time mCoolDown;
	int currentMove, hintLevel;
	Puzzle::Status mStatus;
};

#endif  //CHESS_CHILL_PUZZLESTATE_HPP
