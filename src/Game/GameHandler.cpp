//
// Created by MINH NHUT on 10/23/2023.
//

#include "GameHandler.hpp"
#include "Template/Constants.hpp"
#include "Template/ResourceHolder.hpp"
#include "Template/Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <cmath>
#include <iostream>

const int GameHandler::BOARD_DRAW_SIZE = 680;
const std::string GameHandler::START_FEN =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const std::string GameHandler::ONLY_KINGS_FEN = "k7/8/8/8/8/8/8/7K w - - 0 1";

GameHandler::GameHandler(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts,
                         SoundPlayer& sounds, sf::Vector2f position)
    : mWindow(window),
      mFonts(fonts),
      mSounds(sounds),
      mTextures(textures),
      mSceneGraph(),
      mSceneLayers(),
      mPieces(GameLogic::BOARD_SIZE, nullptr),
      mHighlights(GameLogic::BOARD_SIZE, nullptr),
      mDragging(nullptr),
      mBoardIndex(nullptr),
      mBoardSprite(nullptr),
      mBoardPosition(position),
      mOldSquare(-1),
      mLastMove(-1),
      GameLogic(START_FEN),
      mRotated(false),
      moveCandidates() {
	mWindow.setView(mWindow.getDefaultView());
	buildScene();
}

void GameHandler::buildScene() {
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i) {
		SceneNode::Ptr layer(new SceneNode());
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	mBoardIndex = new SpriteNode(mTextures.get(Textures::BoardIndexWhite));
	mBoardIndex->setPosition(262.f, 105.f);
	mSceneLayers[Background]->attachChild(SceneNode::Ptr(mBoardIndex));

	mBoardSprite = new SpriteNode(mTextures.get(Textures::Board));
	mBoardSprite->centerOrigin();
	mBoardSprite->setPosition(mBoardPosition + sf::Vector2f(BOARD_DRAW_SIZE/2.f, BOARD_DRAW_SIZE/2.f));
	mSceneLayers[Background]->attachChild(SceneNode::Ptr(mBoardSprite));

	for (int square = 0; square < GameLogic::BOARD_SIZE; ++square) {
		int piece = getPiece(square);
		if (piece != 0)
			addPiece(piece, square);
	}
}

void GameHandler::draw() {
	mWindow.draw(mSceneGraph);
}

void GameHandler::update(sf::Time dt) {
	GameLogic::updateTime(dt);
	mSceneGraph.update(dt);
	mSounds.removeStoppedSounds();
}

void GameHandler::rotateBoard() {
	mSounds.play(SoundEffect::Castling);
	mRotated = !mRotated;
	mBoardIndex->setTexture(
	    mTextures.get(mRotated ? Textures::BoardIndexBlack : Textures::BoardIndexWhite));
	mBoardSprite->rotate(180.f);
	for (int square = 0; square < GameLogic::BOARD_SIZE; ++square) {
		if (mPieces[square] != nullptr) {
			mPieces[square]->setPosition(getBoxPosition(square), false);
			mPieces[square]->updateTargetPosition();
		}
		if (mHighlights[square] != nullptr)
			mHighlights[square]->setPosition(getBoxPosition(square));
	}
}

void GameHandler::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::MouseMoved)
		handleMouseMoved(event.mouseButton.x, event.mouseButton.y);
}

void GameHandler::setCursor(sf::Cursor::Type type) {
	sf::Cursor cursor;
	if (cursor.loadFromSystem(type)) {
		mWindow.setMouseCursor(cursor);
	} else
		throw std::runtime_error("Cannot load new cursor");
}

void GameHandler::handleMouseMoved(int x, int y) {}

bool GameHandler::isDragging() const {
	return mDragging != nullptr;
}

void GameHandler::snapDraggingToMouse() {
	sf::Vector2i mouse = sf::Mouse::getPosition(mWindow);
	mDragging->snap(mouse.x, mouse.y);
}

bool GameHandler::isMouseLegalHover() const {
	sf::Vector2i mouse = sf::Mouse::getPosition(mWindow);
	int square = getHoverSquare(mouse.x, mouse.y);
	return (Board::validSquare(square) && mPieces[square] != nullptr &&
	        mPieces[square]->color() == getTurn());
}

void GameHandler::dropPiece(int square) {
	mPieces[mOldSquare]->setOpacity(100);
	mSceneLayers[PopUp]->detachChild(*mDragging);
	mDragging = nullptr;
	if (square != mOldSquare) {
		if (Board::validSquare(square))
			handleMove(Move(mOldSquare, square));
		else
			mSounds.play(SoundEffect::OutOfBound);
	}
}

void GameHandler::pickUpPiece(int square, int x, int y) {
	Piece* hovering = mPieces[square];

	// dim old position and highlight old box
	if ((mLastMove == -1 || (mLastMove & 0x3f) != mOldSquare) && mOldSquare != -1)
		highlightSquare(mOldSquare, Normal);
	clearCandidates();

	mOldSquare = square;
	hovering->setOpacity(50);
	highlightSquare(mOldSquare, Click);
	highlightMove(mLastMove, true);

	// create a fake piece that following player cursor
	mDragging = hovering->clone();
	mDragging->snap(x, y);
	mSceneLayers[PopUp]->attachChild(SceneNode::Ptr(mDragging));

	highlightLegalMoves(square);
}

void GameHandler::moveFromClickedSquare(int to) {
	handleMove(Move(mOldSquare, to));
}

void GameHandler::handleMove(Move move) {
	bool legal = isLegalMove(move);

	clearCandidates();

	// if legal move, then highlight new move, make that move, un-highlight old move, and to squares
	int from = move.from(), to = move.to();
	if (legal && from != to) {
		highlightMove(mLastMove, false);
		highlightMove(to << 6 | from, true);
		makeMove(move);
		if (isFinished())
			setCursor(sf::Cursor::Arrow);

#ifdef DEBUG_ATTACK
		for (int i = 0; i < 64; ++i) {
			if (isAttacked(i))
				highlightSquare(i, Debug);
			else
				highlightSquare(i, Normal);
		}
#endif  // DEBUG_ATTACK
		mOldSquare = -1;
	} else {
		highlightMove(mLastMove, true);
		std::cerr << "Illegal move\n";
	}
}

void GameHandler::highlightLegalMoves(int from) {
	std::vector<Move> moves = getMoveList(from);
	for (Move move : moves) {
		highlightSquare(move.to(), Target);
		moveCandidates.push_back(move.to());
	}
}

void GameHandler::highlightSquare(int square, HighlightRate rate) {
	if (mHighlights[square] != nullptr) {
		mSceneLayers[Background]->detachChild(*mHighlights[square]);
		mHighlights[square] = nullptr;
	}
	if (rate > Normal) {
		sf::Color color;
		switch (rate) {
			case Click:
				color = sf::Color(0, 150, 150, 50);
				break;
			case MovePiece:
				color = sf::Color(0, 150, 150, 100);
				break;
			case Target:
				color = sf::Color(205, 175, 0, 155);
				break;
			case Debug:
				color = sf::Color(255, 0, 0, 100);
				break;
			default:
				color = sf::Color(0, 150, 150, 100);
				break;
		}
		mHighlights[square] = new RectNode(Piece::SIZE, Piece::SIZE, color);
		mHighlights[square]->setPosition(getBoxPosition(square));
		mSceneLayers[Background]->attachChild(SceneNode::Ptr(mHighlights[square]));
	}
}

void GameHandler::highlightMove(int move, bool flag) {
	if (move == -1)
		return;  // no move
	int oldBox = move & 0x3f, newBox = move >> 6;
	if (flag) {
		highlightSquare(newBox, MovePiece);
		highlightSquare(oldBox, MovePiece);
	} else {
		highlightSquare(newBox, Normal);
		highlightSquare(oldBox, Normal);
	}
}

void GameHandler::addPiece(int piece, int square) {
	mPieces[square] = new Piece(mTextures.get(Textures::PieceSet), piece);
	mPieces[square]->setPosition(getBoxPosition(square), false);
	mPieces[square]->updateTargetPosition();
	mSceneLayers[Pieces]->attachChild(SceneNode::Ptr(mPieces[square]));
}

void GameHandler::clearCandidates() {
	for (int square : moveCandidates)
		highlightSquare(square, Normal);
	std::vector<int>().swap(moveCandidates);
	if (mOldSquare > -1) {
		highlightSquare(mOldSquare, Normal);
		mOldSquare = -1;
	}
}

void GameHandler::capturePiece(int square) {
	GameLogic::capturePiece(square);
	mSceneLayers[Pieces]->detachChild(*mPieces[square]);
	mPieces[square] = nullptr;
}

void GameHandler::movePiece(int from, int to) {
	GameLogic::movePiece(from, to);
	mPieces[to] = mPieces[from];
	mPieces[from] = nullptr;
	mLastMove = (to << 6) | from;
	mPieces[to]->setPosition(getBoxPosition(to), Piece::None);
}

void GameHandler::postMove() {
	GameLogic::postMove();
	switch (lastMoveStatus()) {
		case Check:
			mSounds.play(SoundEffect::Check);
			break;
		case Capture:
			mSounds.play(SoundEffect::Capture);
			break;
		case Castling:
			mSounds.play(SoundEffect::Castling);
			break;
		case Promotion:
			mSounds.play(SoundEffect::Promotion);
			break;
		default:
			mSounds.play(SoundEffect::Move);
	}
}

void GameHandler::promotePiece(int square, int piece) {
	GameLogic::promotePiece(square, piece);
	addPiece(piece, square);
}

int GameHandler::getHoverSquare(int x, int y) const {
	if (x < mBoardPosition.x || y < mBoardPosition.y || x > mBoardPosition.x + BOARD_DRAW_SIZE ||
	    y > mBoardPosition.y + BOARD_DRAW_SIZE)
		return -1;
	int column = (x - (int)mBoardPosition.x) / Piece::SIZE;
	int row = 7 - (y - (int)mBoardPosition.y) / Piece::SIZE;

	if (mRotated) {
		row = 7 - row;
		column = 7 - column;
	}

	if (row >= 8 || column >= 8 || row < 0 || column < 0)
		return -1;
	return Board::getSquareID(row, column);
}

sf::Vector2f GameHandler::getBoxPosition(int box) const {
	int row, column;
	row = box >> 3;
	column = box & 7;
	if (mRotated) {
		row = 7 - row;
		column = 7 - column;
	}
	return mBoardPosition +
	       sf::Vector2f(float(column * Piece::SIZE), float((7 - row) * Piece::SIZE));
}