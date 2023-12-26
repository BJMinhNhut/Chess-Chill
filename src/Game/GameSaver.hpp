//
// Created by MINH NHUT on 12/25/2023.
//

#ifndef CHESS_CHILL_GAMESAVER_HPP
#define CHESS_CHILL_GAMESAVER_HPP

#include "GameHandler.hpp"

#include <string>

class GameSaver {
   public:
	static const std::string SAVE_PATH;
	static void saveGame(const std::vector<GameHandler::SnapShot> &snapShots);
   private:
	static std::string getNewFileName();
};

#endif  //CHESS_CHILL_GAMESAVER_HPP
