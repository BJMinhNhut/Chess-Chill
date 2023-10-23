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

GameHandler::GameHandler(sf::RenderWindow& window, FontHolder& fonts)
    : mWindow(window),
      mFonts(fonts),
      mTextures(),
      mSceneGraph(),
      mSceneLayers(),
      mPieces(64, nullptr) {
	mWindow.setView(mWindow.getDefaultView());
	loadTextures();
	buildScene();
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
		sf::Cursor::Type type = isHoverPiece(event.mouseMove.x, event.mouseMove.y)
		                            ? sf::Cursor::Hand
		                            : sf::Cursor::Arrow;
		sf::Cursor cursor;
		if (cursor.loadFromSystem(type)) {
			mWindow.setMouseCursor(cursor);
		} else
			throw std::runtime_error("Cannot load hand cursor");
	}
}

void GameHandler::loadTextures() {
	mTextures.load(Textures::Board, Constants::dataPrefix + "resources/images/boards/default.png");
	mTextures.load(Textures::PiecesSet,
	               Constants::dataPrefix + "resources/images/pieces/default.png");
	// load chess pieces from tile map
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

	addPiece(Piece::King | Piece::Black, 0, 0);
}

void GameHandler::addPiece(int type, int row, int column) {
	std::unique_ptr<Piece> piece(new Piece(mTextures.get(Textures::PiecesSet), type));
	piece->setPosition((float)mWindow.getSize().x / 2 - 340.f + float(row * Piece::SIZE),
	                   (float)mWindow.getSize().y / 2 - 340.f + float(column * Piece::SIZE));
	mPieces[row * 8 + column] = piece.get();
	mSceneLayers[Pieces]->attachChild(std::move(piece));
}

bool GameHandler::isHoverPiece(int x, int y) const {
	for (int i = 0; i < 64; ++i) {
		if (mPieces[i] && mPieces[i]->contains(x, y))
			return true;
	}
	return false;
}