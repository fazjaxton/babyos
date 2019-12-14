#ifndef IMAGE_SCANNER_H
#define IMAGE_SCANNER_H

#include <filesystem>
#include <SFML/Graphics.hpp>
#include <unordered_set>

// Class to load image files from a directory
struct ImageScanner {
    using ImageFilenames = std::unordered_set<std::string>;

    // Initialize with name of directory to scan
    ImageScanner(std::string const & path);

    // Perform the scan
    void scan();

    // Retrieve filenames
    ImageFilenames const & getImageFilenames() const;

private:
    std::filesystem::path dirPath_;
    ImageFilenames imageFilenames_;
};

#endif
