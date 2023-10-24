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
	const static std::string START_FEN;
	const static int BOARD_SIZE;
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
	int getHoverBox(int x, int y) const;

	static int getPieceFromChar(char ch);
	static int getBoxID(int row, int column);

	void addPiece(int type, int box);
	void movePiece(int oldBox, int newBox);

	void checkDropPiece(int x, int y);
	void checkPickUpPiece(int x, int y);
	void handleMouseMoved(int x, int y);

   private:
	enum Layer {
		Background,
		Pieces,
		PopUp,
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
	int oldBox;

	int mBoardLeft, mBoardTop;
	void capturePiece(int box);
};

#endif  //CHESS_GAMEHANDLER_HPP
