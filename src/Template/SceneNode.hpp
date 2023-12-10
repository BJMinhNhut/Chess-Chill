//
// Created by MINH NHUT on 6/13/2023.
//

#ifndef NATUREOFCODE_SCENENODE_HPP
#define NATUREOFCODE_SCENENODE_HPP

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/System/NonCopyable.hpp"
#include "SFML/System/Time.hpp"

#include <memory>
#include <vector>

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable {
   public:
	typedef std::unique_ptr<SceneNode> Ptr;
	enum Transition {
		None,
		Smooth,
	};

   public:
	SceneNode();

	void attachChild(Ptr child);
	Ptr detachChild(const SceneNode& node);
	void detachAllChildren();

	void update(sf::Time dt);

	virtual void setPosition(sf::Vector2f position);
	virtual void setPosition(float pX, float pY);

	sf::Vector2f getWorldPosition() const;
	sf::Transform getWorldTransform() const;

   private:
	virtual void updateCurrent(sf::Time dt);
	void updateChildren(sf::Time dt);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

   private:
	std::vector<Ptr> mChildren;
	SceneNode* mParent;

   protected:
	static const float EPS;
};

#endif  //NATUREOFCODE_SCENENODE_HPP
