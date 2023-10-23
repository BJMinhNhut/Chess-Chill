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

    Label::Label(Type type, const std::string &text,
                 const FontHolder &fonts, const ColorHolder &colors)
    : mText(text, fonts.get(getFontID(type)), getFontSize(type)) {
	mText.setFillColor(colors.get(Colors::Text));
	    mText.setOrigin(0.f, std::floor(mText.getGlobalBounds().height / 2.f));
    }

    bool Label::isSelectable() const {
        return false;
    }

    void Label::setText(const std::string &text) {
        mText.setString(text);
    }

    void Label::setColor(const sf::Color &color) {
        mText.setFillColor(color);
    }

    void Label::alignCenter() {
        Utility::centerOrigin(mText);
    }

    Fonts::ID Label::getFontID(Type type) {
        switch (type) {
            case Main:
                return Fonts::Main;
            case Bold:
                return Fonts::Bold;
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
			    return 19u;
		    default:
			    return 16u;
	    }
    }

    void Label::handleEvent(const sf::Event &event) {}

    void Label::draw(sf::RenderTarget &target,
                     sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(mText, states);
    }

}  // namespace GUI