#ifndef BABYOS_H
#define BABYOS_H

#include <list>
#include <SFML/Graphics.hpp>

#include "animation.h"
#include "entity.h"
#include "picture.h"
#include "random.h"
#include "texture_repository.h"
#include "image_scanner.h"

struct PictureAnimator : public Entity {
    using AnimationPtr = std::unique_ptr<Animation>;

    PictureAnimator(Picture & picture);

    virtual ~PictureAnimator() { }

    virtual bool update(sf::Time /*now*/) override;
    virtual void draw(sf::RenderTarget & target) override;

    Animation & addAnimation(AnimationPtr animation);
    Picture & picture();

private:
    Picture & picture_;
    std::vector<AnimationPtr> animations_;
    sf::Clock clock_;
};

struct BabyOS {
    // Scale by powers of two for possibly better scaling results
    static constexpr bool integralScaling = true;

    void loadTexturesFrom(std::string directory);

    void addAnimations(PictureAnimator & animator);
    void addRandomPicture();
    void addPictureToBase(Picture & picture);

    void update();
    void draw(sf::RenderWindow & window);

    void setSize(sf::Vector2u size);
    void setImageSize(sf::Vector2u imgSize);

    size_t imageCount() const { return textureRepo_.size(); }

private:
    sf::Vector2i getRandomStart(unsigned offset);
    sf::Vector2i getRandomEnd();

    void scalePicture(Picture & picture);

private:
    using PicturePtr = std::unique_ptr<Picture>;
    using AnimatorIterator = std::list<PictureAnimator>::iterator;

    TextureRepository textureRepo_;
    RandomGenerator<size_t> random_;

    std::list<PicturePtr> pictures_;
    std::list<PictureAnimator> animators_;
    std::list<AnimatorIterator> completeAnimators_;

    sf::Sprite baseSprite_;
    sf::RenderTexture baseTexture_;
    sf::Vector2u size_;
    sf::Clock clock_;
};

#endif
