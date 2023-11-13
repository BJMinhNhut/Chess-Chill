//
// Created by MINH NHUT on 11/13/2023.
//

#ifndef CHESS_BOARD_HPP
#define CHESS_BOARD_HPP

class Board {
   public:
	Board();
	Board(const Board &other);

	static int getRank(int square);
	static int getFile(int square);
	static int getSquareID(int rank, int file);
	static bool validSquare(int square);

	[[nodiscard]] int get(int square) const;
	[[nodiscard]] int get(int rank, int file) const;

	// if empty then return -1
	[[nodiscard]] bool getColor(int square) const;
	[[nodiscard]] bool getColor(int rank, int file) const;
	[[nodiscard]] int getType(int square) const;
	[[nodiscard]] int getType(int rank, int file) const;

	[[nodiscard]] int getKing(bool turn) const; // if not found return -1

	void clear();
	void set(int square, int piece);
	void set(int rank, int file, int piece);
	void move(int from, int to);

   private:
	int mBoard[64]{};
};

#endif  //CHESS_BOARD_HPP
