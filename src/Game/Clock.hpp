//
// Created by MINH NHUT on 11/19/2023.
//

#ifndef CHESS_CHILL_CLOCK_HPP
#define CHESS_CHILL_CLOCK_HPP

#include <SFML/System/Time.hpp>

class Clock {
   public:
	Clock();

	void update(sf::Time dt);
	void set(sf::Time time);

	[[nodiscard]] bool isTimeOut() const;
	[[nodiscard]] float get() const;

   private:
	bool mActive;
	sf::Time mTime;
};

#endif  //CHESS_CHILL_CLOCK_HPP
