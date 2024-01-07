//
// Created by MINH NHUT on 6/11/2023.
//

#ifndef NATUREOFCODE_STATE_HPP
#define NATUREOFCODE_STATE_HPP

#include "ResourceIdentifiers.hpp"
#include "SoundPlayer.hpp"
#include "StateIdentifiers.hpp"
#include "Settings.hpp"
#include "GameOptions.hpp"
#include "OldGames.hpp"
#include "Game/PuzzleList.hpp"

#include "SFML/System/Time.hpp"
#include "SFML/Window/Event.hpp"

namespace sf {
class RenderWindow;
}

class StateStack;
class SoundPlayer;

class State {
   public:
	typedef std::unique_ptr<State> Ptr;

	struct Context {
		enum Mode {
			None,
			Normal,
			Review,
			Puzzles,
		};
		Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts,
		        SoundPlayer& sounds, Settings& settings, GameOptions& options, OldGames& oldGames, PuzzleList& puzzles, Mode &mode);

		sf::RenderWindow* window;
		TextureHolder* textures;
		FontHolder* fonts;
		SoundPlayer* sounds;
		Settings* settings;
		GameOptions* options;
		OldGames* oldGames;
		PuzzleList* puzzles;
		Mode *mode;
	};

   public:
	State(StateStack& stack, Context context);

	virtual ~State();

	virtual void draw() = 0;

	virtual bool update(sf::Time dt) = 0;

	virtual bool handleEvent(const sf::Event& event) = 0;

   protected:
	void requestStackPush(States::ID stateID);

	void requestStackPop();

	void requestStackClear();

	[[nodiscard]] Context getContext() const;

   private:
	StateStack* mStack;
	Context mContext;
};

#endif  //NATUREOFCODE_STATE_HPP
