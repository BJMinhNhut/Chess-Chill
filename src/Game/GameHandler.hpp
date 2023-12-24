//
// Created by MINH NHUT on 10/23/2023.
//

#ifndef CHESS_GAMEHANDLER_HPP
#define CHESS_GAMEHANDLER_HPP

#include "Game/Logic/GameLogic.hpp"
#include "Piece.hpp"
#include "Template/RectNode.hpp"
#include "Template/ResourceHolder.hpp"
#include "Template/ResourceIdentifiers.hpp"
#include "Template/SceneNode.hpp"
#include "Template/SoundPlayer.hpp"
#include "Template/SpriteNode.hpp"
#include "Template/State.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Cursor.hpp>

#include <array>

namespace sf {
class RenderWindow;
class Event;
}  // namespace sf

class GameHandler : public sf::NonCopyable {
	friend GameLogic;

   public:
	const static int BOARD_DRAW_SIZE;

   public:
	GameLogic::Ptr mLogic;

   public:
	explicit GameHandler(State::Context context, sf::Vector2f position);

	void update(sf::Time dt);
	void handleEvent(const sf::Event& event);
	void draw();

	void rotateBoard();

	void handleMove(Move move);
	void moveFromClickedSquare(int to);

	void pickUpPiece(int square, int x, int y);
	void dropPiece(int square);

	void displayPromoteWindow();

	void snapDraggingToMouse();
	void setCursor(sf::Cursor::Type type);

	bool isDragging() const;
	bool isMouseLegalHover() const;
	int getHoverSquare(int x, int y) const;

	bool needPromotion() const;

	void loadPreviousMove();
	void loadNextMove();
	void loadLastMove();
	void loadFirstMove();

   private:
	enum Layer { Background, Pieces, PopUp, LayerCount };

	enum HighlightRate {
		Normal,
		Click,
		MovePiece,
		Target,
		Debug,
	};

   private:
	void buildScene();
	GameLogic* getLogic(GameOptions::Type type);

	sf::Vector2f getBoxPosition(int box) const;

	void addPiece(int square, int piece);
	void movePiece(int from, int to);
	void capturePiece(int square);
	void promotePiece(int square, int piece);
	void postMove();

	void highlightLegalMoves(int from);
	void highlightSquare(int square, HighlightRate rate);
	void highlightMove(int move, bool flag);
	void clearCandidates();

	void loadSnapShot(int index);

   private:
	struct SnapShot {
		Board board;
		int lastMove;

		SnapShot(const Board& board, int lastMove);
	};

   private:
	sf::RenderWindow& mWindow;
	TextureHolder& mTextures;
	SoundPlayer& mSounds;

	SceneNode mSceneGraph;
	std::array<SceneNode*, LayerCount> mSceneLayers;
	std::vector<Piece*> mPieces;
	std::vector<RectNode*> mHighlights;
	std::vector<int> moveCandidates;

	std::vector<SnapShot> mSnapShots;
	int mSnapShotIndex;

	Piece* mDragging;
	int mOldSquare;
	int mLastMove;  // (newBox << 6) | oldBox;

	bool mPromoteWindow;
	int mPromoteFrom, mPromoteTo;

	SpriteNode *mBoardIndex, *mBoardSprite;
	bool mRotated;

	const sf::Vector2f mBoardPosition;
};

#endif  //CHESS_GAMEHANDLER_HPP
