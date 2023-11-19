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
	void loadOptionsGUI();
	void loadDescriptionGUI();

	void updateType();
	void updateMode();

   private:
	GUI::Container mGUIContainer;
};

#endif  //CHESS_CHILL_GAMEOPTIONSSTATE_HPP
