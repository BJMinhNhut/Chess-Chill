//
// Created by MINH NHUT on 10/23/2023.
//

#ifndef CHESS_GAMESTATE_HPP
#define CHESS_GAMESTATE_HPP

#include "Template/State.hpp"
#include "Game/GameHandler.hpp"
#include "GUI/Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class GameState : public State {
public:
    GameState(StateStack &stack, Context context);

    virtual void draw();

    virtual bool update(sf::Time dt);

    virtual bool handleEvent(const sf::Event &event);

private:
	GameHandler mGame;
//    sf::Sprite mBackgroundSprite;
//    sf::Sprite mTitle;
//    GUI::Container mGUIContainer;
};


#endif //CHESS_GAMESTATE_HPP
