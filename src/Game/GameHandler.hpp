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
#include "GameLogic.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <array>

namespace sf {
class RenderWindow;
class Event;
}  // namespace sf

class GameHandler : public sf::NonCopyable, public GameLogic {
   public:
	const static std::string START_FEN;
	const static int BOARD_DRAW_SIZE;

   public:
	explicit GameHandler(sf::RenderWindow& window, FontHolder& fonts, SoundPlayer& sounds);

	void update(sf::Time dt);
	void draw();
	void handleEvent(const sf::Event& event);

   private:
	enum Layer { Background, Pieces, PopUp, LayerCount };

	enum HighlightRate {
		Normal,
		Click,
		Move,
		Target,
	};

   private:
	void loadTextures();
	void buildScene();
	void loadPieces();
	void handleMove(int start, int target, bool drop = false);

	Piece* checkHoverPiece(int x, int y) const;
	int getHoverSquare(int x, int y) const;
	sf::Vector2f getBoxPosition(int box) const;

	// TODO: inherit from GameLogic
	void addPiece(int piece, int square) override;
	void movePiece(int from, int to, bool captured) override;
	void capturePiece(int square) override;

	void checkDropPiece(int square);
	void checkClick(int square);
	void checkPickUpPiece(int x, int y);
	void handleMouseMoved(int x, int y);

	void highlightBox(int box, HighlightRate rate);
	void highlightMove(int move, bool flag);
	void clearCandidates();

   private:
	sf::RenderWindow& mWindow;
	TextureHolder mTextures;
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

	int mBoardLeft, mBoardTop;
};

#endif  //CHESS_GAMEHANDLER_HPP
