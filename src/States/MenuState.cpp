//
// Created by MINH NHUT on 6/27/2023.
//

#include "MenuState.hpp"
#include "GUI/Button.hpp"
#include "Template/ResourceHolder.hpp"
#include "Template/Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

MenuState::MenuState(StateStack &stack, Context context)
        : State(stack, context),
          mGUIContainer(),
          mBackgroundSprite(context.textures->get(Textures::Background)),
          mTitle(context.textures->get(Textures::Title)) {

    Utility::centerOrigin(mTitle);
    mTitle.setPosition(context.window->getSize().x / 2.f,
                       context.window->getSize().y / 4.f);

    sf::Font &font = context.fonts->get(Fonts::Main);
    sf::Vector2u bounds = context.window->getSize();

    auto startButton = std::make_shared<GUI::Button>(
            GUI::Button::Big, *context.fonts, *context.textures,
            *context.colors);
    startButton->setPosition(bounds.x / 2u, bounds.y / 2u);
    startButton->setText("Start");
    startButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::DataMenu);
    });

    auto settingsButton = std::make_shared<GUI::Button>(
            GUI::Button::Big, *context.fonts, *context.textures,
            *context.colors);
    settingsButton->setPosition(bounds.x / 2.f, bounds.y / 2.f + 70.f);
    settingsButton->setText("Settings");
    settingsButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::Settings);
    });

    auto aboutButton = std::make_shared<GUI::Button>(
            GUI::Button::Big, *context.fonts, *context.textures,
            *context.colors);
    aboutButton->setPosition(bounds.x / 2.f, bounds.y / 2.f + 140.f);
    aboutButton->setText("About");
    aboutButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::About);
    });

    auto exitButton = std::make_shared<GUI::Button>(
            GUI::Button::Big, *context.fonts, *context.textures,
            *context.colors);
    exitButton->setPosition(bounds.x / 2.f, bounds.y / 2.f + 210.f);
    exitButton->setText("Exit");
    exitButton->setCallback([this]() { requestStackPop(); });

    mGUIContainer.pack(startButton);
    mGUIContainer.pack(exitButton);
    mGUIContainer.pack(aboutButton);
    mGUIContainer.pack(settingsButton);
}

void MenuState::draw() {
    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());

    window.draw(mBackgroundSprite);
    window.draw(mTitle);
    window.draw(mGUIContainer);
}

bool MenuState::update(sf::Time dt) {
    return true;
}

bool MenuState::handleEvent(const sf::Event &event) {
    mGUIContainer.handleEvent(event);
    return false;
}