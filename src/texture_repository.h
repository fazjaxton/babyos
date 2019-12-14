#ifndef TEXTURE_H
#define TEXTURE_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "image_scanner.h"

struct Picture;

struct Texture {
    Texture(sf::Texture const & texture,
            sf::Vector2u const & maxSize);

    std::unique_ptr<Picture> createPicture() const;

private:
    sf::RenderTexture renderTexture_;
    float scale_ = 1.0f;
};

struct TextureRepository {
    using TexturePtr = std::unique_ptr<Texture>;

    void addFromFile(std::string const & filename);
    void addFromScanner(ImageScanner const & scanner);
    void setMaxSize(sf::Vector2u const & size);

    size_t              size()                      const { return textures_.size(); }
    Texture const &     getTexture(size_t idx)      const { return *textures_.at(idx); }

private:
    std::vector<TexturePtr> textures_;
    sf::Vector2u maxSize_;
};


#endif
