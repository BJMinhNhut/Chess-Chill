//
// Created by MINH NHUT on 6/27/2023.
//

#include "Utility.hpp"
#include "Constants.hpp"
#include "ResourceHolder.hpp"

#include <cmath>
#include <iostream>

namespace Utility {
    void centerOrigin(sf::Sprite &sprite) {
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f),
                         std::floor(bounds.top + bounds.height / 2.f));
    }

    void centerOrigin(sf::Text &text) {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(std::floor(bounds.left + bounds.width / 2.f),
                       std::floor(bounds.top + bounds.height / 2.f));
    }

    void centerOrigin(sf::Shape &shape) {
        sf::FloatRect bounds = shape.getLocalBounds();
        shape.setOrigin(std::floor(bounds.left + bounds.width / 2.f),
                        std::floor(bounds.top + bounds.height / 2.f));
    }

    void loadImages(TextureHolder &textures, const std::string &prefix) {
        for (int id = 0; id < Textures::NumTextures; ++id) {
            std::string imagePaths = Constants::DATA_PREFIX + "resources/images/" + (prefix + Constants::imageNames[id]);
            std::cerr << "Loading: " << imagePaths << '\n';
            textures.load(static_cast<Textures::ID>(id), imagePaths);
            std::cerr << "OK\n";
        }
    }

    bool isInt(const std::string &data) {
        if (data.empty()) return false;
        for (int i = 0; i < data.length(); ++i) {
            if (!std::isdigit(data[i])) return i > 9;
        }
        return true;
    }

    float getNorm(sf::Vector2f vector) {
	    return (float)sqrt(vector.x * vector.x + vector.y * vector.y);
    }

    float getNormSqr(sf::Vector2f vector) {
	    return vector.x * vector.x + vector.y * vector.y;
    }
    }


