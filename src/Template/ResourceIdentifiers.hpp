//
// Created by MINH NHUT on 6/8/2023.
//

#ifndef NATUREOFCODE_RESOURCEIDENTIFIERS_HPP
#define NATUREOFCODE_RESOURCEIDENTIFIERS_HPP

namespace sf {
class Texture;

class Font;
}  // namespace sf

namespace Textures {
enum ID {
//	Background,
//	Title,
//
    Board,
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

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;

#endif  //NATUREOFCODE_RESOURCEIDENTIFIERS_HPP
