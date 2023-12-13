//
// Created by MINH NHUT on 10/24/2023.
//

#ifndef CHESS_RECTNODE_HPP
#define CHESS_RECTNODE_HPP

#include "SceneNode.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

class RectNode : public SceneNode {
   public:
	explicit RectNode(int width, int height, const sf::Color &color);

	// rate from 0 to 100
	void setOpacity(int rate);

   private:
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

   private:
	sf::RectangleShape mRect;
};

#endif  //CHESS_RECTNODE_HPP
