//
// Created by MINH NHUT on 7/2/2023.
//

#include "Container.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>

namespace GUI {

Container::Container() : mChildren(), mSelectedChild(-1), mActivatedChild(-1) {}

void Container::pack(Component::Ptr component) {
	mChildren.push_back(component);
}

void Container::activateChild(Component::Ptr component) {
	for (int index = 0; index < mChildren.size(); ++index)
		if (mChildren[index] == component)
			activate(index);
}

void Container::reset() {
	if (hasActivation()) {
		mChildren[mActivatedChild]->deactivate();
		mChildren[mActivatedChild]->deselect();
	}

	if (hasSelection())
		mChildren[mSelectedChild]->deselect();
	mSelectedChild = mActivatedChild = -1;
}

bool Container::isSelectable() const {
	return false;
}

void Container::update(sf::Time dt) {
	for (int index = 0; index < mChildren.size(); index++) {
		mChildren[index]->update(dt);
	}
}

void Container::handleEvent(const sf::Event& event) {
	// If we have selected a child then give it events
	if (hasActivation()) {
		mChildren[mActivatedChild]->handleEvent(event);
	}

	//	std::cout << "*current " << mSelectedChild << '\n';
	if (event.type == sf::Event::MouseMoved) {
		updateSelect(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
	} else if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			updateSelect(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
			if (hasSelection()) {
				activate(mSelectedChild);
				//				std::cout << "activate " << mSelectedChild << '\n';
			}
		}
	}
}

void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();

	for (const Component::Ptr& child : mChildren)
		target.draw(*child, states);
}

bool Container::hasSelection() const {
	return mSelectedChild >= 0;
}

bool Container::hasActivation() const {
	return mActivatedChild >= 0;
}

void Container::select(std::size_t index) {
	if (mChildren[index]->isSelectable() && !mChildren[index]->isActive()) {
		if (hasSelection() && !mChildren[mSelectedChild]->isActive())
			mChildren[mSelectedChild]->deselect();

		//		std::cout << "select " << index << '\n';
		mChildren[index]->select();
		mSelectedChild = index;
	}
}

void Container::activate(std::size_t index) {
	if (mChildren[index]->isSelectable() && !mChildren[index]->isActive()) {
		if (hasActivation()) {
			mChildren[mActivatedChild]->deactivate();
			mChildren[mActivatedChild]->deselect();
		}
		mChildren[index]->activate();
		if (mChildren[index]->isActive())
			mActivatedChild = index;
	}
}

void Container::updateSelect(sf::Vector2i point) {
	for (int index = 0; index < mChildren.size(); ++index) {
		if (mChildren[index]->isSelectable() && !mChildren[index]->isActive() &&
		    mChildren[index]->contains(point)) {
			select(index);
			return;
		}
	}
	// no component is selected
	if (hasSelection() && !mChildren[mSelectedChild]->isActive()) {
		//		std::cout << "deselect " << mSelectedChild << '\n';
		mChildren[mSelectedChild]->deselect();
		mSelectedChild = -1;
	}
}

}  // namespace GUI