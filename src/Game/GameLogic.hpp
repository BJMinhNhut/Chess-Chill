//
// Created by MINH NHUT on 10/25/2023.
//

#ifndef CHESS_GAMELOGIC_HPP
#define CHESS_GAMELOGIC_HPP

#include "Board.hpp"
#include "AttackBoard.hpp"
#include "Clock.hpp"

#include <SFML/System/Time.hpp>

#include <string>
#include <vector>

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
		Normal,
		Check,
		Capture,
		Castling,
		Promotion,
	};

   public:
	explicit GameLogic(const std::string &fen);
	GameLogic(const GameLogic &other);

	[[nodiscard]] bool isFinished() const;
	[[nodiscard]] bool isChecked() const;
	[[nodiscard]] bool isCaptured() const;
	[[nodiscard]] Status status() const;
	[[nodiscard]] std::string getWinner() const;
	[[nodiscard]] float getRemainingTime(bool turn) const;
	[[nodiscard]] std::vector<int> getMoveList(int from) const;
	[[nodiscard]] std::vector<int> getLegalMoves() const;
	[[nodiscard]] bool getTurn() const;
	[[nodiscard]] int getEvaluation() const;
	[[nodiscard]] int getPiece(int square) const;
	[[nodiscard]] int getKing(int color) const;
	[[nodiscard]] int getLastMovePiece() const;
	[[nodiscard]] int getSecondLastMovePiece() const;
	[[nodiscard]] int getCastling() const;

	void makeMove(int from, int to);
	void setClock(bool turn, sf::Time time, sf::Time bonus = sf::seconds(0));

   protected:
	virtual void capturePiece(int square);
	virtual void movePiece(int from, int to);
	virtual void postMove();
	virtual void promotePiece(int square, int piece);

	virtual void updateTime(sf::Time dt);

	[[nodiscard]] MoveStatus lastMoveStatus() const;

	[[nodiscard]] bool isLegalMove(int from, int to) const;
	[[nodiscard]] bool isAttacked(int square) const;

   private:
	void move(int from, int to);
	void updateStatus();

	[[nodiscard]] bool isInsufficientMaterial();
	[[nodiscard]] bool isThreefoldRepetition();

	[[nodiscard]] bool isPseudoLegalMove(int from, int to) const;

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
	Board mBoard;
	AttackBoard mAttacks;

	Status mStatus;
	MoveStatus mLastMove;
	int mLastMovePiece, mSecondLastMovePiece;

	std::vector<std::string> mHistory;

	Clock mClock[2];
};

#endif  //CHESS_GAMELOGIC_HPP
