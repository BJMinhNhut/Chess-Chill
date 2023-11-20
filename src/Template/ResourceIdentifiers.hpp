//
// Created by MINH NHUT on 6/8/2023.
//

#ifndef NATUREOFCODE_RESOURCEIDENTIFIERS_HPP
#define NATUREOFCODE_RESOURCEIDENTIFIERS_HPP

namespace sf {
class Texture;
class Font;
class SoundBuffer;
}  // namespace sf

namespace Textures {
enum ID {
	Background,
	Title,
	TitleBar,
	AboutPanel,
	SettingsPanel,
	EndGamePanel,
	GameOptionsPanel,
	OptionBox,

	ChessStandard,
	Chess960,

	BackButtonNormal,
	BackButtonSelected,
	HomeButtonNormal,
	HomeButtonSelected,
	MenuButtonNormal,
	MenuButtonSelected,
	ForwardButtonNormal,
	ForwardButtonSelected,
	RotateButtonNormal,
	RotateButtonSelected,
	PlayButtonNormal,
	PlayButtonSelected,

	Board,
	PieceSet,
	GameGUI,
	BoardIndexWhite,
	BoardIndexBlack,

	NumTextures,
};
}

namespace Fonts {
enum ID {
	Main,
	Bold,
	Mono,
};
}

namespace SoundEffect {
enum ID {
	Move,
	Capture,
	Check,
	Castling,
	Promotion,
	OutOfBound,
	EndGame,
	StartGame,
};
}

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffect::ID> SoundBufferHolder;

#endif  //NATUREOFCODE_RESOURCEIDENTIFIERS_HPP
