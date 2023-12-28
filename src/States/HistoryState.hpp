//
// Created by MINH NHUT on 12/28/2023.
//

#ifndef CHESS_CHILL_HISTORYSTATE_HPP
#define CHESS_CHILL_HISTORYSTATE_HPP

#include "GUI/Container.hpp"
#include "Template/State.hpp"
#include "Game/GameSaver.hpp"

class HistoryState : public State {
   public:
	HistoryState(StateStack& stack, Context context);

	void draw() override;
	bool update(sf::Time dt) override;
	bool handleEvent(const sf::Event& event) override;

   private:
	void loadBasicGUI();
	void loadHistoryList();
	void loadHistoryGUI();

	void nextPage();
	void previousPage();

   private:
	static const int PAGE_MAX;

   private:
	GUI::Container mGUIContainer;
	std::vector<std::string> mList;
	int mPage;
};

#endif  //CHESS_CHILL_HISTORYSTATE_HPP
