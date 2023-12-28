//
// Created by MINH NHUT on 12/28/2023.
//

#ifndef CHESS_CHILL_REVIEWSTATE_HPP
#define CHESS_CHILL_REVIEWSTATE_HPP

#include "GUI/Container.hpp"
#include "GUI/Label.hpp"
#include "GUI/Sprite.hpp"
#include "Game/GameHandler.hpp"
#include "Template/State.hpp"

class ReviewState : public State {
   public:
	const static sf::Vector2f BOARD_POSITION;

   public:
	ReviewState(StateStack& stack, Context context);

	void draw() override;

	bool update(sf::Time dt) override;

	bool handleEvent(const sf::Event& event) override;

   private:
	void loadBasicGUI();
	void loadGameGUI();
	void loadControllerGUI();

	void rotateBoard();

	void updateMoveList();
	static std::string getClockString(float time);
	std::string getTitle() const;

   private:
	GameHandler mGame;

	GUI::Container mGUIContainer, mReviewContainer;
	GUI::Label::Ptr mWinner, mDescription;
	GUI::Label::Ptr mClock[2], mPlayerLabel[2];
	GUI::Label::Ptr mMoveList;
};

#endif  //CHESS_CHILL_REVIEWSTATE_HPP
