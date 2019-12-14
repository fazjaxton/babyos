#include "picture.h"

/*******************************************************************/
Picture::Picture(sf::Texture const & texture)
  : sprite_(texture)
{
    sprite_.setOrigin((float)texture.getSize().x / 2,
                      (float)texture.getSize().y / 2);
}

/*******************************************************************/
Picture::Picture(Picture && other)
    : sprite_(other.sprite_)
{ }

/*******************************************************************/
void Picture::setScale(float scale)
{
    sprite_.setScale(scale, scale);
}

/*******************************************************************/
void Picture::draw(sf::RenderTarget & target)
{
    target.draw(sprite_);
}
