//
// Created by MINH NHUT on 1/2/2024.
//

#include "HistoryPanel.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI {
HistoryPanel::HistoryPanel(const FontHolder& fonts, const TextureHolder& textures)
    : Button(Type::History, fonts, textures) {
	mTime.setFont(fonts.get(Fonts::ID::Main));
	mTime.setCharacterSize(20);
	mTime.setFillColor(sf::Color::White);
	mTime.setPosition(314.f, 45.f);

	mDate.setFont(fonts.get(Fonts::ID::Bold));
	mDate.setCharacterSize(20);
	mDate.setFillColor(sf::Color::White);
	mDate.setPosition(30.f, 11.f);

	mMode.setFont(fonts.get(Fonts::ID::Main));
	mMode.setCharacterSize(20);
	mMode.setFillColor(sf::Color::White);
	mMode.setPosition(210.f, 45.f);

	mType.setFont(fonts.get(Fonts::ID::Main));
	mType.setCharacterSize(20);
	mType.setFillColor(sf::Color::White);
	mType.setPosition(30.f, 45.f);

	mResult.setFont(fonts.get(Fonts::ID::Main));
	mResult.setCharacterSize(20);
	mResult.setFillColor(sf::Color::White);
	mResult.setPosition(438.f, 45.f);

	alignTopLeft();
}

void HistoryPanel::setTime(const std::string& time) {
	mTime.setString(time);
}

void HistoryPanel::setMode(const std::string& mode) {
	mMode.setString(mode);
}

void HistoryPanel::setType(const std::string& type) {
	mType.setString(type);
}

void HistoryPanel::setResult(const std::string& result) {
	mResult.setString(result);
}

void HistoryPanel::setDate(const std::string& date) {
	mDate.setString(date);
}

void HistoryPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	Button::draw(target, states);

	states.transform *= getTransform();
	target.draw(mTime, states);
	target.draw(mDate, states);
	target.draw(mMode, states);
	target.draw(mType, states);
	target.draw(mResult, states);
}

}  // namespace GUI