//
// Created by MINH NHUT on 11/19/2023.
//

#ifndef CHESS_CHILL_GAMEOPTIONS_HPP
#define CHESS_CHILL_GAMEOPTIONS_HPP

#include <string>
#include <vector>

class GameOptions {
   public:
	enum Mode {
		PvP,
		PvAI,
		AIvAI,
		NumModes,
	};


	enum Type {
		Standard,
		Chess960,
		KingOfTheHill,
		NumTypes,
	};

	enum Side {
		White,
		Black,
		Random,
		NumSides,
	};

   private:
	static const std::vector<std::string> MODE_NAMES, SIDE_NAMES;
	static const std::vector<std::pair<std::string, int>> TYPE_NAMES;
	static const std::vector<std::pair<int, int>> TIMES;

   public:
	GameOptions();

	[[nodiscard]] std::string getStringTime() const;
	[[nodiscard]] std::string getStringMode() const;
	[[nodiscard]] std::string getStringType() const;
	[[nodiscard]] std::string getStringSide() const;
	[[nodiscard]] int getTypeDescriptionID() const;

	[[nodiscard]] Mode getMode() const;
	[[nodiscard]] Type getType() const;
	[[nodiscard]] Side getSide() const;

	// as seconds
	int getTime() const;
	int getIncrement() const;

	void nextMode();
	void nextSide();
	void nextType();
	void nextTime();

	void prevMode();
	void prevSide();
	void prevType();
	void prevTime();

   private:
	Mode mMode;
	Type mType;
	Side mSide;
	int mTimeID;
};

#endif  //CHESS_CHILL_GAMEOPTIONS_HPP
