//
// Created by MINH NHUT on 11/19/2023.
//

#ifndef CHESS_CHILL_CLOCK_HPP
#define CHESS_CHILL_CLOCK_HPP

#include <SFML/System/Time.hpp>

class Clock {
   public:
	Clock();

	void update(bool turn, sf::Time dt);
	void set(sf::Time time, sf::Time increment = sf::seconds(0));

	void increment(bool turn);

	[[nodiscard]] bool isTimeOut(bool turn) const;
	[[nodiscard]] float get(bool turn) const;

   private:
	bool mActive;
	sf::Time mTime[2], mIncrement;
};

#endif  //CHESS_CHILL_CLOCK_HPP
