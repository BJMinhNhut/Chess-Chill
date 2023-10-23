//
// Created by MINH NHUT on 8/19/2023.
//

#ifndef DATAVISUALIZATION2_INPUTSTR_HPP
#define DATAVISUALIZATION2_INPUTSTR_HPP

#include "GUI/Component.hpp"
#include "Template/ColorHolder.hpp"
#include "Template/ResourceHolder.hpp"
#include "Template/ResourceIdentifiers.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace GUI {
class InputStr : public Component {
   public:
	static const sf::Time CURSOR_LIFE;
	static const int MAX_LENGTH;
	static const int DISPLAY_LENGTH;
	typedef std::shared_ptr<InputStr> Ptr;
	enum ValidationResult { Success, InvalidCharacter, InvalidLength };

   public:
	InputStr(const FontHolder& fonts, const TextureHolder& textures, const ColorHolder& colors);

	void setString(const std::string& str);
	void setLengthLimit(const int& limit);
	void randomize();
	void clear();

	int getLengthLimit() const;
	std::string getString() const;

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
	int mLengthLimit;
	std::string mString;
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

#endif  //DATAVISUALIZATION2_INPUTSTR_HPP
