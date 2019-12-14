#include <filesystem>

#include "image_scanner.h"
#include "util.h"

/*******************************************************************/
ImageScanner::ImageScanner(std::string const & path)
    : dirPath_(path)
{
    if (!std::filesystem::exists(dirPath_)) {
        throw std::runtime_error("Directory not found");
    }
    if (!std::filesystem::is_directory(dirPath_)) {
        throw std::runtime_error("Is not directory");
    }
}

/*******************************************************************/
ImageScanner::ImageFilenames const & ImageScanner::getImageFilenames() const
{ 
    return imageFilenames_;
}

/*******************************************************************/
void ImageScanner::scan()
{
    for (auto entry : std::filesystem::directory_iterator(dirPath_)) {
        if (!entry.exists() || !entry.is_regular_file()) {
            continue;
        }
        std::string file = entry.path().string();

        auto dotIdx = file.find_last_of('.');
        if (dotIdx == std::string::npos) {
            continue;
        }

        std::string extension = file.substr(dotIdx + 1);
        toLower(extension);

        if (extension == "jpg" || extension == "jpeg" || extension == "png") {
            imageFilenames_.insert(file);
        }
    }
}
