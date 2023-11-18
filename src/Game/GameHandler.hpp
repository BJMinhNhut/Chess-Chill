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
	void draw();
	void handleEvent(const sf::Event& event);

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
	void handleMove(int from, int to, bool drop = false);

	Piece* checkHoverPiece(int x, int y) const;
	int getHoverSquare(int x, int y) const;
	sf::Vector2f getBoxPosition(int box) const;

	void addPiece(int piece, int square);
	void movePiece(int from, int to) override;
	void capturePiece(int square) override;
	void postMove() override;
	void promotePiece(int square, int piece) override;

	void checkDropPiece(int square);
	void checkClick(int square);
	void checkPickUpPiece(int x, int y);
	void handleMouseMoved(int x, int y);

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

	const sf::Vector2f mBoardPosition;
};

#endif  //CHESS_GAMEHANDLER_HPP
