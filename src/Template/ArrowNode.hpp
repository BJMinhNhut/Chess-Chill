//
// Created by MINH NHUT on 1/8/2024.
//

#ifndef CHESS_CHILL_ARROWNODE_HPP
#define CHESS_CHILL_ARROWNODE_HPP

#include "SceneNode.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

class ArrowNode : public SceneNode {
   public:
	typedef std::unique_ptr<ArrowNode> Ptr;
	static const float HEAD_RADIUS;
	static const float THICKNESS;

   public:
	explicit ArrowNode(sf::Vector2f line);

	// rate from 0 to 100
	void setOpacity(int rate);
	void setFillColor(sf::Color color);

   private:
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

	static sf::RectangleShape getLineShape(sf::Vector2f line);
	static sf::CircleShape getHeadShape(sf::Vector2f line);

   private:
	sf::RectangleShape mLine;
	sf::CircleShape mHead;
};

#endif  //CHESS_CHILL_ARROWNODE_HPP
