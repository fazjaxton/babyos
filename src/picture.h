#ifndef PICTURE_H
#define PICTURE_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

struct Picture {
    Picture(sf::Texture const & texture);

    Picture(Picture const & other) = delete;
    Picture & operator=(Picture const &) = delete;

    Picture(Picture && other);

    sf::Sprite & sprite()                   { return sprite_; }

    void setScale(float scale);

    void draw(sf::RenderTarget & target);

private:
    sf::Sprite sprite_;
};

#endif
