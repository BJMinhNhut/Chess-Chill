//
// Created by MINH NHUT on 11/19/2023.
//

#ifndef CHESS_CHILL_GAMEOPTIONSSTATE_HPP
#define CHESS_CHILL_GAMEOPTIONSSTATE_HPP

#include "GUI/Container.hpp"
#include "GUI/Label.hpp"
#include "GUI/Sprite.hpp"
#include "Template/Settings.hpp"
#include "Template/State.hpp"

#include <SFML/Graphics/Sprite.hpp>

class GameOptionsState : public State {
   public:
	GameOptionsState(StateStack& stack, Context context);

	void draw() override;

	bool update(sf::Time dt) override;

	bool handleEvent(const sf::Event& event) override;

   private:
	void loadBasicGUI();
	void loadGameModeGUI();
	void loadTypeGUI();
	void loadTimeGUI();
	void loadDescriptionGUI();
	void loadSideGUI();

	void updateType();
	void updateMode();
	void updateTime();
	void updateSide();

   private:
	GUI::Container mGUIContainer, mSideContainer;
	GUI::Label::Ptr mType, mMode, mTime, mSide;
	GUI::Sprite::Ptr mDescription;
};

#endif  //CHESS_CHILL_GAMEOPTIONSSTATE_HPP
