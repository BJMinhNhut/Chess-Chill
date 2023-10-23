//
// Created by MINH NHUT on 10/23/2023.
//

#ifndef CHESS_SPRITENODE_HPP
#define CHESS_SPRITENODE_HPP

#include "SceneNode.hpp"

#include <SFML/Graphics/Sprite.hpp>

class SpriteNode : public SceneNode {
   public:
	explicit SpriteNode(const sf::Texture& texture);
	explicit SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect);

	void centerOrigin();

	bool contains(int x, int y) const;

   private:
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

   private:
	sf::Sprite mSprite;
};

#endif  //CHESS_SPRITENODE_HPP
