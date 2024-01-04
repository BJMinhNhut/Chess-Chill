//
// Created by MINH NHUT on 12/28/2023.
//

#ifndef CHESS_CHILL_HISTORYSTATE_HPP
#define CHESS_CHILL_HISTORYSTATE_HPP

#include "GUI/Container.hpp"
#include "GUI/Label.hpp"
#include "Game/GameSaver.hpp"
#include "Template/State.hpp"

class HistoryState : public State {
   public:
	HistoryState(StateStack& stack, Context context);

	void draw() override;
	bool update(sf::Time dt) override;
	bool handleEvent(const sf::Event& event) override;

   private:
	void loadBasicGUI();
	void loadHistoryList();

	void loadPanel(int id, int pathID, const std::string& path);

	void loadCurrentPage();
	void nextPage();
	void previousPage();

	int getNumPages() const;

   private:
	static const int PAGE_MAX;
	static const float PANEL_INDENT;

   private:
	GUI::Container mGUIContainer, mPageContainer;
	GUI::Label::Ptr mPageLabel;
	int mPage;
};

#endif  //CHESS_CHILL_HISTORYSTATE_HPP
