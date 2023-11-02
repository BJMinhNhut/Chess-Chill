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

const int GameHandler::BOARD_SIZE = 680;
const std::string GameHandler::START_FEN =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

GameHandler::GameHandler(sf::RenderWindow& window, FontHolder& fonts, SoundPlayer& sounds)
    : mWindow(window),
      mFonts(fonts),
      mSounds(sounds),
      mTextures(),
      mSceneGraph(),
      mSceneLayers(),
      mPieces(64, nullptr),
      mHighlights(64, nullptr),
      mDragging(nullptr),
      mBoardLeft(),
      mBoardTop(),
      mOldBox(-1),
      mLastMove(-1),
      mLogic(START_FEN) {
	mWindow.setView(mWindow.getDefaultView());

	loadTextures();
	buildScene();
	loadPieces();
}

void GameHandler::draw() {
	mWindow.draw(mSceneGraph);
}

void GameHandler::update(sf::Time dt) {
	mSceneGraph.update(dt);
}

void GameHandler::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::MouseMoved) {
		handleMouseMoved(event.mouseMove.x, event.mouseMove.y);
	} else if (event.type == sf::Event::MouseButtonPressed) {
		checkPickUpPiece(event.mouseButton.x, event.mouseButton.y);
	} else if (event.type == sf::Event::MouseButtonReleased) {
		if (mDragging == nullptr && mOldBox > -1 && mPieces[mOldBox] != nullptr) {
			handleMove(mOldBox, getHoverBox(event.mouseButton.x, event.mouseButton.y));
		} else checkDropPiece(event.mouseButton.x, event.mouseButton.y);
	}
}

void GameHandler::loadPieces() {
	for (int i = 0; i < 64; ++i) {
		int piece = mLogic.getPiece(i);
		std::cout << piece << ' ' << i << '\n';
		if (piece != 0) {
			addPiece(piece, i);
		}
	}
}

void GameHandler::handleMouseMoved(int x, int y) {
	//		std::cerr << event.mouseMove.x << ' ' << event.mouseMove.y << '\n';
	if (mDragging) {
		mDragging->snap(x, y);
	} else {
		sf::Cursor::Type type = checkHoverPiece(x, y) ? sf::Cursor::Hand : sf::Cursor::Arrow;
		sf::Cursor cursor;
		if (cursor.loadFromSystem(type)) {
			mWindow.setMouseCursor(cursor);
		} else
			throw std::runtime_error("Cannot load hand cursor");
	}
}

void GameHandler::checkDropPiece(int x, int y) {
	if (mDragging == nullptr)
		return;
	mPieces[mOldBox]->setOpacity(100);
	const int newBox = getHoverBox(x, y);
	handleMove(mOldBox, newBox);

	mSceneLayers[PopUp]->detachChild(*mDragging);
	mDragging = nullptr;
}

void GameHandler::handleMove(int start, int target) {
	bool isLegalMove = start >= 0 && start < 64 && target >= 0 && target < 64 &&
	                    mLogic.isLegalMove((target << 6) | start);

	// if legal move, then highlight new move, make that move, un-highlight old move
	if (isLegalMove) {
		if (mLastMove > -1)
			highlightMove(mLastMove, false);
		highlightMove(target << 6 | start, true);
		movePiece(start, target);
	} else {
		highlightBox(mOldBox, Click);
		if (mLastMove > -1)
			highlightMove(mLastMove, true);
	}
}

void GameHandler::checkPickUpPiece(int x, int y) {
	assert(mDragging == nullptr);
	Piece* hovering = checkHoverPiece(x, y);

	if (hovering == nullptr)
		return;

	// dim old position and highlight old box
	if ((mLastMove == -1 || (mLastMove & 0x3f) != mOldBox) && mOldBox != -1)
		highlightBox(mOldBox, Normal);
	mOldBox = getHoverBox(x, y);
	hovering->setOpacity(50);
	highlightBox(mOldBox, Click);

	// create a fake piece that following player cursor
	mDragging = hovering->clone();
	mDragging->snap(x, y);
	mSceneLayers[PopUp]->attachChild(SceneNode::Ptr(mDragging));
}

void GameHandler::highlightBox(int box, GameHandler::HighlightRate rate) {
	if (mHighlights[box] != nullptr) {
		mSceneLayers[Background]->detachChild(*mHighlights[box]);
		mHighlights[box] = nullptr;
	}
	if (rate > Normal) {
		mHighlights[box] = new RectNode(Piece::SIZE, Piece::SIZE,
		                                sf::Color(0, 150, 150, rate == Click ? 50 : 100));
		mHighlights[box]->setPosition(getBoxPosition(box));
		mSceneLayers[Background]->attachChild(SceneNode::Ptr(mHighlights[box]));
	}
}

void GameHandler::highlightMove(int move, bool flag) {
	int oldBox = move & 0x3f, newBox = move >> 6;
	if (flag) {
		highlightBox(newBox, Move);
		highlightBox(oldBox, Move);
	} else {
		highlightBox(newBox, Normal);
		highlightBox(oldBox, Normal);
	}
}

void GameHandler::loadTextures() {
	mTextures.load(Textures::Board, Constants::dataPrefix + "resources/images/boards/default.png");
	mTextures.load(Textures::PiecesSet,
	               Constants::dataPrefix + "resources/images/pieces/default.png");
}

void GameHandler::buildScene() {
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i) {
		SceneNode::Ptr layer(new SceneNode());
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	// Add the background sprite to the scene
	mBoardLeft = (int)mWindow.getSize().x / 2 - BOARD_SIZE / 2;
	mBoardTop = (int)mWindow.getSize().y / 2 - BOARD_SIZE / 2;

	std::unique_ptr<SpriteNode> boardSprite(new SpriteNode(mTextures.get(Textures::Board)));
	boardSprite->setPosition((float)mBoardLeft, (float)mBoardTop);
	mSceneLayers[Background]->attachChild(std::move(boardSprite));
}

void GameHandler::addPiece(int type, int box) {
	mPieces[box] = new Piece(mTextures.get(Textures::PiecesSet), type);
	mPieces[box]->setPosition(getBoxPosition(box), false);
	mPieces[box]->updateTargetPosition();
	mSceneLayers[Pieces]->attachChild(SceneNode::Ptr(mPieces[box]));
}

void GameHandler::movePiece(int oldBox, int newBox) {
	std::cout << "move " << oldBox << ' ' << newBox << '\n';
	// if chang box then remove piece at new box, move piece at old box to new box, and remove old box marking
	if (newBox != oldBox) {
		if (mPieces[newBox] != nullptr) {
			capturePiece(newBox);
			mSounds.play(SoundEffect::Capture);
		} else {
			mSounds.play(SoundEffect::Move);
		}

		mLogic.makeMove((newBox << 6) | oldBox);
		mPieces[newBox] = mPieces[oldBox];
		mPieces[oldBox] = nullptr;
		mLastMove = (newBox << 6) | oldBox;
		oldBox = -1;
	} else
		mLastMove = -1;
	mPieces[newBox]->setPosition(getBoxPosition(newBox), mDragging == nullptr);
}

void GameHandler::capturePiece(int box) {
	std::cout << "capture " << box << '\n';
	mSceneLayers[Pieces]->detachChild(*mPieces[box]);
	mPieces[box] = nullptr;
}

int GameHandler::getHoverBox(int x, int y) const {
	if (x < mBoardLeft || y < mBoardTop)
		return -1;
	int column = 7 - (x - mBoardLeft) / Piece::SIZE;
	int row = 7 - (y - mBoardTop) / Piece::SIZE;
	if (row >= 8 || column >= 8 || row < 0 || column < 0)
		return -1;
	return GameLogic::getBoxID(row, column);
}

Piece* GameHandler::checkHoverPiece(int x, int y) const {
	int boxID = getHoverBox(x, y);
	return (boxID < 0) ? nullptr : mPieces[boxID];
}

sf::Vector2f GameHandler::getBoxPosition(int box) const {
	return {(float)mBoardLeft + float((7-(box & 7)) * Piece::SIZE),
	                    (float)mBoardTop + float((7-(box >> 3)) * Piece::SIZE)};
}