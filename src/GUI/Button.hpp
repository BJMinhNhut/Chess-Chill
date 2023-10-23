//
// Created by MINH NHUT on 7/2/2023.
//

#ifndef DATAVISUALIZATION2_BUTTON_HPP
#define DATAVISUALIZATION2_BUTTON_HPP

#include "GUI/Component.hpp"
#include "Template/ColorHolder.hpp"
#include "Template/ResourceHolder.hpp"
#include "Template/ResourceIdentifiers.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace GUI {
    class Button : public Component {
    public:
        enum Type {
            Home,
            Back,
            Command,
            Checkbox,
            Play,
            Pause,
            Replay,
            SmallArrow,
            Arrow,
            DoubleArrow,
            Small,
            Big,
            MenuHash,
            MenuAVL,
            Menu234,
            MenuMinHeap,
            MenuMaxHeap,
            MenuTrie,
            MenuGraph,
        };

    public:
        typedef std::shared_ptr<Button> Ptr;
        typedef std::function<void()> Callback;

    public:
        Button(Type type, const FontHolder &fonts,
               const TextureHolder &textures, const ColorHolder &colors);

        void setCallback(Callback callback);

        void setText(const std::string &text);

        void setToggle(bool flag);

        virtual bool isSelectable() const;

        virtual void select();

        virtual void deselect();

        virtual void activate();

        virtual void deactivate();

        virtual void handleEvent(const sf::Event &event);

        virtual bool contains(sf::Vector2i point) const;

    private:
        virtual void draw(sf::RenderTarget &target,
                          sf::RenderStates states) const;

        static Textures::ID getNormalTextureID(Type type);

        static Textures::ID getSelectedTextureID(Type type);

        static Textures::ID getPressedTextureID(Type type);

        void setFont(Type type, const FontHolder &fonts,
                     const ColorHolder &colors);

    private:
        Callback mCallback;
        const sf::Texture mNormalTexture;
        const sf::Texture mSelectedTexture;
        const sf::Texture mPressedTexture;

        sf::Sprite mSprite;
        sf::Text mText;

        bool mIsToggle;
    };

}  // namespace GUI

#endif //DATAVISUALIZATION2_BUTTON_HPP
