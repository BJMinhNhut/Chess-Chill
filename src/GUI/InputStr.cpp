//
// Created by MINH NHUT on 8/19/2023.
//

#include "InputStr.hpp"
#include "Template/Random.hpp"
#include "Template/Utility.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>

namespace GUI {
const sf::Time InputStr::CURSOR_LIFE = sf::milliseconds(400);
const int InputStr::MAX_LENGTH = 1000;
const int InputStr::DISPLAY_LENGTH = 9;

InputStr::InputStr(const FontHolder& fonts, const TextureHolder& textures,
                   const ColorHolder& colors)
    : mNormalTexture(textures.get(Textures::InputNormal)),
      mSelectedTexture(textures.get(Textures::InputSelected)),
      mPressedTexture(textures.get(Textures::InputActivated)),
      mText("", fonts.get(Fonts::Mono), 18),
      cursorDrawable(true),
      mCursor(sf::Vector2f(2.f, 16.f)),
      cursorCountdown(CURSOR_LIFE),
      mSprite(mNormalTexture),
      mString(),
      mLengthLimit(0) {

	mText.setFillColor(colors.get(Colors::Text));
	alignText();

	Utility::centerOrigin(mSprite);

	mCursor.setFillColor(colors.get(Colors::Text));
	mCursor.setOrigin(-2.f, 0.f);
}

void InputStr::setString(const std::string& str) {
	mString = buffer = str;
	mText.setString(str);
	assert(validate() == Success);
	alignText();
}

void InputStr::setLengthLimit(const int& limit) {
	mLengthLimit = limit;
}

void InputStr::randomize() {
	int len = Random::getInt(1, mLengthLimit);
	std::string str;
	while (len--)
		str.push_back((char)Random::getInt('A', 'Z'));
	setString(str);
}

void InputStr::clear() {
	std::string().swap(buffer);
	mText.setString(buffer);
	mString = "";
}

int InputStr::getLengthLimit() const {
	return mLengthLimit;
}

std::string InputStr::getString() const {
	return mString;
}

bool InputStr::isSelectable() const {
	return true;
}

void InputStr::select() {
	Component::select();
	mSprite.setTexture(mSelectedTexture, true);
}

void InputStr::deselect() {
	Component::deselect();
	mSprite.setTexture(mNormalTexture, true);
}

void InputStr::activate() {
	Component::activate();
#ifdef SFML_DEBUG
	std::cout << "InputStr activated\n";
#endif
	mSprite.setTexture(mPressedTexture, true);
}

void InputStr::deactivate() {
#ifdef SFML_DEBUG
	std::cout << "InputStr deactivated\n";
#endif
	Component::deactivate();

	// Reset texture to right one depending on if we are selected or not.
	if (isSelected())
		mSprite.setTexture(mSelectedTexture, true);
	else
		mSprite.setTexture(mNormalTexture, true);
}

void InputStr::update(sf::Time dt) {
	if (isActive()) {
		cursorCountdown += dt;

		if (cursorCountdown > CURSOR_LIFE) {
			cursorDrawable ^= 1;
			cursorCountdown = sf::milliseconds(0.f);
		}
	} else
		cursorDrawable = false;
}

void InputStr::handleEvent(const sf::Event& event) {
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
			mString = buffer;
			mText.setString(buffer);
		} else
			mText.setString(buffer.substr(std::max(0, (int)buffer.length() - DISPLAY_LENGTH),
			                              std::min((int)buffer.length(), DISPLAY_LENGTH)));

		alignText();
	}
}

bool InputStr::contains(sf::Vector2i point) const {
	sf::IntRect bounds(getPosition().x - mSprite.getGlobalBounds().width / 2.f,
	                   getPosition().y - mSprite.getGlobalBounds().height / 2.f,
	                   mSprite.getGlobalBounds().width, mSprite.getGlobalBounds().height);

	return bounds.contains(point);
}

void InputStr::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mText, states);

	if (cursorDrawable)
		target.draw(mCursor, states);
}

InputStr::ValidationResult InputStr::validate() const {
	if (buffer.length() > mLengthLimit)
		return InvalidLength;
	if (!validateCharacter())
		return InvalidCharacter;
	return Success;
}

bool InputStr::validateCharacter() const {
	for (char mChar : buffer)
		if (!isAllowed(mChar)) {
			std::cerr << int(mChar) << " is not allowed\n";
			return false;
		}
	return true;
}

bool InputStr::isAllowed(const char& ch) {
	return isupper(ch);  // positive integer only
}

void InputStr::alignText() {
	Utility::centerOrigin(mText);
	unsigned int lastIndex = mText.getString().getSize();

	if (mText.getString().isEmpty())
		mCursor.setPosition(0.f, -9.f);
	else
		mCursor.setPosition(mText.findCharacterPos((int)lastIndex - 1) + sf::Vector2f(10.f, 4.f));
}

}  // namespace GUI