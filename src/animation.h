#ifndef ANIMATION_H
#define ANIMATION_H

#include <list>
#include <SFML/Graphics.hpp>

/*******************************************************************/
struct Animation {
    Animation() { }

    virtual ~Animation() {}

    virtual void update(sf::Time now, sf::Transformable & object) = 0;

    bool isComplete()                     const { return complete_; }
    void setStartTime(sf::Time time)            { startTime_ = time; }
    void setDuration(sf::Time time)             { duration_ = time; }

protected:
    float getPercent(sf::Time now);
    void setComplete(bool complete) { complete_ = complete; }

private:
    sf::Time startTime_;
    sf::Time duration_;
    bool complete_ = false;
};

/*******************************************************************/
struct RotateMotion : public Animation {
    RotateMotion(float rotations);
    virtual ~RotateMotion() { }

    virtual void update(sf::Time now, sf::Transformable & object) final;

private:
    float rotation_;
};

/*******************************************************************/
struct LinearMotion : public Animation {
    LinearMotion(sf::Vector2i const & startPos, sf::Vector2i const & endPos);
    virtual ~LinearMotion() { }

    virtual void update(sf::Time now, sf::Transformable & object) final;

private:
    sf::Vector2i startPos_;
    sf::Vector2i endPos_;
};

#endif
