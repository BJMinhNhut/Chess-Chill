//
// Created by MINH NHUT on 10/23/2023.
//

#ifndef CHESS_GAMEHANDLER_HPP
#define CHESS_GAMEHANDLER_HPP

#include "Template/ResourceHolder.hpp"
#include "Template/ResourceIdentifiers.hpp"
#include "Template/SceneNode.hpp"
#include "Template/SpriteNode.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <array>

namespace sf {
class RenderWindow;
}

class GameHandler : public sf::NonCopyable {
   public:
	explicit GameHandler(sf::RenderWindow& window, FontHolder& fonts);
	void update(sf::Time dt);
	void draw();
	void loadTextures();
	void buildScene();

   private:
	enum Layer {
		Background,
		// Highlight, Pieces, Arrows,
		LayerCount
	};

   private:
	sf::RenderWindow& mWindow;
	TextureHolder mTextures;
	FontHolder& mFonts;

	SceneNode mSceneGraph;
	std::array<SceneNode*, LayerCount> mSceneLayers;
};

#endif  //CHESS_GAMEHANDLER_HPP
