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
		int8_t checkMate;
		int move;
		std::string notation;
		SoundEffect::ID sound;
		Board board;

		SnapShot(const Board& board, int lastMove, std::string notation, SoundEffect::ID sound,
		         int8_t checkMate);
	};

   public:
	explicit GameSaver(GameOptions options);
	~GameSaver() = default;

	void capture(const GameHandler& gameHandler);
	void load(const std::string& path);
	void save(GameLogic::Result result);

	[[nodiscard]] SnapShot get(int index) const;
	[[nodiscard]] unsigned int size() const;

   private:
	static std::string getFileName(time_t time);

   private:
	GameOptions mOptions;
	time_t mTime;
	GameLogic::Result mResult;
	std::vector<SnapShot> mSnapShots;
};

#endif  //CHESS_CHILL_GAMESAVER_HPP
