//
// Created by MINH NHUT on 11/16/2023.
//

#ifndef CHILL_ABOUTSTATE_HPP
#define CHILL_ABOUTSTATE_HPP

#include "GUI/Container.hpp"
#include "Template/State.hpp"

#include <SFML/Graphics/Sprite.hpp>

class AboutState : public State {
   public:
	explicit AboutState(StateStack& stack, Context context);

	virtual void draw();

	virtual bool update(sf::Time dt);

	virtual bool handleEvent(const sf::Event& event);

   private:
	GUI::Container mGUIContainer;
	sf::Sprite mBackgroundSprite;
};

#endif  //CHILL_ABOUTSTATE_HPP
