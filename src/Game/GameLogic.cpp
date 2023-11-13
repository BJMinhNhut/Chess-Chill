//
// Created by MINH NHUT on 10/25/2023.
//

#include "GameLogic.hpp"
#include "AttackGenerator.hpp"
#include "Piece.hpp"

#include <cassert>
#include <iostream>
#include <sstream>

const int GameLogic::BOARD_SIZE = 64;

GameLogic::GameLogic()
    : mTurn(false), mCastling(0), mEnPassant(-1), mHalfMove(0), mFullMove(0), mBoard() {
	mAttackBoard[0] = mAttackBoard[1] = 0;
}

GameLogic::GameLogic(const GameLogic& other) : mBoard(other.mBoard) {
	mTurn = other.mTurn;
	mCastling = other.mCastling;
	mEnPassant = other.mEnPassant;
	mHalfMove = other.mHalfMove;
	mFullMove = other.mFullMove;
	mAttackBoard[0] = other.mAttackBoard[0];
	mAttackBoard[1] = other.mAttackBoard[1];
}

bool GameLogic::isLegalMove(int from, int to) const {
	if (from == to || !Board::validSquare(from) || !Board::validSquare(to))
		return false;
	int piece = mBoard.get(from);
	int capture = mBoard.get(to);

	//	std::cout << piece << ' ' << mTurn << '\n';
	if (piece == 0 || Piece::getColor(piece) != mTurn)
		return false;
	if (capture != 0 && Piece::getColor(capture) == mTurn)
		return false;
	bool legal = true;
	switch (piece & 7) {
		case Piece::Pawn:
			legal = isLegalPawnMove(from, to);
			break;
		case Piece::Knight:
			legal = isLegalKnightMove(from, to);
			break;
		case Piece::Bishop:
			legal = isLegalBishopMove(from, to);
			break;
		case Piece::Rook:
			legal = isLegalRookMove(from, to);
			break;
		case Piece::Queen:
			legal = isLegalQueenMove(from, to);
			break;
		case Piece::King:
			legal = isLegalKingMove(from, to);
			break;
		default:
			assert(false);
	};

	if (legal) {
		GameLogic copy(*this);
		copy.makeMove(from, to);
		legal = !copy.isKingInCheck(mTurn);
	}

	return legal;
}

bool GameLogic::isKingInCheck() const {
	return isKingInCheck(mTurn);
}

bool GameLogic::getTurn() const {
	return mTurn;
}

void GameLogic::makeMove(int from, int to) {
	//	assert(isLegalMove(from, to));

	bool captured = false;

	if (isEnPassant(from, to)) {
		capturePiece(mEnPassant + (mTurn ? 8 : -8));
		captured = true;
	}

	if (mBoard.get(to) != 0) {
		capturePiece(to);
		captured = true;
	}

	if (isLegalCastling(from, to)) {
		int dir = to - from < 0 ? -1 : 1;
		int rook = dir < 0 ? from - 4 : from + 3;
		movePiece(rook, from + dir);
	}
	if (mBoard.getType(from) == Piece::Pawn)
		updateEnPassant(from, to);
	if (mBoard.getType(from) == Piece::King || mBoard.getType(from) == Piece::Rook)
		updateCastling(from);

	movePiece(from, to);
	// check for promotion
	if (mBoard.getType(to) == Piece::Pawn && (Board::getRank(to) == 0 || Board::getRank(to) == 7)) {
		promotePiece(to, Piece::Queen|(Piece::getColor(mBoard.get(to)) << 3));
	}
	postMove(captured);
}

void GameLogic::addPiece(int piece, int square) {
	std::cout << "add " << piece << ' ' << square << '\n';
	mBoard.set(square, piece);
}

void GameLogic::capturePiece(int square) {
	std::cout << "capture " << square << '\n';
	assert(mBoard.getType(square) != Piece::King);
	mBoard.set(square, 0);
}

void GameLogic::movePiece(int from, int to) {
	mBoard.move(from, to);
}

void GameLogic::postMove(bool captured) {
	if (mTurn)
		mFullMove++;
	updateAttacks(false);
	updateAttacks(true);
	mTurn ^= 1;
}

void GameLogic::promotePiece(int square, int piece) {
	assert(mBoard.getType(square) == Piece::Pawn);
	mBoard.set(square, piece);
}

void GameLogic::loadFEN(const std::string& fen) {
	std::istringstream iss(fen);
	std::string board;
	iss >> board;
	mBoard.clear();
	int rank = 7, file = 0;
	for (char ch : board) {
		if (std::isdigit(ch))
			file += ch - '0';
		else if (ch == '/')
			rank--, file = 0;
		else if (std::isalpha(ch)) {
			addPiece(Piece::getPieceFromChar(ch), Board::getSquareID(rank, file++));
		} else
			break;
	}
	std::string turn;
	iss >> turn;
	mTurn = turn == "b";
	std::string castling;
	iss >> castling;
	mCastling = 0;
	for (char ch : castling) {
		if (ch == 'K')
			mCastling |= 1;
		if (ch == 'Q')
			mCastling |= 2;
		if (ch == 'k')
			mCastling |= 4;
		if (ch == 'q')
			mCastling |= 8;
	}
	std::string enPassant;
	iss >> enPassant;
	if (enPassant == "-")
		mEnPassant = -1;
	else
		mEnPassant = Board::getSquareID(enPassant[1] - '1', enPassant[0] - 'a');
	iss >> mHalfMove >> mFullMove;
	updateAttacks(false);
	updateAttacks(true);
}

void GameLogic::updateEnPassant(int from, int to) {
	// check and update mEnPassant when pawn move 2 square
	int piece = mBoard.get(from);
	int diff = to - from;
	int absDiff = diff < 0 ? -diff : diff;
	int dir = Piece::getColor(piece) ? -8 : 8;
	if (Piece::getType(piece) == Piece::Pawn && absDiff == 16) {
		mEnPassant = from + dir;
	} else
		mEnPassant = -1;
	std::cout << from << ' ' << to << ' ' << mEnPassant << '\n';
}

void GameLogic::updateCastling(int from) {
	int piece = mBoard.get(from);
	int color = Piece::getColor(piece);
	if (Piece::getType(piece) == Piece::King) {
		mCastling &= ~(color ? 12 : 3);
	} else if (Piece::getType(piece) == Piece::Rook) {
		if (from == (color ? 63 : 7))
			mCastling &= ~(color ? 4 : 1);
		else if (from == (color ? 56 : 0))
			mCastling &= ~(color ? 8 : 2);
	}
}

void GameLogic::updateAttacks(bool turn) {
	mAttackBoard[turn] = 0;
	for (int i = 0; i < 64; i++) {
		int piece = mBoard.get(i);
		if (piece == 0 || Piece::getColor(piece) != turn)
			continue;
		switch (Piece::getType(piece)) {
			case Piece::Pawn:
				mAttackBoard[turn] |= AttackGenerator::generatePawnAttacks(i, turn);
				break;
			case Piece::Knight:
				mAttackBoard[turn] |= AttackGenerator::generateKnightAttacks(i);
				break;
			case Piece::Bishop:
				mAttackBoard[turn] |= AttackGenerator::generateBishopAttacks(i, mBoard);
				break;
			case Piece::Rook:
				mAttackBoard[turn] |= AttackGenerator::generateRookAttacks(i, mBoard);
				break;
			case Piece::Queen:
				mAttackBoard[turn] |= AttackGenerator::generateQueenAttacks(i, mBoard);
				break;
			case Piece::King:
				mAttackBoard[turn] |= AttackGenerator::generateKingAttacks(i);
				break;
			default:
				assert(false);
		}
	}
}

bool GameLogic::isEnPassant(int from, int to) const {
	int piece = mBoard.get(from);
	int diff = to - from;
	int absDiff = diff < 0 ? -diff : diff;
	int dir = Piece::getColor(piece) ? -8 : 8;
	int targetRow = to >> 3;
	int startRow = from >> 3;
	if (mEnPassant != to)
		return false;
	if (diff * dir < 0)
		return false;
	if (targetRow != startRow + (dir >> 3))
		return false;
	if (Piece::getType(piece) != Piece::Pawn)
		return false;
	//	std::cout <<"yay " << mEnPassant << ' ' << from << ' ' << to << ' ' << to+dir << '\n';
	return (absDiff == 7 || absDiff == 9);
}

bool GameLogic::isLegalPawnMove(int from, int to) const {
	int piece = mBoard.get(from);
	int capture = mBoard.get(to);
	int color = Piece::getColor(piece);
	int diff = to - from;
	int absDiff = diff < 0 ? -diff : diff;
	int dir = color ? -8 : 8;
	int start = color ? 48 : 8;
	int end = color ? 55 : 15;
	int targetRow = to >> 3;
	int startRow = from >> 3;

	if (diff * dir <= 0)
		return false;
	if (absDiff == 8)
		return capture == 0;
	if (absDiff == 16 && from >= start && from <= end)
		return capture == 0 && mBoard.get(from + dir) == 0;
	if (targetRow != startRow + (dir >> 3))
		return false;
	if (absDiff == 7 && capture != 0)
		return Piece::getColor(capture) != color;
	if (absDiff == 9 && capture != 0)
		return Piece::getColor(capture) != color;

	// check for enpassant
	if (isEnPassant(from, to))
		return true;

	return false;
}

bool GameLogic::isLegalKnightMove(int from, int to) {
	int r1 = from >> 3, c1 = from & 7;
	int r2 = to >> 3, c2 = to & 7;
	int dr = r2 - r1, dc = c2 - c1;
	int absDr = dr < 0 ? -dr : dr;
	int absDc = dc < 0 ? -dc : dc;
	if (absDr == 2 && absDc == 1)
		return true;
	if (absDr == 1 && absDc == 2)
		return true;
	return false;
}

bool GameLogic::isLegalBishopMove(int from, int to) const {
	// check for diagonal
	int diffRank = (to >> 3) - (from >> 3);
	int diffFile = (to & 7) - (from & 7);
	int absDiffRank = diffRank < 0 ? -diffRank : diffRank;
	int absDiffFile = diffFile < 0 ? -diffFile : diffFile;
	if (absDiffRank != absDiffFile)
		return false;

	// check for blocking
	int dirRow = diffRank < 0 ? -1 : 1;
	int dirCol = diffFile < 0 ? -1 : 1;
	int rank = from >> 3, file = from & 7;
	for (int i = 1; i < absDiffRank; i++) {
		rank += dirRow;
		file += dirCol;
		if (mBoard.get(rank, file) != 0)
			return false;
	}

	return true;
}

bool GameLogic::isLegalRookMove(int from, int to) const {
	// check for straight
	int diffRank = (to >> 3) - (from >> 3);
	int diffFile = (to & 7) - (from & 7);
	int absDiffRank = diffRank < 0 ? -diffRank : diffRank;
	int absDiffFile = diffFile < 0 ? -diffFile : diffFile;
	if (absDiffRank != 0 && absDiffFile != 0)
		return false;

	// check for block
	int dirRow = diffRank != 0 ? diffRank / absDiffRank : 0;
	int dirCol = diffFile != 0 ? diffFile / absDiffFile : 0;
	int rank = from >> 3, file = from & 7;
	for (int i = 1; i < absDiffRank + absDiffFile; i++) {
		rank += dirRow;
		file += dirCol;
		if (mBoard.get(rank, file) != 0)
			return false;
	}
	return true;
}

bool GameLogic::isLegalQueenMove(int from, int to) const {
	if (isLegalBishopMove(from, to))
		return true;
	if (isLegalRookMove(from, to))
		return true;
	return false;
}

bool GameLogic::isLegalKingMove(int from, int to) const {
	int diffRank = (to >> 3) - (from >> 3);
	int diffFile = (to & 7) - (from & 7);
	int absDiffRank = diffRank < 0 ? -diffRank : diffRank;
	int absDiffFile = diffFile < 0 ? -diffFile : diffFile;
	if (absDiffRank <= 1 && absDiffFile <= 1 && !isAttacked(to))
		return true;

	// check whether move is castling, and castling is legal, using mCastling
	return isLegalCastling(from, to);
}

bool GameLogic::isLegalCastling(int from, int to) const {
	int diffRank = (to >> 3) - (from >> 3);
	int diffFile = (to & 7) - (from & 7);
	int absDiffRank = diffRank < 0 ? -diffRank : diffRank;
	int absDiffFile = diffFile < 0 ? -diffFile : diffFile;
	if (absDiffFile == 2 && absDiffRank == 0) {
		bool color = mTurn;
		int dir = diffFile < 0 ? -1 : 1;
		if (Board::getFile(from) != 4)
			return false;
		if (Board::getRank(from) != (color ? 7 : 0))
			return false;
		if (isAttacked(from) || isAttacked(from + dir) || isAttacked(from + 2 * dir))
			return false;
		if (mBoard.get(from + dir) != 0 || mBoard.get(from + 2 * dir) != 0)  // check for blocking
			return false;
		if (dir == 1)
			return mCastling & (color ? 4 : 1);
		else
			return mCastling & (color ? 8 : 2);
	}
	return false;
}

bool GameLogic::isKingInCheck(bool turn) const {
	int king = mBoard.getKing(turn);
	if (king == -1) {
		std::cout << "king not found\n";
		return false;
	}
	return isAttacked(king, turn);
}

bool GameLogic::isAttacked(int square) const {
	return mAttackBoard[(!mTurn)] & (1LL << square);
}

bool GameLogic::isAttacked(int square, bool turn) const {
	return mAttackBoard[!turn] & (1LL << square);
}
