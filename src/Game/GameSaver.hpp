//
// Created by MINH NHUT on 12/25/2023.
//

#ifndef CHESS_CHILL_GAMESAVER_HPP
#define CHESS_CHILL_GAMESAVER_HPP

#include "Board.hpp"
#include "Logic/GameLogic.hpp"
#include "Template/GameOptions.hpp"
#include "Template/SoundPlayer.hpp"

#include <string>

class GameHandler;

class GameSaver {
   public:
	static const std::string SAVE_PATH;

	struct SnapShot {
		Board board;
		int move;
		SoundEffect::ID sound;
		int8_t checkMate;
		char notation[10];

		SnapShot();
		SnapShot(const Board& board, int lastMove, const char notation[10], SoundEffect::ID sound,
		         int8_t checkMate);
	};

   public:
	explicit GameSaver(GameOptions options);
	explicit GameSaver(const std::string& path);
	~GameSaver() = default;

	void capture(const GameHandler& gameHandler);
	void load(const std::string& path);
	void save(GameLogic::Result result);

	[[nodiscard]] SnapShot getSnapShot(int index) const;
	[[nodiscard]] GameOptions getOptions() const;
	[[nodiscard]] std::string getResult() const;
	[[nodiscard]] std::string getDate() const;
	[[nodiscard]] unsigned int size() const;

   private:
	static std::string getFileName(time_t time);

   private:
	GameOptions mOptions;
	time_t mTime{};
	GameLogic::Result mResult;
	std::vector<SnapShot> mSnapShots;
};

#endif  //CHESS_CHILL_GAMESAVER_HPP
