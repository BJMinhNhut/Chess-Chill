//
// Created by MINH NHUT on 11/16/2023.
//

#ifndef CHESS_CHILL_SETTINGSSTATE_HPP
#define CHESS_CHILL_SETTINGSSTATE_HPP

#include "GUI/Container.hpp"
#include "GUI/Label.hpp"
#include "GUI/Sprite.hpp"
#include "Template/State.hpp"

#include <SFML/Graphics/Sprite.hpp>

class SettingsState : public State {
   public:
	SettingsState(StateStack& stack, Context context);

	void draw() override;

	bool update(sf::Time dt) override;

	bool handleEvent(const sf::Event& event) override;

   private:
	void loadBasicGUI();
	void loadPieceSetGUI();
	void loadBoardGUI();
	void loadSoundGUI();

	void updatePieceSet();
	void updateBoard();

   private:
	GUI::Container mGUIContainer;

	GUI::Sprite::Ptr mPieceSet, mBoard;
	GUI::Label::Ptr mSound;
};

#endif  //CHESS_CHILL_SETTINGSSTATE_HPP
