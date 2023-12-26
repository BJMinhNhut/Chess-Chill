//
// Created by MINH NHUT on 12/25/2023.
//

#ifndef CHESS_CHILL_GAMESAVER_HPP
#define CHESS_CHILL_GAMESAVER_HPP

#include "Template/SoundPlayer.hpp"
#include "Board.hpp"

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

		SnapShot(const Board& board, int lastMove, std::string notation, SoundEffect::ID sound, int8_t checkMate);
	};

   public:
	GameSaver() = default;
	void capture(const GameHandler& gameHandler);
	void save();

	[[nodiscard]] SnapShot get(int index) const;
	[[nodiscard]] unsigned int size() const;

   private:
	static std::string getNewFileName();

   private:
	std::vector<SnapShot> mSnapShots;
};

#endif  //CHESS_CHILL_GAMESAVER_HPP
