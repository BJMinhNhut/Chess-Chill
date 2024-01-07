//
// Created by MINH NHUT on 1/5/2024.
//

#ifndef CHESS_CHILL_PUZZLEMENUSTATE_HPP
#define CHESS_CHILL_PUZZLEMENUSTATE_HPP

#include "GUI/Button.hpp"
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

	void loadPanel(int order, int puzzleID);

	void loadCurrentPage();
	void nextPage();
	void previousPage();

	int getNumPages() const;
	static GUI::Button::Type getButtonType(Puzzle::Status status);

   private:
	static const int PAGE_MAX;
	static const float PANEL_INDENT_X, PANEL_INDENT_Y;

   private:
	GUI::Container mGUIContainer, mPageContainer;
	GUI::Label::Ptr mPageLabel;
	int mPage;
};

#endif  //CHESS_CHILL_PUZZLEMENUSTATE_HPP
