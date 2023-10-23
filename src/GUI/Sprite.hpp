//
// Created by MINH NHUT on 7/4/2023.
//

#ifndef DATAVISUALIZATION2_SPRITE_HPP
#define DATAVISUALIZATION2_SPRITE_HPP

#include "GUI/Component.hpp"
#include "Template/ResourceHolder.hpp"
#include "Template/ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace GUI {

    class Sprite : public Component {
    public:
        typedef std::shared_ptr<Sprite> Ptr;

    public:
        explicit Sprite(const sf::Texture &texture);

        virtual bool isSelectable() const;

        void setTexture(const sf::Texture &texture);

        virtual void handleEvent(const sf::Event &event);

    private:
        void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    private:
        sf::Sprite mSprite;
    };

}  // namespace GUI


#endif //DATAVISUALIZATION2_SPRITE_HPP
