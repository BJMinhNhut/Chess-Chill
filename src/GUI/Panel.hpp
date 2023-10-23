//
// Created by MINH NHUT on 7/4/2023.
//

#ifndef DATAVISUALIZATION2_PANEL_HPP
#define DATAVISUALIZATION2_PANEL_HPP

#include "GUI/Component.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace GUI {
class Panel : public Component {
   public:
	Panel(const float& width, const float& height, const sf::Color& fillColor,
	      const sf::Color& outlineColor);

	bool isSelectable() const override;

	void handleEvent(const sf::Event& event) override;

	sf::Vector2f getSize() const;

   private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

   private:
	sf::RectangleShape mRect;
};
}  // namespace GUI

#endif  //DATAVISUALIZATION2_PANEL_HPP
