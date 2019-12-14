#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Window.hpp>

struct Entity {
    virtual ~Entity() { }

    virtual bool update(sf::Time now) { return true; };
    virtual void draw(sf::RenderTarget & target) { };
};

#endif
