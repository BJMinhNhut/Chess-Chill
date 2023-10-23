//
// Created by MINH NHUT on 10/23/2023.
//

#ifndef CHESS_GAMEHANDLER_HPP
#define CHESS_GAMEHANDLER_HPP

#include "Piece.hpp"
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
class Event;
}

class GameHandler : public sf::NonCopyable {
   public:
	const std::string START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
   public:
	explicit GameHandler(sf::RenderWindow& window, FontHolder& fonts);

	void update(sf::Time dt);
	void draw();
	void handleEvent(const sf::Event& event);

	void loadBoardFromFEN(const std::string& fen);

	void loadTextures();
	void buildScene();

   private:
	Piece* checkHoverPiece(int x, int y) const;
	static int getPieceFromChar(char ch) ;

	void addPiece(int type, int row, int column);

   private:
	enum Layer {
		Background,
		// Highlight,
		Pieces,
		//Arrows,
		LayerCount
	};

   private:
	sf::RenderWindow& mWindow;
	TextureHolder mTextures;
	FontHolder& mFonts;

	SceneNode mSceneGraph;
	std::array<SceneNode*, LayerCount> mSceneLayers;
	std::vector<Piece*> mPieces;

	Piece* mDragging;
};

#endif  //CHESS_GAMEHANDLER_HPP
