//
// Created by MINH NHUT on 7/2/2023.
//

#ifndef DATAVISUALIZATION2_COMPONENT_HPP
#define DATAVISUALIZATION2_COMPONENT_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <memory>

namespace sf {
    class Event;
}  // namespace sf

namespace GUI {
    class Component : public sf::Drawable,
                      public sf::Transformable,
                      private sf::NonCopyable {
    public:
        typedef std::shared_ptr<Component> Ptr;

    public:
        Component();

        virtual ~Component();

        virtual bool isSelectable() const = 0;

        bool isSelected() const;

        virtual void select();

        virtual void deselect();

        virtual bool isActive() const;

        virtual void activate();

        virtual void deactivate();

        virtual bool contains(sf::Vector2i point) const;

        virtual void update(sf::Time dt);

        virtual void handleEvent(const sf::Event &event) = 0;

    private:
        bool mIsSelected;
        bool mIsActive;
    };
}  // namespace GUI

#endif //DATAVISUALIZATION2_COMPONENT_HPP
