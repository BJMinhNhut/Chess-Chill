//
// Created by MINH NHUT on 6/27/2023.
//

#ifndef DATAVISUALIZATION2_MENUSTATE_HPP
#define DATAVISUALIZATION2_MENUSTATE_HPP

#include "Template/State.hpp"
#include "GUI/Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class MenuState : public State {
public:
    MenuState(StateStack &stack, Context context);

    void draw() override;

    bool update(sf::Time dt) override;

    bool handleEvent(const sf::Event &event) override;

private:
    sf::Sprite mBackgroundSprite;
    sf::Sprite mTitle;
    GUI::Container mGUIContainer;
};


#endif //DATAVISUALIZATION2_MENUSTATE_HPP
