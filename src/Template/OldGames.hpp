//
// Created by MINH NHUT on 12/28/2023.
//

#ifndef CHESS_CHILL_OLDGAMES_HPP
#define CHESS_CHILL_OLDGAMES_HPP

#include <string>
#include <vector>

class OldGames {
   public:
	OldGames();
	~OldGames();

	void load();

	void setIndex(int index);
	void resetIndex();

	[[nodiscard]] size_t getSize() const;
	[[nodiscard]] bool pathChosen() const;
	[[nodiscard]] std::string getPath() const;
	[[nodiscard]] std::string getPathByID(int id) const;

   private:
	int mIndex;
	std::vector<std::string> mPaths;
};

#endif  //CHESS_CHILL_OLDGAMES_HPP
