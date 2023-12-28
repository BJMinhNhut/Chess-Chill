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

   private:
	static const std::vector<std::string> MODE_NAMES;
	static const std::vector<std::pair<std::string, int>> TYPE_NAMES;
	static const std::vector<std::pair<int, int>> TIMES;

   public:
	GameOptions();

	[[nodiscard]] std::string getStringTime() const;
	[[nodiscard]] std::string getStringMode() const;
	[[nodiscard]] std::string getStringType() const;
	[[nodiscard]] int getTypeDescriptionID() const;

	[[nodiscard]] Mode getMode() const;
	[[nodiscard]] Type getType() const;

	// as seconds
	int getTime() const;
	int getIncrement() const;

	void nextMode();
	void nextType();
	void nextTime();

	void prevMode();
	void prevType();
	void prevTime();

   private:
	Mode mMode;
	Type mType;
	int mTimeID;
};

#endif  //CHESS_CHILL_GAMEOPTIONS_HPP
