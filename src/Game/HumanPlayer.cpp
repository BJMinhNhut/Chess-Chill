//
// Created by MINH NHUT on 11/21/2023.
//

#include "HumanPlayer.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Cursor.hpp>

HumanPlayer::HumanPlayer(GameHandler& gameHandler, int color) : Player(gameHandler, color) {
	setName("Player");
}

void HumanPlayer::update(sf::Time dt) {
	if (mGameHandler.isDragging()) {
		mGameHandler.snapDraggingToMouse();
	} else {
		sf::Cursor::Type type = mGameHandler.isMouseLegalHover() ? sf::Cursor::Hand : sf::Cursor::Arrow;
		mGameHandler.setCursor(type);
	}
}

void HumanPlayer::handleEvent(const sf::Event& event) {
	if (event.mouseButton.button == sf::Mouse::Left) {
		if (event.type == sf::Event::MouseButtonPressed) {
			handleMousePressed(event.mouseButton.x, event.mouseButton.y);
		} else if (event.type == sf::Event::MouseButtonReleased) {
			handleMouseReleased(event.mouseButton.x, event.mouseButton.y);
		}
	}
}

void HumanPlayer::handleMousePressed(int x, int y) {
	assert(!mGameHandler.isDragging());
	int square = mGameHandler.getHoverSquare(x, y);
	if (square == -1) return;
	if (mGameHandler.isMouseLegalHover())
		mGameHandler.pickUpPiece(square, x, y);
}

void HumanPlayer::handleMouseReleased(int x, int y) {
	int square = mGameHandler.getHoverSquare(x, y);
	if (square == -1) return;
	if (mGameHandler.isDragging())
		mGameHandler.dropPiece(square);
	else
		mGameHandler.moveFromClickedSquare(square);
}

