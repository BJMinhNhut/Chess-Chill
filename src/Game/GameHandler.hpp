//
// Created by MINH NHUT on 10/23/2023.
//

#ifndef CHESS_GAMEHANDLER_HPP
#define CHESS_GAMEHANDLER_HPP

#include "GameLogic.hpp"
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
#include <SFML/Window/Cursor.hpp>

#include <array>

namespace sf {
class RenderWindow;
class Event;
}  // namespace sf

class GameHandler : public sf::NonCopyable, public GameLogic {
   public:
	const static std::string START_FEN;
	const static std::string ONLY_KINGS_FEN;
	const static int BOARD_DRAW_SIZE;

   public:
	explicit GameHandler(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts,
	                     SoundPlayer& sounds, sf::Vector2f position);

	void update(sf::Time dt);
	void handleEvent(const sf::Event& event);
	void draw();

	void rotateBoard();

	void handleMouseMoved(int x, int y);
	void handleMove(int from, int to);
	void moveFromClickedSquare(int to);

	void pickUpPiece(int square, int x, int y);
	void dropPiece(int square);

	void displayPromoteWindow(int square);
	void promotePiece(int square, int piece) override;

	void snapDraggingToMouse();
	void setCursor(sf::Cursor::Type type);

	bool isDragging() const;
	bool isMouseLegalHover() const;
	int getHoverSquare(int x, int y) const;

   private:
	enum Layer { Background, Pieces, PopUp, LayerCount };

	enum HighlightRate {
		Normal,
		Click,
		Move,
		Target,
		Debug,
	};

   private:
	void buildScene();

	sf::Vector2f getBoxPosition(int box) const;

	void addPiece(int piece, int square);
	void movePiece(int from, int to) override;
	void capturePiece(int square) override;
	void postMove() override;

	void highlightLegalMoves(int from);
	void highlightSquare(int square, HighlightRate rate);
	void highlightMove(int move, bool flag);
	void clearCandidates();

   private:
	sf::RenderWindow& mWindow;
	TextureHolder& mTextures;
	FontHolder& mFonts;
	SoundPlayer& mSounds;

	SceneNode mSceneGraph;
	std::array<SceneNode*, LayerCount> mSceneLayers;
	std::vector<Piece*> mPieces;
	std::vector<RectNode*> mHighlights;
	std::vector<int> moveCandidates;

	Piece* mDragging;
	int mOldSquare;
	int mLastMove;  // (newBox << 6) | oldBox;

	SpriteNode* mBoardIndex;
	bool mRotated;

	const sf::Vector2f mBoardPosition;
};

#endif  //CHESS_GAMEHANDLER_HPP
