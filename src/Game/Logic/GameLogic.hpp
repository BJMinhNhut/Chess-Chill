//
// Created by MINH NHUT on 10/25/2023.
//

#ifndef CHESS_GAMELOGIC_HPP
#define CHESS_GAMELOGIC_HPP

#include "AttackBoard.hpp"
#include "Game/Board.hpp"
#include "Game/Clock.hpp"
#include "Game/Move.hpp"
#include "Template/GameOptions.hpp"

#include "SFML/System/Time.hpp"

#include <map>
#include <string>
#include <vector>

class GameHandler;

class GameLogic {
   public:
	typedef std::unique_ptr<GameLogic> Ptr;
	static const int BOARD_SIZE;
	enum Status {
		OnGoing,
		Checkmate,
		Stalemate,
		InsufficientMaterial,
		ThreefoldRepetition,
		FiftyMoveRule,
		TopOfTheHill,
		Resign,
		Timeout,
	};

	enum Result {
		WhiteWin,
		BlackWin,
		Draw,
	};

	enum MoveStatus {
		Normal = 0,
		Check = 1,
		Capture = 2,
		Castling = 4,
		Promotion = 8,
	};

   public:
	explicit GameLogic(const std::string &fen, GameHandler* handler);
	GameLogic(const GameLogic& other, GameHandler* handler);
	GameLogic(const GameLogic& other) = delete;
	virtual ~GameLogic();

	[[nodiscard]] virtual GameLogic* clone() const = 0;
	[[nodiscard]] virtual GameOptions::Type getType() const = 0;

	[[nodiscard]] bool isFinished() const;
	[[nodiscard]] bool isChecked() const;
	[[nodiscard]] bool isCaptured() const;
	[[nodiscard]] bool isCastled() const;
	[[nodiscard]] bool isPromoted() const;

	[[nodiscard]] Status status() const;
	[[nodiscard]] Result result() const;

	[[nodiscard]] std::string getWinner() const;
	[[nodiscard]] float getRemainingTime(bool turn) const;

	[[nodiscard]] std::vector<Move> getMoveList(int from) const;
	[[nodiscard]] std::vector<Move> getLegalMoves() const;


	[[nodiscard]] Board getBoard() const;
	[[nodiscard]] bool getTurn() const;
	[[nodiscard]] int getHalfMove() const;

	[[nodiscard]] int getEvaluation() const;

	[[nodiscard]] int getPiece(int square) const;
	[[nodiscard]] int getKing(int color) const;
	[[nodiscard]] int getLastMovePiece() const;
	[[nodiscard]] int getCastling() const;

	void attachHandler(GameHandler* handler);
	void makeMove(Move move);
	void setClock(sf::Time time, sf::Time bonus = sf::seconds(0));

	void updateTime(sf::Time dt);

	[[nodiscard]] bool isLegalMove(Move move) const;
	[[nodiscard]] bool isAttacked(int square) const;
	[[nodiscard]] bool isLegalPromotion(int from, int to) const;

   protected:
	[[nodiscard]] bool hasLegalMove() const;
	[[nodiscard]] bool isInsufficientMaterial();
	[[nodiscard]] bool isThreefoldRepetition();
	[[nodiscard]] bool isFiftyMoveRule();

	void saveHistory();

   private:
	void pureMove(Move move);
	void addPiece(int square, int piece);
	void promotePiece(int square, int piece);
	void capturePiece(int square);
	void movePiece(int from, int to);
	void postMove();

	[[nodiscard]] bool isPseudoLegalMove(Move move) const;
	[[nodiscard]] bool isLegalPawnMove(int from, int to) const;
	[[nodiscard]] bool isEnPassant(int from, int to) const;
	static bool isLegalKnightMove(int from, int to);
	[[nodiscard]] bool isLegalBishopMove(int from, int to) const;
	[[nodiscard]] bool isLegalRookMove(int from, int to) const;
	[[nodiscard]] bool isLegalQueenMove(int from, int to) const;
	[[nodiscard]] bool isLegalKingMove(int from, int to) const;

	virtual void updateStatus() = 0;
	[[nodiscard]] virtual bool isLegalCastling(int from, int to) const = 0;
	[[nodiscard]] virtual int getRook(bool color, bool side) const = 0;
	[[nodiscard]] virtual bool getRookSide(int square) const = 0;

   protected:
	Board mBoard;
	AttackBoard mAttacks;
	Status mStatus;

   private:
	GameHandler* mHandler;
	int8_t mLastMovePiece, mLastMove;
	std::map<std::string, int8_t> mFENs;
	Clock mClock;
};

#endif  //CHESS_GAMELOGIC_HPP
