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

GameHandler::GameHandler(sf::RenderWindow& window, FontHolder& fonts)
    : mWindow(window),
      mFonts(fonts),
      mTextures(),
      mSceneGraph(),
      mSceneLayers(),
      mPieces(64, nullptr),
      mDragging(nullptr),
      mBoardLeft(),
      mBoardTop(),
      oldBox(-1) {
	mWindow.setView(mWindow.getDefaultView());

	loadTextures();
	buildScene();

	loadBoardFromFEN(START_FEN);
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
		checkDropPiece(event.mouseButton.x, event.mouseButton.y);
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
	mPieces[oldBox]->setOpacity(100);
	mSceneLayers[PopUp]->detachChild(*mDragging);
	mDragging = nullptr;
	const int newBox = getHoverBox(x, y);
	movePiece(oldBox, newBox);
}

void GameHandler::checkPickUpPiece(int x, int y) {
	assert(mDragging == nullptr);
	Piece* hovering = checkHoverPiece(x, y);

	if (hovering == nullptr)
		return;

	hovering->setOpacity(50);
	mDragging = hovering->clone();
	mSceneLayers[PopUp]->attachChild(SceneNode::Ptr(mDragging));
	oldBox = getHoverBox(x, y);
	mDragging->snap(x, y);
}

void GameHandler::loadTextures() {
	mTextures.load(Textures::Board, Constants::dataPrefix + "resources/images/boards/default.png");
	mTextures.load(Textures::PiecesSet,
	               Constants::dataPrefix + "resources/images/pieces/alpha.png");
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

int GameHandler::getPieceFromChar(char ch) {
	int piece = std::islower(ch) ? Piece::Black : Piece::White;
	switch (tolower(ch)) {
		case 'p':
			piece |= Piece::Pawn;
			break;
		case 'n':
			piece |= Piece::Knight;
			break;
		case 'b':
			piece |= Piece::Bishop;
			break;
		case 'r':
			piece |= Piece::Rook;
			break;
		case 'q':
			piece |= Piece::Queen;
			break;
		case 'k':
			piece |= Piece::King;
			break;
		default:
			assert(false);
	}
	return piece;
}

int GameHandler::getBoxID(int row, int column) {
	assert(row >= 0 && row < 8 && column >= 0 && column < 8);
	return (row << 3) | column;
}

void GameHandler::loadBoardFromFEN(const std::string& fen) {
	int row = 0, col = 0;
	for (char ch : fen) {
		if (std::isdigit(ch))
			col += ch - '0';
		else if (ch == '/')
			row++, col = 0;
		else if (std::isalpha(ch))
			addPiece(getPieceFromChar(ch), getBoxID(row, col++));
		else
			break;
	}
	// TODO: active, castling, en-passant, half-move, full move
}

void GameHandler::addPiece(int type, int box) {
	std::unique_ptr<Piece> piece(new Piece(mTextures.get(Textures::PiecesSet), type));
	piece->setPosition(float(mBoardLeft + (box % 8) * Piece::SIZE),
	                   float(mBoardTop + (box >> 3) * Piece::SIZE));
	mPieces[box] = piece.get();
	mSceneLayers[Pieces]->attachChild(std::move(piece));
}

void GameHandler::movePiece(int oldBox, int newBox) {
	std::cout << oldBox << ' ' << newBox << '\n';
	assert(oldBox >= 0 && oldBox < 64);

	// oldBox must be valid
	assert(mPieces[oldBox] != nullptr);

	// if newBox not valid then move to old position
	if (!(newBox >= 0 && newBox < 64))
		newBox = oldBox;

	// if chang box then remove piece at new box, move piece at old box to new box, and remove old box marking
	if (newBox != oldBox) {
		if (mPieces[newBox] != nullptr)
			capturePiece(newBox);

		mPieces[newBox] = mPieces[oldBox];

		mPieces[oldBox] = nullptr;
		oldBox = -1;
	}
	mPieces[newBox]->setPosition(mBoardLeft + (newBox % 8) * Piece::SIZE,
	                             mBoardTop + (newBox >> 3) * Piece::SIZE);
}

void GameHandler::capturePiece(int box) {
	mSceneLayers[Pieces]->detachChild(*mPieces[box]);
	mPieces[box] = nullptr;
}

int GameHandler::getHoverBox(int x, int y) const {
	if (x < mBoardLeft || y < mBoardTop)
		return -1;
	int column = (x - mBoardLeft) / Piece::SIZE;
	int row = (y - mBoardTop) / Piece::SIZE;
	if (row > 7 || column > 7)
		return -1;
	return getBoxID(row, column);
}

Piece* GameHandler::checkHoverPiece(int x, int y) const {
	int boxID = getHoverBox(x, y);
	return (boxID < 0) ? nullptr : mPieces[boxID];
}