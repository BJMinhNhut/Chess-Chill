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
      mDragging(nullptr) {
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
		//		std::cerr << event.mouseMove.x << ' ' << event.mouseMove.y << '\n';
		if (mDragging) {
			mDragging->setPosition(event.mouseMove.x - Piece::SIZE / 2,
			                       event.mouseMove.y - Piece::SIZE / 2);
		} else {
			sf::Cursor::Type type = checkHoverPiece(event.mouseMove.x, event.mouseMove.y)
			                            ? sf::Cursor::Hand
			                            : sf::Cursor::Arrow;
			sf::Cursor cursor;
			if (cursor.loadFromSystem(type)) {
				mWindow.setMouseCursor(cursor);
			} else
				throw std::runtime_error("Cannot load hand cursor");
		}
	} else if (event.type == sf::Event::MouseButtonPressed) {
		mDragging = checkHoverPiece(event.mouseButton.x, event.mouseButton.y);
		if (mDragging) {
			mDragging->setPosition(event.mouseButton.x - Piece::SIZE / 2,
			                       event.mouseButton.y - Piece::SIZE / 2);
		}
	} else if (event.type == sf::Event::MouseButtonReleased) {
		if (mDragging)
			mDragging = nullptr;
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
	std::unique_ptr<SpriteNode> boardSprite(new SpriteNode(mTextures.get(Textures::Board)));
	boardSprite->setPosition((float)mWindow.getSize().x / 2, (float)mWindow.getSize().y / 2);
	boardSprite->centerOrigin();
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

void GameHandler::loadBoardFromFEN(const std::string& fen) {
	int row = 0, col = 0;
	for (char ch : fen) {
		if (std::isdigit(ch))
			col += ch - '0';
		else if (ch == '/')
			row++, col = 0;
		else if (std::isalpha(ch))
			addPiece(getPieceFromChar(ch), row, col++);
		else
			break;
	}
	// TODO: active, castling, en-passant, half-move, full move
}

void GameHandler::addPiece(int type, int row, int column) {
	std::unique_ptr<Piece> piece(new Piece(mTextures.get(Textures::PiecesSet), type));
	piece->setPosition((float)mWindow.getSize().x / 2 - 340.f + float(column * Piece::SIZE),
	                   (float)mWindow.getSize().y / 2 - 340.f + float(row * Piece::SIZE));
	mPieces[row * 8 + column] = piece.get();
	mSceneLayers[Pieces]->attachChild(std::move(piece));
}

Piece* GameHandler::checkHoverPiece(int x, int y) const {
	static const int midx = mWindow.getSize().x/2, midy = mWindow.getSize().y/2;
	static const int left = midx - BOARD_SIZE/2, top = midy - BOARD_SIZE/2;
	if (x < left || y < top) return nullptr;
	int column = (x - left) / Piece::SIZE;
	int row = (y - top) / Piece::SIZE;
	if (row < 0 || row > 7 || column < 0 || column > 7) return nullptr;
	return mPieces[row * 8 + column];
}