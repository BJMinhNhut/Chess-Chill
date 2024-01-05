//
// Created by MINH NHUT on 1/5/2024.
//

#ifndef CHESS_CHILL_PUZZLEMENUSTATE_HPP
#define CHESS_CHILL_PUZZLEMENUSTATE_HPP

#include "GUI/Container.hpp"
#include "GUI/Label.hpp"
#include "Game/Puzzle.hpp"
#include "Template/State.hpp"

class PuzzleMenuState : public State {
   public:
	PuzzleMenuState(StateStack& stack, Context context);

	void draw() override;
	bool update(sf::Time dt) override;
	bool handleEvent(const sf::Event& event) override;

   private:
	void loadBasicGUI();

	//	void loadPanel(int id, int pathID, const std::string& path);

	void loadCurrentPage();
	void nextPage();
	void previousPage();

	int getNumPages() const;

   private:
	static const int PAGE_MAX;
	static const float PANEL_INDENT_X, PANEL_INDENT_Y;
	static const std::string PATH;

   private:
	GUI::Container mGUIContainer, mPageContainer;
	std::vector<Puzzle> mList;
	GUI::Label::Ptr mPageLabel;
	int mPage;
};

#endif  //CHESS_CHILL_PUZZLEMENUSTATE_HPP
