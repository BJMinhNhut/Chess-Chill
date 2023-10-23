//
// Created by MINH NHUT on 7/3/2023.
//

#ifndef DATAVISUALIZATION2_COLORHOLDER_HPP
#define DATAVISUALIZATION2_COLORHOLDER_HPP

#include <SFML/Graphics/Color.hpp>

#include <cassert>
#include <map>
#include <memory>
#include <string>

namespace Colors {
enum ID {
	Text,
	UIPrimary,
	UISecondary,
	UIBorder,
	DimBorder,
	Blue,
	Red,
	Highlight,
	DimHighlight,
};
}

class ColorHolder {
   public:
	void load(Colors::ID id, const sf::Color& color);

	[[nodiscard]] const sf::Color& get(Colors::ID id) const;

   private:
	void insertColor(Colors::ID id, std::unique_ptr<sf::Color> color);

   private:
	std::map<Colors::ID, std::unique_ptr<sf::Color>> mColorMap;
};

#endif  //DATAVISUALIZATION2_COLORHOLDER_HPP
