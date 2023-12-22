//
// Created by MINH NHUT on 10/23/2023.
//

#ifndef CHESS_GAMESTATE_HPP
#define CHESS_GAMESTATE_HPP

#include "GUI/Container.hpp"
#include "GUI/Label.hpp"
#include "GUI/Sprite.hpp"
#include "Game/GameHandler.hpp"
#include "Game/Player.hpp"
#include "Template/State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class GameState : public State {
   public:
	const static sf::Vector2f BOARD_POSITION;

   public:
	GameState(StateStack& stack, Context context);

	void draw() override;

	bool update(sf::Time dt) override;

	bool handleEvent(const sf::Event& event) override;

   private:
	void loadBasicGUI();
	void loadGameGUI();
	void loadControllerGUI();
	void loadEndGameGUI();
	void loadResult();

	void rotateBoard();

	void updateClock();
	static std::string getClockString(float time);
	std::string getTitle() const;

   private:
	Player::Ptr mPlayers[2];
	GameHandler mGame;
	GUI::Container mGUIContainer;
	GUI::Container mEndGameContainer;
	GUI::Label::Ptr mWinner, mDescription;
	GUI::Label::Ptr mClock[2], mPlayerLabel[2], mEvaluation;
	sf::Time mCoolDown;
	bool mReviewMode;
};

#endif  //CHESS_GAMESTATE_HPP
