#include "babyos.h"

/*******************************************************************/
PictureAnimator::PictureAnimator(Picture & picture)
  : picture_(picture)
{ } 

/*******************************************************************/
bool PictureAnimator::update(sf::Time /*now*/) {
    bool complete = true;
    sf::Time now = clock_.getElapsedTime();

    for (auto it = animations_.begin(); it != animations_.end(); ++it) {
        auto & animation = **it;
        if (!animation.isComplete()) {
            animation.update(now, picture_.sprite());
            complete = false;
        }
    }

    return complete;
}

/*******************************************************************/
void PictureAnimator::draw(sf::RenderTarget & target) {
    picture_.draw(target);
}

/*******************************************************************/
Animation & PictureAnimator::addAnimation(AnimationPtr animation) {
    animation->setStartTime(clock_.getElapsedTime());
    return *animations_.emplace_back(std::move(animation));
}

/*******************************************************************/
Picture & PictureAnimator::picture() {
    return picture_;
}

/*******************************************************************/
void BabyOS::loadTexturesFrom(std::string directory) {
    ImageScanner scanner(directory);
    scanner.scan();
    textureRepo_.addFromScanner(scanner);
}

/*******************************************************************/
sf::Vector2i BabyOS::getRandomStart(unsigned offset) {
    sf::Vector2i start;
    if (random_.generate(2)) {
        if (random_.generate(2)) {
            start.y = size_.y + 50;
        }
        else {
            start.y = -50;
        }
        start.x = random_.generate(size_.x);
    }
    else {
        if (random_.generate(2)) {
            start.x = size_.x + 50;
        }
        else {
            start.x = -50;
        }
        start.y = random_.generate(size_.y);
    }

    return start;
}

/*******************************************************************/
sf::Vector2i BabyOS::getRandomEnd() {
    return sf::Vector2i(random_.generate(size_.x),
                     random_.generate(size_.y));
}

/*******************************************************************/
void BabyOS::addAnimations(PictureAnimator & animator) {
    auto start = getRandomStart(50);
    auto end = getRandomEnd();

    auto & animation = animator.addAnimation(
                std::make_unique<LinearMotion>(
                    start,
                    end
                )
            );
    animation.setDuration(sf::seconds(2));

    auto & animation2 = animator.addAnimation(
                std::make_unique<RotateMotion>(
                    (float)random_.generate(300) / 200
                )
            );
    animation2.setDuration(sf::seconds(2));
}

/*******************************************************************/
void BabyOS::addRandomPicture() {
    if (textureRepo_.size() == 0) {
        throw std::runtime_error("No textures loaded");
    }

    auto texIdx = random_.generate(textureRepo_.size());
    auto & picturePtr = pictures_.emplace_back(
                            textureRepo_.getTexture(texIdx).createPicture()
                        );

    auto & animator = animators_.emplace_back(*picturePtr.get());
    addAnimations(animator);
}

/*******************************************************************/
void BabyOS::addPictureToBase(Picture & picture) {
    picture.draw(baseTexture_);
    baseTexture_.display();
}

/*******************************************************************/
void BabyOS::update() {
    completeAnimators_.clear();

    sf::Time now = clock_.getElapsedTime();
    for (auto it = animators_.begin(); it != animators_.end(); ++it) {
        bool complete = it->update(now);
        if (complete) {
            completeAnimators_.push_back(it);
        }
    }
}

/*******************************************************************/
void BabyOS::draw(sf::RenderWindow & window) {
    window.draw(baseSprite_);
    for (auto & a : animators_) {
        a.draw(window);
    }

    for (auto & it : completeAnimators_) {
        addPictureToBase(it->picture());
        animators_.erase(it);
    }
}

/*******************************************************************/
void BabyOS::setSize(sf::Vector2u size) {
    size_ = size;

    baseTexture_.create(size.x, size.y);
    baseSprite_.setTexture(baseTexture_.getTexture());
}

/*******************************************************************/
void BabyOS::setImageSize(sf::Vector2u imageSize) {
    textureRepo_.setMaxSize(imageSize);
}
