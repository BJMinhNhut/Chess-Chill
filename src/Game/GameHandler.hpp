//
// Created by MINH NHUT on 10/23/2023.
//

#ifndef CHESS_GAMEHANDLER_HPP
#define CHESS_GAMEHANDLER_HPP

#include "Piece.hpp"
#include "Template/RectNode.hpp"
#include "Template/ResourceHolder.hpp"
#include "Template/ResourceIdentifiers.hpp"
#include "Template/SceneNode.hpp"
#include "Template/SoundPlayer.hpp"
#include "Template/SpriteNode.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <array>

namespace sf {
class RenderWindow;
class Event;
}  // namespace sf

class GameHandler : public sf::NonCopyable {
   public:
	const static std::string START_FEN;
	const static int BOARD_SIZE;

   public:
	explicit GameHandler(sf::RenderWindow& window, FontHolder& fonts, SoundPlayer& sounds);

	void update(sf::Time dt);
	void draw();
	void handleEvent(const sf::Event& event);

	void loadBoardFromFEN(const std::string& fen);

	void loadTextures();
	void buildScene();
	void handleMove(int start, int target);

   private:
	enum Layer { Background, Pieces, PopUp, LayerCount };

	enum HighlightRate {
		Normal,
		Move,
		Click,
	};

   private:
	Piece* checkHoverPiece(int x, int y) const;
	int getHoverBox(int x, int y) const;
	sf::Vector2f getBoxPosition(int box) const;

	static int getPieceFromChar(char ch);
	static int getBoxID(int row, int column);

	void addPiece(int type, int box);
	void movePiece(int oldBox, int newBox);
	void capturePiece(int box);

	void checkDropPiece(int x, int y);
	void checkPickUpPiece(int x, int y);
	void handleMouseMoved(int x, int y);

	void highlightBox(int box, HighlightRate rate);
	void highlightMove(int move, bool flag);

   private:
	sf::RenderWindow& mWindow;
	TextureHolder mTextures;
	FontHolder& mFonts;
	SoundPlayer& mSounds;

	SceneNode mSceneGraph;
	std::array<SceneNode*, LayerCount> mSceneLayers;
	std::vector<Piece*> mPieces;
	std::vector<RectNode*> mHighlights;

	Piece* mDragging;
	int mOldBox;
	int mLastMove;  // (newBox << 6) | oldBox;

	int mBoardLeft, mBoardTop;
};

#endif  //CHESS_GAMEHANDLER_HPP
