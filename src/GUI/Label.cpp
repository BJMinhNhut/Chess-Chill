//
// Created by MINH NHUT on 7/4/2023.
//

#include "Label.hpp"

#include "Template/Constants.hpp"
#include "Template/Utility.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cmath>

namespace GUI {

Label::Label(Type type, const std::string& text, const FontHolder& fonts, const sf::Color& color)
    : mText(text, fonts.get(getFontID(type)), getFontSize(type)) {
	mText.setFillColor(color);
	mText.setOrigin(0.f, std::floor(mText.getGlobalBounds().height / 2.f));
}

bool Label::isSelectable() const {
	return false;
}

bool Label::isEmpty() const {
	return mText.getString().isEmpty();
}

void Label::setText(const std::string& text) {
	mText.setString(text);
}

void Label::setColor(const sf::Color& color) {
	mText.setFillColor(color);
}

void Label::alignCenter() {
	Utility::centerOrigin(mText);
}

void Label::alignRight() {
	sf::FloatRect bounds = mText.getLocalBounds();
	mText.setOrigin(bounds.width, std::floor(bounds.height / 2.f));
}

Fonts::ID Label::getFontID(Type type) {
	switch (type) {
		case Bold:
		case Title:
			return Fonts::Bold;
		case Clock:
		case Mono:
			return Fonts::Mono;
		default:
			return Fonts::Main;
	}
}

unsigned int Label::getFontSize(Type type) {
	switch (type) {
		case Main:
		case Bold:
			return 24u;
		case Small:
			return 20u;
		case Clock:
			return 48u;
		case Title:
			return 64u;
		default:
			return 18u;
	}
}

void Label::handleEvent(const sf::Event& event) {}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mText, states);
}

}  // namespace GUI