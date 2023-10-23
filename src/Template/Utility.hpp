//
// Created by MINH NHUT on 6/27/2023.
//

#ifndef DATAVISUALIZATION2_UTILITY_HPP
#define DATAVISUALIZATION2_UTILITY_HPP

#include "ResourceIdentifiers.hpp"
#include "ColorHolder.hpp"

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sf {
    class Sprite;

    class Text;
}  // namespace sf

namespace Utility {
    void centerOrigin(sf::Sprite &sprite);

    void centerOrigin(sf::Text &text);

    void centerOrigin(sf::Shape &rect);

    bool isInt(const std::string &data);

    float getNorm(sf::Vector2f vector);

    float getNormSqr(sf::Vector2f vector);

    void loadImages(TextureHolder &textures, const std::string &prefix);
}

#endif //DATAVISUALIZATION2_UTILITY_HPP
