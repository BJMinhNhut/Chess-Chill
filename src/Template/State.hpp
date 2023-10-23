//
// Created by MINH NHUT on 6/11/2023.
//

#ifndef NATUREOFCODE_STATE_HPP
#define NATUREOFCODE_STATE_HPP

#include "ResourceIdentifiers.hpp"
#include "ColorHolder.hpp"
#include "StateIdentifiers.hpp"

#include "SFML/System/Time.hpp"
#include "SFML/Window/Event.hpp"

namespace sf {
    class RenderWindow;
}

class StateStack;

class State {
public:
    typedef std::unique_ptr<State> Ptr;

    struct Context {
        Context(sf::RenderWindow &window, TextureHolder &textures, FontHolder &fonts, ColorHolder &colors);

        sf::RenderWindow *window;
        TextureHolder *textures;
        FontHolder *fonts;
        ColorHolder *colors;
    };

public:
    State(StateStack &stack, Context context);

    virtual ~State();

    virtual void draw() = 0;

    virtual bool update(sf::Time dt) = 0;

    virtual bool handleEvent(const sf::Event &event) = 0;

protected:
    void requestStackPush(States::ID stateID);

    void requestStackPop();

    void requestStateClear();

    [[nodiscard]] Context getContext() const;

private:
    StateStack *mStack;
    Context mContext;
};

#endif  //NATUREOFCODE_STATE_HPP
