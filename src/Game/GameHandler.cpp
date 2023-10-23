//
// Created by MINH NHUT on 10/23/2023.
//

#include "GameHandler.hpp"
#include "Template/Constants.hpp"
#include "Template/ResourceHolder.hpp"
#include "Template/Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <cmath>

GameHandler::GameHandler(sf::RenderWindow& window, FontHolder& fonts)
    : mWindow(window), mFonts(fonts), mTextures(), mSceneGraph(), mSceneLayers() {
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

void GameHandler::loadTextures() {
	mTextures.load(Textures::Board, Constants::dataPrefix + "resources/images/boards/default.png");
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