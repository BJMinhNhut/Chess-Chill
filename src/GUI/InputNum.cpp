//
// Created by MINH NHUT on 8/3/2023.
//

#include "InputNum.hpp"
#include "Template/Random.hpp"
#include "Template/Utility.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>

namespace GUI {
const sf::Time InputNum::CURSOR_LIFE = sf::milliseconds(400);
const int InputNum::MAX_LENGTH = 1000;
const int InputNum::DISPLAY_LENGTH = 9;

InputNum::InputNum(const FontHolder& fonts, const TextureHolder& textures,
                   const ColorHolder& colors)
    : mNormalTexture(textures.get(Textures::InputNormal)),
      mSelectedTexture(textures.get(Textures::InputSelected)),
      mPressedTexture(textures.get(Textures::InputActivated)),
      mText("", fonts.get(Fonts::Mono), 18),
      cursorDrawable(true),
      mCursor(sf::Vector2f(2.f, 16.f)),
      cursorCountdown(CURSOR_LIFE),
      mSprite(mNormalTexture),
      mMaxValue(0),
      mMinValue(0),
      mValue(0) {

	mText.setFillColor(colors.get(Colors::Text));
	alignText();

	Utility::centerOrigin(mSprite);

	mCursor.setFillColor(colors.get(Colors::Text));
	mCursor.setOrigin(-2.f, 0.f);
}

void InputNum::setValue(const int& value) {
	assert(value >= mMinValue && value <= mMaxValue);
	mValue = value;
	buffer = std::to_string(mValue);
	mText.setString(buffer);
	alignText();
}

void InputNum::setRange(const int& minValue, const int& maxValue) {
	assert(minValue <= maxValue);
	mMinValue = minValue;
	mMaxValue = maxValue;
}

void InputNum::randomizeValue() {
	setValue(Random::getInt(mMinValue, mMaxValue));
}

void InputNum::clear() {
	std::string().swap(buffer);
	mText.setString(buffer);
	mValue = 0;
}

std::pair<int, int> InputNum::getRange() const {
	return std::make_pair(mMinValue, mMaxValue);
}

std::string InputNum::getStringRange() const {
	return '[' + std::to_string(mMinValue) + ',' + std::to_string(mMaxValue) + ']';
}

int InputNum::getValue() const {
	return mValue;
}

bool InputNum::isSelectable() const {
	return true;
}

void InputNum::select() {
	Component::select();
	mSprite.setTexture(mSelectedTexture, true);
}

void InputNum::deselect() {
	Component::deselect();
	mSprite.setTexture(mNormalTexture, true);
}

void InputNum::activate() {
	Component::activate();
#ifdef SFML_DEBUG
	std::cout << "InputNum activated\n";
#endif
	mSprite.setTexture(mPressedTexture, true);
}

void InputNum::deactivate() {
#ifdef SFML_DEBUG
	std::cout << "InputNum deactivated\n";
#endif
	Component::deactivate();

	// Reset texture to right one depending on if we are selected or not.
	if (isSelected())
		mSprite.setTexture(mSelectedTexture, true);
	else
		mSprite.setTexture(mNormalTexture, true);
}

void InputNum::update(sf::Time dt) {
	if (isActive()) {
		cursorCountdown += dt;

		if (cursorCountdown > CURSOR_LIFE) {
			cursorDrawable ^= 1;
			cursorCountdown = sf::milliseconds(0.f);
		}
	} else
		cursorDrawable = false;
}

void InputNum::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed || event.type == sf::Event::TextEntered) {

		if (event.type == sf::Event::TextEntered) {
			// handle text input
#ifdef SFML_DEBUG
			std::cout << "text hit!\n";
#endif
			char mChar = static_cast<char>(event.text.unicode);
			if (isAllowed(mChar) && buffer.length() < MAX_LENGTH) {
				buffer.push_back(mChar);
			}
		} else if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::BackSpace) {
				// pop back buffer
				if (!buffer.empty())
					buffer.pop_back();
			}
		}

		if (validate() == Success) {
			mValue = stoi(buffer);
			buffer = std::to_string(mValue);
			mText.setString(buffer);
		} else
			mText.setString(buffer.substr(std::max(0, (int)buffer.length() - DISPLAY_LENGTH),
			                              std::min((int)buffer.length(), DISPLAY_LENGTH)));

		alignText();
	}
}

bool InputNum::contains(sf::Vector2i point) const {
	sf::IntRect bounds(getPosition().x - mSprite.getGlobalBounds().width / 2.f,
	                   getPosition().y - mSprite.getGlobalBounds().height / 2.f,
	                   mSprite.getGlobalBounds().width, mSprite.getGlobalBounds().height);

	return bounds.contains(point);
}

void InputNum::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mText, states);

	if (cursorDrawable)
		target.draw(mCursor, states);
}

InputNum::ValidationResult InputNum::validate() const {
	if (!validateCharacter())
		return InvalidCharacter;
	try {
		int temp = stoi(buffer);
		if (temp < mMinValue || temp > mMaxValue)
			return InvalidValue;
	} catch (std::exception& err) {
		return InvalidLength;
	}
	return Success;
}

bool InputNum::validateCharacter() const {
	for (char mChar : buffer)
		if (!isAllowed(mChar)) {
			std::cerr << int(mChar) << " is not allowed\n";
			return false;
		}
	return true;
}

bool InputNum::isAllowed(const char& ch) {
	return std::isdigit(ch);  // positive integer only
}

void InputNum::alignText() {
	Utility::centerOrigin(mText);
	unsigned int lastIndex = mText.getString().getSize();

	if (mText.getString().isEmpty())
		mCursor.setPosition(0.f, -9.f);
	else
		mCursor.setPosition(mText.findCharacterPos((int)lastIndex - 1) + sf::Vector2f(10.f, 4.f));
}

}  // namespace GUI