//
// Created by MINH NHUT on 11/21/2023.
//

#include "HumanPlayer.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Cursor.hpp>

HumanPlayer::HumanPlayer(GameHandler& gameHandler, int color) : Player(gameHandler, color) {}

void HumanPlayer::update(sf::Time dt) {}

void HumanPlayer::handleEvent(const sf::Event& event) {
	if (event.mouseButton.button == sf::Mouse::Left) {
		if (event.type == sf::Event::MouseButtonPressed) {
			handleMousePressed(event.mouseButton.x, event.mouseButton.y);
		} else if (event.type == sf::Event::MouseButtonReleased) {
			handleMouseReleased(event.mouseButton.x, event.mouseButton.y);
		}
	} else if (event.type == sf::Event::MouseMoved) {
		handleMouseMoved(event.mouseButton.x, event.mouseButton.y);
	}
}

void HumanPlayer::handleMousePressed(int x, int y) {
	assert(!mGameHandler.isDragging());
	int square = mGameHandler.getHoverSquare(x, y);
	if (square == -1) return;
	if (mGameHandler.isLegalHover(x, y))
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

void HumanPlayer::handleMouseMoved(int x, int y) {
	//		std::cerr << event.mouseMove.x << ' ' << event.mouseMove.y << '\n';
	if (mGameHandler.isDragging()) {
		mGameHandler.snapDragging(x, y);
	} else {
		sf::Cursor::Type type = mGameHandler.isLegalHover(x, y) ? sf::Cursor::Hand : sf::Cursor::Arrow;
		mGameHandler.setCursor(type);
	}
}
