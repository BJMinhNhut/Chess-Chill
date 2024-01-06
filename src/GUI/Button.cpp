//
// Created by MINH NHUT on 7/2/2023.
//

#include "Button.hpp"
#include "Template/Constants.hpp"
#include "Template/Utility.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>

namespace GUI {

Button::Button(Type type, const FontHolder& fonts, const TextureHolder& textures)
    : mCallback(),
      mIsToggle(false),
      mNormalTexture(textures.get(getNormalTextureID(type))),
      mSelectedTexture(textures.get(getSelectedTextureID(type))),
      mPressedTexture(textures.get(getPressedTextureID(type))),
      mSprite(mNormalTexture),
      mText() {
	setFont(type, fonts);
	Utility::centerOrigin(mSprite);
}

void Button::alignTopLeft() {
	mSprite.setOrigin(0.f, 0.f);
}

Textures::ID Button::getNormalTextureID(Type type) {
	switch (type) {
		case Menu:
			return Textures::MenuButtonNormal;
		case Home:
			return Textures::HomeButtonNormal;
		case Back:
			return Textures::BackButtonNormal;
		case Forward:
			return Textures::ForwardButtonNormal;
		case Rotate:
			return Textures::RotateButtonNormal;
		case Play:
			return Textures::PlayButtonNormal;
		case Close:
			return Textures::CloseButtonNormal;
		case Last:
			return Textures::LastButtonNormal;
		case First:
			return Textures::FirstButtonNormal;
		case Previous:
			return Textures::PreviousButtonNormal;
		case Next:
			return Textures::NextButtonNormal;
		case History:
			return Textures::HistoryButtonNormal;
		case Puzzle:
			return Textures::PuzzleNormal;
		case PuzzleSolved:
			return Textures::PuzzleSolvedNormal;
		case PuzzleFailed:
			return Textures::PuzzleFailedNormal;
		default:
			return Textures::Background;
	}
}

Textures::ID Button::getSelectedTextureID(Type type) {
	switch (type) {
		case Menu:
			return Textures::MenuButtonSelected;
		case Home:
			return Textures::HomeButtonSelected;
		case Back:
			return Textures::BackButtonSelected;
		case Forward:
			return Textures::ForwardButtonSelected;
		case Rotate:
			return Textures::RotateButtonSelected;
		case Play:
			return Textures::PlayButtonSelected;
		case Close:
			return Textures::CloseButtonSelected;
		case Last:
			return Textures::LastButtonSelected;
		case First:
			return Textures::FirstButtonSelected;
		case Previous:
			return Textures::PreviousButtonSelected;
		case Next:
			return Textures::NextButtonSelected;
		case History:
			return Textures::HistoryButtonSelected;
		case Puzzle:
			return Textures::PuzzleSelected;
		case PuzzleSolved:
			return Textures::PuzzleSolvedSelected;
		case PuzzleFailed:
			return Textures::PuzzleFailedSelected;
		default:
			return Textures::Background;
	}
}

Textures::ID Button::getPressedTextureID(Type type) {
	switch (type) {
		default:
			return getSelectedTextureID(type);
	}
}

void Button::setFont(Type type, const FontHolder& fonts) {
	switch (type) {
		case Puzzle:
		case PuzzleSolved:
		case PuzzleFailed:
			mText.setFont(fonts.get(Fonts::Bold));
			mText.setCharacterSize(32u);
			mText.setFillColor(sf::Color::White);
			break;
		case Menu:
			mText.setPosition(0.f, -3.f);
		default:
			mText.setFont(fonts.get(Fonts::Main));
			mText.setCharacterSize(24u);
			mText.setFillColor(sf::Color::White);
			break;
	}
}

void Button::setCallback(Callback callback) {
	mCallback = std::move(callback);
}

void Button::setText(const std::string& text) {
	mText.setString(text);
	Utility::centerOrigin(mText);
}

void Button::setToggle(bool flag) {
	mIsToggle = flag;
}

bool Button::isSelectable() const {
	return true;
}

void Button::select() {
	Component::select();
	mSprite.setTexture(mSelectedTexture, true);
}

void Button::deselect() {
	Component::deselect();
	mSprite.setTexture(mNormalTexture, true);
}

void Button::activate() {
	Component::activate();

	// If we are toggle then we should show that the button is pressed and thus "toggled".
	if (mIsToggle)
		mSprite.setTexture(mPressedTexture, true);

	if (mCallback)
		mCallback();

	// If we are not a toggle then deactivate the button since we are just momentarily activated.
	if (!mIsToggle)
		deactivate();
}

void Button::deactivate() {
	Component::deactivate();

	if (mIsToggle) {
		// Reset texture to right one depending on if we are selected or not.
		if (isSelected())
			mSprite.setTexture(mSelectedTexture, true);
		else
			mSprite.setTexture(mNormalTexture, true);
	}
}

void Button::handleEvent(const sf::Event& event) {}

bool Button::contains(sf::Vector2i point) const {
	sf::Vector2f origin = mSprite.getOrigin();
	sf::IntRect bounds(getPosition().x - origin.x, getPosition().y - origin.y,
	                   mSprite.getGlobalBounds().width, mSprite.getGlobalBounds().height);

	return bounds.contains(point);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mText, states);
}

}  // namespace GUI