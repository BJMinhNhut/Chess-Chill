//
// Created by MINH NHUT on 7/4/2023.
//

#ifndef DATAVISUALIZATION2_SETTINGS_HPP
#define DATAVISUALIZATION2_SETTINGS_HPP

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>
#include <vector>

class Settings {
   public:
	static const std::string SETTINGS_FILE;

	static const std::vector<std::string> PIECESET_NAMES;
	static const std::string PIECESET_PATH;

	static const std::vector<std::string> BOARD_NAMES;
	static const std::string BOARD_PATH;

   public:
	Settings();

	void save();

	void nextPieceSet();
	void previousPieceSet();

	void nextBoard();
	void previousBoard();

	[[nodiscard]] std::string getPieceSetPath() const;
	[[nodiscard]] std::string getBoardPath() const;

   private:
	void print();
	void load();

   public:
	bool operator==(const Settings& settings) const;
	bool operator!=(const Settings& settings) const;

   private:
	int mPieceSetID;
	int mBoardID;

};

#endif  // SETTINGS_HPP

#endif  //DATAVISUALIZATION2_SETTINGS_HPP
