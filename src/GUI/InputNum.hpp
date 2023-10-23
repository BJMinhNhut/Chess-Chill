//
// Created by MINH NHUT on 8/3/2023.
//

#ifndef DATAVISUALIZATION2_INPUTNUM_HPP
#define DATAVISUALIZATION2_INPUTNUM_HPP

#include "GUI/Component.hpp"
#include "Template/ColorHolder.hpp"
#include "Template/ResourceHolder.hpp"
#include "Template/ResourceIdentifiers.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace GUI {
class InputNum : public Component {
   public:
	static const sf::Time CURSOR_LIFE;
	static const int MAX_LENGTH;
	static const int DISPLAY_LENGTH;
	typedef std::shared_ptr<InputNum> Ptr;
	enum ValidationResult { Success, InvalidCharacter, InvalidValue, InvalidLength };

   public:
	InputNum(const FontHolder& fonts, const TextureHolder& textures, const ColorHolder& colors);

	void setValue(const int& value);
	void setRange(const int& minValue, const int& maxValue);
	void randomizeValue();
	void clear();

	std::pair<int, int> getRange() const;
	std::string getStringRange() const;
	int getValue() const;

	bool isSelectable() const override;
	void select() override;
	void deselect() override;

	void activate() override;
	void deactivate() override;

	void update(sf::Time dt) override;
	void handleEvent(const sf::Event& event) override;
	bool contains(sf::Vector2i point) const override;

	ValidationResult validate() const;

   private:
	static bool isAllowed(const char& ch);
	bool validateCharacter() const;
	void alignText();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

   private:
	int mMinValue, mMaxValue, mValue;
	std::string buffer;

	sf::RectangleShape mCursor;
	bool cursorDrawable;
	sf::Time cursorCountdown;

	const sf::Texture mNormalTexture;
	const sf::Texture mSelectedTexture;
	const sf::Texture mPressedTexture;

	sf::Sprite mSprite;
	sf::Text mText;
};
}  // namespace GUI

#endif  //DATAVISUALIZATION2_INPUTNUM_HPP
