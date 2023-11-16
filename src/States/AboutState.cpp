//
// Created by MINH NHUT on 11/16/2023.
//

#include "AboutState.hpp"
#include "GUI/Label.hpp"
#include "GUI/Sprite.hpp"
#include "GUI/Button.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

AboutState::AboutState(StateStack &stack, Context context)
    : State(stack, context),
      mGUIContainer(),
      mBackgroundSprite(context.textures->get(Textures::Background)) {

	auto backButton = std::make_shared<GUI::Button>(
	    GUI::Button::Back, *context.fonts, *context.textures);
	backButton->setPosition(509.f, 53.f);
	backButton->setCallback([this]() {
		requestStackPop();
	});
	mGUIContainer.pack(backButton);

	auto homeButton = std::make_shared<GUI::Button>(
	    GUI::Button::Home, *context.fonts, *context.textures);
	homeButton->setPosition(1063.f + 54.f/2, 53.f);
	homeButton->setCallback([this]() {
		requestStateClear();
		requestStackPush(States::Menu);
	});
	mGUIContainer.pack(homeButton);

	auto titleBar = std::make_shared<GUI::Sprite>(
	    context.textures->get(Textures::TitleBar));
	titleBar->setPosition(800.f, 53.f);
	mGUIContainer.pack(titleBar);

	auto titleLabel = std::make_shared<GUI::Label>(
	    GUI::Label::Bold, "About", *context.fonts);
	titleLabel->setPosition(titleBar->getPosition());
	titleLabel->alignCenter();
	mGUIContainer.pack(titleLabel);

	auto aboutPanel = std::make_shared<GUI::Sprite>(
	    context.textures->get(Textures::AboutPanel));
	aboutPanel->setPosition(482.f + 636.f/2, 236.f + 468.f/2);
	mGUIContainer.pack(aboutPanel);
}

void AboutState::draw() {
	sf::RenderWindow &window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool AboutState::update(sf::Time dt) {
	return true;
}

bool AboutState::handleEvent(const sf::Event &event) {
	mGUIContainer.handleEvent(event);
	return false;
}
