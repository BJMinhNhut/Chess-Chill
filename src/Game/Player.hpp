//
// Created by MINH NHUT on 11/20/2023.
//

#ifndef CHESS_CHILL_PLAYER_HPP
#define CHESS_CHILL_PLAYER_HPP

#include "GameHandler.hpp"

/* Abstract class for player, including AI and Human */
class Player {
   public:
	typedef std::unique_ptr<Player> Ptr;
	enum Color {
		White,
		Black,
	};

   public:
	Player(GameHandler& gameHandler, int color);
	virtual ~Player() = default;

	virtual void update(sf::Time dt) = 0;
	virtual void handleEvent(const sf::Event& event) = 0;

	[[nodiscard]] int getColor() const;
	[[nodiscard]] std::string getName() const;

   protected:
	void setName(const std::string& name);
	GameHandler& mGameHandler;

   private:
	std::string mName;
	int mColor;
};

#endif  //CHESS_CHILL_PLAYER_HPP
