#include "animation.h"

/*******************************************************************/
float Animation::getPercent(sf::Time now)
{
    float percent = (now - startTime_) / duration_;
    return (percent >= 1.0f) ? 1.0f : percent;
}

/*******************************************************************/
RotateMotion::RotateMotion(float rotations)
    : rotation_(-rotations * 360)
{ }

/*******************************************************************/
void RotateMotion::update(sf::Time now, sf::Transformable & object)
{
    auto percent = getPercent(now);
    object.setRotation(rotation_ * (1.0f - percent));
    setComplete(percent >= 1.0);
}

/*******************************************************************/
LinearMotion::LinearMotion(sf::Vector2i const & startPos, sf::Vector2i const & endPos)
    : startPos_(startPos)
    , endPos_(endPos)
{ }

/*******************************************************************/
void LinearMotion::update(sf::Time now, sf::Transformable & object)
{
    auto percent = getPercent(now);
    int x = (endPos_.x - startPos_.x) * percent + startPos_.x;
    int y = (endPos_.y - startPos_.y) * percent + startPos_.y;

    object.setPosition(x, y);
    setComplete(percent >= 1.0);
}
