//
// Created by MINH NHUT on 7/2/2023.
//

#include "Component.hpp"

namespace GUI {

    Component::Component() : mIsSelected(false), mIsActive(false) {}

    Component::~Component() = default;

    bool Component::isSelected() const {
        return mIsSelected;
    }

    void Component::select() {
        mIsSelected = true;
    }

    void Component::deselect() {
        mIsSelected = false;
    }

    bool Component::isActive() const {
        return mIsActive;
    }

    void Component::activate() {
        mIsActive = true;
    }

    void Component::deactivate() {
        mIsActive = false;
    }

    bool Component::contains(sf::Vector2i point) const {
        return false;
    }

    void Component::update(sf::Time dt) {
        // do nothing as default
    }

}  // namespace GUI