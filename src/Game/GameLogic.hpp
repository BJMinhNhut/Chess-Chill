//
// Created by MINH NHUT on 10/25/2023.
//

#ifndef CHESS_GAMELOGIC_HPP
#define CHESS_GAMELOGIC_HPP

#include "AttackBoard.hpp"
#include "Board.hpp"
#include "Clock.hpp"
#include "Move.hpp"

#include <SFML/System/Time.hpp>

#include <string>
#include <vector>
#include <map>

class GameHandler;

class GameLogic {

   public:
	static const int BOARD_SIZE;
	enum Status {
		OnGoing,
		Checkmate,
		Stalemate,
		InsufficientMaterial,
		ThreefoldRepetition,
		Resign,
		Timeout,
	};

	enum MoveStatus {
		Normal = 0,
		Check = 1,
		Capture = 2,
		Castling = 4,
		Promotion = 8,
	};

   public:
	explicit GameLogic(const std::string& fen, GameHandler* handler);
	GameLogic(const GameLogic& other, GameHandler* handler);
	GameLogic(const GameLogic& other) = delete;

	[[nodiscard]] bool isFinished() const;
	[[nodiscard]] bool isChecked() const;
	[[nodiscard]] bool isCaptured() const;
	[[nodiscard]] bool isCastled() const;
	[[nodiscard]] bool isPromoted() const;

	[[nodiscard]] Status status() const;

	[[nodiscard]] std::string getWinner() const;
	[[nodiscard]] float getRemainingTime(bool turn) const;

	[[nodiscard]] std::vector<Move> getMoveList(int from) const;
	[[nodiscard]] std::vector<Move> getLegalMoves() const;

	[[nodiscard]] bool getTurn() const;

	[[nodiscard]] int getEvaluation() const;

	[[nodiscard]] int getPiece(int square) const;
	[[nodiscard]] int getKing(int color) const;
	[[nodiscard]] int getLastMovePiece() const;
	[[nodiscard]] int getCastling() const;

	void makeMove(Move move);
	void setClock(sf::Time time, sf::Time bonus = sf::seconds(0));

	void updateTime(sf::Time dt);

	[[nodiscard]] bool isLegalMove(Move move) const;
	[[nodiscard]] bool isAttacked(int square) const;
	[[nodiscard]] bool isLegalPromotion(int from, int to) const;

   private:

	void pureMove(Move move);
	void promotePiece(int square, int piece);
	void capturePiece(int square);
	void movePiece(int from, int to);
	void postMove();

	void updateStatus();

	[[nodiscard]] bool isInsufficientMaterial();
	[[nodiscard]] bool isThreefoldRepetition();

	[[nodiscard]] bool isPseudoLegalMove(Move move) const;

	[[nodiscard]] bool hasLegalMove() const;
	[[nodiscard]] bool isLegalPawnMove(int from, int to) const;
	[[nodiscard]] bool isEnPassant(int from, int to) const;
	static bool isLegalKnightMove(int from, int to);
	[[nodiscard]] bool isLegalBishopMove(int from, int to) const;
	[[nodiscard]] bool isLegalRookMove(int from, int to) const;
	[[nodiscard]] bool isLegalQueenMove(int from, int to) const;
	[[nodiscard]] bool isLegalKingMove(int from, int to) const;
	[[nodiscard]] bool isLegalCastling(int from, int to) const;

   private:
	GameHandler* mHandler;
	int8_t mLastMovePiece, mLastMove;
	Status mStatus;
	std::map<std::string, int8_t> mFENs;
	Clock mClock;
	Board mBoard;
	AttackBoard mAttacks;
};

#endif  //CHESS_GAMELOGIC_HPP
