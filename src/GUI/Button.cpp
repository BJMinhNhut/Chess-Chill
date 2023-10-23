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

Button::Button(Type type, const FontHolder& fonts, const TextureHolder& textures,
               const ColorHolder& colors)
    : mCallback(),
      mIsToggle(false),
      mNormalTexture(textures.get(getNormalTextureID(type))),
      mSelectedTexture(textures.get(getSelectedTextureID(type))),
      mPressedTexture(textures.get(getPressedTextureID(type))),
      mSprite(mNormalTexture),
      mText() {
	setFont(type, fonts, colors);
	Utility::centerOrigin(mSprite);
}

Textures::ID Button::getNormalTextureID(Type type) {
	switch (type) {
		case Small:
			return Textures::SmallButtonNormal;
		case Big:
			return Textures::BigButtonNormal;
		case Command:
			return Textures::CommandNormal;
		case Home:
			return Textures::HomeNormal;
		case Checkbox:
			return Textures::CheckBoxNormal;
		case Back:
			return Textures::BackNormal;
		case Play:
			return Textures::PlayNormal;
		case Pause:
			return Textures::PauseNormal;
		case Replay:
			return Textures::ReplayNormal;
		case SmallArrow:
			return Textures::ArrowDownNormal;
		case Arrow:
			return Textures::ArrowNormal;
		case DoubleArrow:
			return Textures::DoubleArrowNormal;
		case MenuHash:
			return Textures::MenuHashNormal;
		case MenuAVL:
			return Textures::MenuAVLNormal;
		case Menu234:
			return Textures::Menu234Normal;
		case MenuMinHeap:
			return Textures::MenuMinHeapNormal;
		case MenuMaxHeap:
			return Textures::MenuMaxHeapNormal;
		case MenuTrie:
			return Textures::MenuTrieNormal;
		case MenuGraph:
			return Textures::MenuGraphNormal;
		default:
			return Textures::SmallButtonNormal;
	}
}

Textures::ID Button::getSelectedTextureID(Type type) {
	switch (type) {
		case Small:
			return Textures::SmallButtonSelected;
		case Big:
			return Textures::BigButtonSelected;
		case Command:
			return Textures::CommandSelected;
		case Home:
			return Textures::HomeSelected;
		case Checkbox:
			return Textures::CheckBoxSelected;
		case Back:
			return Textures::BackSelected;
		case Play:
			return Textures::PlaySelected;
		case Pause:
			return Textures::PauseSelected;
		case Replay:
			return Textures::ReplaySelected;
		case SmallArrow:
			return Textures::ArrowDownSelected;
		case Arrow:
			return Textures::ArrowSelected;
		case DoubleArrow:
			return Textures::DoubleArrowSelected;
		case MenuHash:
			return Textures::MenuHashSelected;
		case MenuAVL:
			return Textures::MenuAVLSelected;
		case Menu234:
			return Textures::Menu234Selected;
		case MenuMinHeap:
			return Textures::MenuMinHeapSelected;
		case MenuMaxHeap:
			return Textures::MenuMaxHeapSelected;
		case MenuTrie:
			return Textures::MenuTrieSelected;
		case MenuGraph:
			return Textures::MenuGraphSelected;
		default:
			return Textures::SmallButtonSelected;
	}
}

Textures::ID Button::getPressedTextureID(Type type) {
	switch (type) {
		case Command:
			return Textures::CommandActivated;
		case Checkbox:
			return Textures::CheckBoxActivated;
		default:
			return getSelectedTextureID(type);
	}
}

void Button::setFont(Type type, const FontHolder& fonts, const ColorHolder& colors) {
	switch (type) {
		case Small:
			mText.setFont(fonts.get(Fonts::Main));
			mText.setCharacterSize(16u);
			mText.setFillColor(colors.get(Colors::Text));
			break;

		case Big:
			mText.setFont(fonts.get(Fonts::Bold));
			mText.setCharacterSize(18u);
			mText.setFillColor(sf::Color::White);
			break;

		default:
			mText.setFont(fonts.get(Fonts::Main));
			mText.setCharacterSize(16u);
			mText.setFillColor(colors.get(Colors::Text));
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
	sf::IntRect bounds(getPosition().x - mSprite.getGlobalBounds().width / 2.f,
	                   getPosition().y - mSprite.getGlobalBounds().height / 2.f,
	                   mSprite.getGlobalBounds().width, mSprite.getGlobalBounds().height);

	return bounds.contains(point);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mText, states);
}

}  // namespace GUI