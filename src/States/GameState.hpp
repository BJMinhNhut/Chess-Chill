//
// Created by MINH NHUT on 10/23/2023.
//

#ifndef CHESS_GAMESTATE_HPP
#define CHESS_GAMESTATE_HPP

#include "Template/State.hpp"
#include "Game/GameHandler.hpp"
#include "GUI/Container.hpp"
#include "GUI/Label.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class GameState : public State {
public:
    GameState(StateStack &stack, Context context);

    void draw() override;

    bool update(sf::Time dt) override;

    bool handleEvent(const sf::Event &event) override;

   private:
	void loadBasicGUI();
	void loadEndGameGUI();
	void loadResult();

private:
	GameHandler mGame;
    GUI::Container mGUIContainer;
	GUI::Container mEndGameContainer;
	GUI::Label::Ptr mWinner, mDescription;
};


#endif //CHESS_GAMESTATE_HPP
