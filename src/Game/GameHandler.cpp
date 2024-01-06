//
// Created by MINH NHUT on 10/23/2023.
//

#include "GameHandler.hpp"
#include "Game/Logic/Chess960Logic.hpp"
#include "Game/Logic/FenGenerator.hpp"
#include "Game/Logic/KothLogic.hpp"
#include "Game/Logic/StandardLogic.hpp"
#include "GameSaver.hpp"
#include "Template/Constants.hpp"
#include "Template/ResourceHolder.hpp"
#include "Template/Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <cmath>
#include <iostream>

//#define DEBUG_ATTACK

const int GameHandler::BOARD_DRAW_SIZE = 680;

GameHandler::GameHandler(State::Context context, sf::Vector2f position)
    : mWindow(*context.window),
      mSounds(*context.sounds),
      mTextures(*context.textures),
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
      mLogic(nullptr),
      mPromoteWindow(false),
      mPromoteFrom(-1),
      mPromoteTo(-1),
      mRotated(false),
      moveCandidates(),
      mSnapShotIndex(0),
      mSaver(*context.options) {
	mWindow.setView(mWindow.getDefaultView());

	buildScene();
	switch (*context.mode) {
		case State::Context::Normal:
			std::cout << "Game : Normal Mode\n";
			initNormalGame();
			break;
		case State::Context::Review:
			std::cout << "Game : Review Mode\n";
			initReviewGame(context.oldGames->getPath());
			break;
		case State::Context::Puzzles:
			std::cout << "Game : Puzzle Mode\n";
			initPuzzleGame(*context.puzzle);
			break;
		default:
			throw(std::invalid_argument("Invalid mode"));
			break;
	}
}

void GameHandler::initNormalGame() {
	mLogic = GameLogic::Ptr(getLogic(mSaver.getOptions().getType()));
	for (int square = 0; square < GameLogic::BOARD_SIZE; ++square) {
		int piece = mLogic->getPiece(square);
		if (piece != 0)
			addPiece(square, piece);
	}
	saveSnapShot();
}

void GameHandler::initReviewGame(const std::string& path) {
	//		std::cout << "Load from: " << context.oldGames->getPath() << "\n";
	mSaver.load(path);
	//		context.oldGames->resetIndex();
	loadFirstMove();
}

void GameHandler::initPuzzleGame(const Puzzle& puzzle) {
	std::cout << "Load fen: " << puzzle.getFen() << "\n";
	mLogic = GameLogic::Ptr(new StandardLogic(puzzle.getFen(), this));
	for (int square = 0; square < GameLogic::BOARD_SIZE; ++square) {
		int piece = mLogic->getPiece(square);
		if (piece != 0)
			addPiece(square, piece);
	}
}

GameLogic* GameHandler::getLogic(GameOptions::Type type) {
	switch (type) {
		case GameOptions::Chess960:
			return new Chess960Logic(this);
		case GameOptions::KingOfTheHill:
			return new KothLogic(this);
		default:
			return new StandardLogic(this);
	}
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
	mBoardSprite->setPosition(mBoardPosition +
	                          sf::Vector2f(BOARD_DRAW_SIZE / 2.f, BOARD_DRAW_SIZE / 2.f));
	mSceneLayers[Background]->attachChild(SceneNode::Ptr(mBoardSprite));
}

void GameHandler::draw() {
	mWindow.draw(mSceneGraph);
}

void GameHandler::update(sf::Time dt) {
	mLogic->updateTime(dt);
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
	if (!needPromotion())
		return;
	if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			sf::Vector2i mouse(event.mouseButton.x, event.mouseButton.y);
			if (mouse.y < 443 || mouse.y > 443 + 85)
				return;
			if (mouse.x < 456 || mouse.x > 456 + 340)
				return;
			std::cout << "Mouse: " << mouse.x << " " << mouse.y << "\n";
			int id = (mouse.x - 456) / 85;
			int type = Piece::Queen + id;
			int color = mLogic->getTurn() ? Piece::Black : Piece::White;
			std::cout << "Promote to: " << type << "\n";
			mPromoteWindow = false;
			mSceneLayers[PopUp]->detachAllChildren();
			handleMove(Move(mPromoteFrom, mPromoteTo, type | color));
		}
	} else if (event.type == sf::Event::MouseMoved) {
		sf::Vector2i mouse(event.mouseMove.x, event.mouseMove.y);
		if (mouse.y < 443 || mouse.y > 443 + 85 || mouse.x < 456 || mouse.x > 456 + 340)
			setCursor(sf::Cursor::Arrow);
		else
			setCursor(sf::Cursor::Hand);
	}
}

void GameHandler::setCursor(sf::Cursor::Type type) {
	sf::Cursor cursor;
	if (cursor.loadFromSystem(type)) {
		mWindow.setMouseCursor(cursor);
	} else
		throw std::runtime_error("Cannot load new cursor");
}

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
	        mPieces[square]->color() == mLogic->getTurn());
}

void GameHandler::dropPiece(int square) {
	mPieces[mOldSquare]->setOpacity(100);
	mSceneLayers[PopUp]->detachChild(*mDragging);
	mDragging = nullptr;
	if (square != mOldSquare) {
		if (Board::validSquare(square)) {
			handleMove(Move(mOldSquare, square));
		} else
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

bool GameHandler::needPromotion() const {
	return mPromoteWindow;
}

void GameHandler::displayPromoteWindow() {
	mPromoteWindow = true;
	auto* window = new SpriteNode(mTextures.get(Textures::PromoteWindow));
	window->setPosition(417.f, 384.f);
	mSceneLayers[PopUp]->attachChild(SceneNode::Ptr(window));

	int left = 86, top = mLogic->getTurn() ? 86 : 0;
	auto pieces =
	    new SpriteNode(mTextures.get(Textures::PieceSet), sf::IntRect(left, top, 340, 85));
	pieces->setPosition(456.f, 443.f);
	mSceneLayers[PopUp]->attachChild(SceneNode::Ptr(pieces));
}

void GameHandler::handleMove(Move move) {
	bool legal = mLogic->isLegalMove(move);

	clearCandidates();

	// if legal move, then highlight new move, make that move, un-highlight old pureMove, and to squares
	int from = move.from(), to = move.to();
	if (legal && from != to) {
		if (mLogic->isLegalPromotion(from, to) && move.promote() == Piece::None) {
			mPromoteFrom = from;
			mPromoteTo = to;
			displayPromoteWindow();
			return;
		}
		assert(mSnapShotIndex == (int)mSaver.size() - 1);
		highlightMove(mLastMove, false);
		mLogic->makeMove(move);
		highlightMove(mLastMove, true);
		saveSnapShot();

		if (mLogic->isFinished()) {
			setCursor(sf::Cursor::Arrow);
			if (mLogic->status() == GameLogic::Checkmate) {
				highlightSquare(mLogic->getKing(mLogic->getTurn()), Debug);
			}
			mSaver.save(mLogic->result());
		}
	} else {
		highlightMove(mLastMove, true);
		//		std::cerr << "Illegal pureMove\n";
	}
}

int GameHandler::getLastMove() const {
	return mLastMove;
}

void GameHandler::highlightLegalMoves(int from) {
	std::vector<Move> moves = mLogic->getMoveList(from);
	for (Move move : moves) {
		//		if (Piece::getType(mLogic->getPiece(move.from())) == Piece::King) {
		//			std::cout << "King: " << move.to() << "\n";
		//		}
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
		return;  // no pureMove
	int oldBox = move & 0x3f, newBox = move >> 6;
	if (flag) {
		highlightSquare(newBox, MovePiece);
		highlightSquare(oldBox, MovePiece);
	} else {
		highlightSquare(newBox, Normal);
		highlightSquare(oldBox, Normal);
	}
}

void GameHandler::addPiece(int square, int piece) {
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
	mSceneLayers[Pieces]->detachChild(*mPieces[square]);
	mPieces[square] = nullptr;
}

void GameHandler::movePiece(int from, int to) {
	if (from != to) {
		mPieces[to] = mPieces[from];
		mPieces[from] = nullptr;
		mPieces[to]->setPosition(getBoxPosition(to), Piece::None);
	}
	mLastMove = (to << 6) | from;
}

void GameHandler::postMove() {
	if (mLogic->isChecked())
		mSounds.play(SoundEffect::Check);
	else if (mLogic->isCaptured())
		mSounds.play(SoundEffect::Capture);
	else if (mLogic->isCastled())
		mSounds.play(SoundEffect::Castling);
	else if (mLogic->isPromoted())
		mSounds.play(SoundEffect::Promotion);
	else
		mSounds.play(SoundEffect::Move);

#ifdef DEBUG_ATTACK
	for (int i = 0; i < 64; ++i) {
		if (mLogic->isAttacked(i))
			highlightSquare(i, Debug);
		else
			highlightSquare(i, Normal);
	}
#endif  // DEBUG_ATTACK
	mOldSquare = -1;
}

void GameHandler::promotePiece(int square, int piece) {
	capturePiece(square);
	addPiece(square, piece);
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

void GameHandler::saveSnapShot() {
	mSaver.capture(*this);
	mSnapShotIndex = (int)mSaver.size() - 1;
}

void GameHandler::loadSnapShot(int index) {
	if (index < 0 || index >= (int)mSaver.size())
		return;
	for (int i = 0; i < GameLogic::BOARD_SIZE; ++i)
		highlightSquare(i, Normal);

	GameSaver::SnapShot snapShot = mSaver.getSnapShot(index);
	highlightMove(snapShot.move, true);
	for (int i = 0; i < GameLogic::BOARD_SIZE; ++i) {
		if (mPieces[i] != nullptr)
			mSceneLayers[Pieces]->detachChild(*mPieces[i]);
		mPieces[i] = nullptr;
		if (snapShot.board.get(i) != 0)
			addPiece(i, snapShot.board.get(i));
	}
	mSounds.play(snapShot.sound);
	if (snapShot.checkMate != -1)
		highlightSquare(snapShot.checkMate, Debug);
	mSnapShotIndex = index;
}

void GameHandler::loadPreviousMove() {
	if (mSnapShotIndex > 0)
		loadSnapShot(mSnapShotIndex - 1);
}

void GameHandler::loadNextMove() {
	if (mSnapShotIndex < (int)mSaver.size() - 1)
		loadSnapShot(mSnapShotIndex + 1);
}

void GameHandler::loadLastMove() {
	loadSnapShot((int)mSaver.size() - 1);
}

void GameHandler::loadFirstMove() {
	loadSnapShot(0);
}

std::vector<std::string> GameHandler::getLatestMoves(int numMoves, int& id) const {
	std::vector<std::string> moves;
	int start = std::max(1, mSnapShotIndex - numMoves * 2 + 1);
	if (start % 2 == 0)
		++start;
	for (int i = start; i <= mSnapShotIndex; ++i)
		moves.emplace_back(mSaver.getSnapShot(i).notation);
	id = start;
	return moves;
}
