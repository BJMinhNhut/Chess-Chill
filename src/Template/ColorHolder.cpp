//
// Created by MINH NHUT on 7/3/2023.
//

#include "ColorHolder.hpp"

void ColorHolder::load(Colors::ID id, const sf::Color &color) {
    std::unique_ptr<sf::Color> colorPtr(new sf::Color(color));
    insertColor(id, std::move(colorPtr));
}

const sf::Color &ColorHolder::get(Colors::ID id) const {
    auto found = mColorMap.find(id);
    assert(found != mColorMap.end());

    return *found->second;
}

void ColorHolder::insertColor(Colors::ID id,
                              std::unique_ptr<sf::Color> color) {
    // Insert and check
    if (mColorMap.find(id) == mColorMap.end()) {
        auto inserted =
                mColorMap.insert(std::make_pair(id, std::move(color)));
        assert(inserted.second);
    } else {
        mColorMap[id] = std::move(color);
    }
}