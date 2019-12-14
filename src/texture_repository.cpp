#ifndef TEXTURE_REPOSITORY_H
#define TEXTURE_REPOSITORY_H

#include <cassert>

#include "picture.h"
#include "texture_repository.h"

namespace {

float getScale(sf::Vector2u const & size, sf::Vector2u const & maxSize) {
    float const ratioX = (float)size.x / maxSize.x;
    float const ratioY = (float)size.y / maxSize.y;
    float ratio = std::max(ratioX, ratioY);

    return 1.0 / ratio;
}

}

/*******************************************************************/
Texture::Texture(
        sf::Texture const & texture,
        sf::Vector2u const & maxSize)
{
    static const sf::ContextSettings settings{
        0,      /* depth bits */
        0,      /* stencil bits */
        4};     /* antialiasing level */

    // Calculate scale to apply to image
    auto const size = texture.getSize();
    float renderScale = getScale(size, maxSize);

    sf::Sprite sprite(texture);

    if (renderScale < 1.0) {
        // Scale down when rendering to video memory
        sprite.setScale(renderScale, renderScale);
        scale_ = 1.0;
    }
    else {
        // Render to video memory at current size and scale up when displaying
        scale_ = renderScale;
        renderScale = 1.0;
    }

    // Size texture to hold scaled image
    renderTexture_.create(size.x * renderScale, size.y * renderScale, settings);

    // Render image to texture
    renderTexture_.draw(sprite);
    renderTexture_.display();
}

/*******************************************************************/
std::unique_ptr<Picture> Texture::createPicture() const
{
    auto picture = std::make_unique<Picture>(renderTexture_.getTexture());
    picture->setScale(scale_);
    return picture;
}

/*******************************************************************/
void TextureRepository::addFromFile(std::string const & filename)
{
    assert(maxSize_.x > 0 && maxSize_.y > 0 && "setMaxSize() not called before addFromFile()");

    sf::Image image;
    if (!image.loadFromFile(filename)) {
        return;
    }

    sf::Texture texture;
    if (!texture.loadFromImage(image)) {
        return;
    }

    textures_.emplace_back(new Texture(texture, maxSize_));
}

/*******************************************************************/
void TextureRepository::addFromScanner(ImageScanner const & scanner)
{
    for (auto const & filename : scanner.getImageFilenames()) {
        addFromFile(filename);
    }
}

/*******************************************************************/
void TextureRepository::setMaxSize(sf::Vector2u const & size)
{
    maxSize_ = size;
}

#endif
