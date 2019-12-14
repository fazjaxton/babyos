#include <algorithm>
#include <iostream>
#include <fmt/core.h>
#include <getopt.h>
#include <SFML/Window.hpp>

#include "babyos.h"
#include "cxxopts/cxxopts.hpp"
#include "util.h"

struct Options {
    std::vector<std::string> imageDirs;
    bool lock = false;
    std::string exitCode = "quit";
    size_t width = 0;
    size_t height = 0;
    bool widthIsPercent = false;
    bool heightIsPercent = false;
};

/*******************************************************************/
static Options parseCmdLine(int argc, char ** argv) {
    using ArgSpec = cxxopts::Options;

    ArgSpec spec(basename(argv[0]));
    spec.add_options()
        (
            "h,help",
            "Display this help message"
        )
        (
            "d,imagedir",
            "Directory to search for images, may be specified more than once",
            cxxopts::value<std::vector<std::string>>(),
            "<directory>"
        )
        (
            "l,lock",
            "Prevent window from being closed (ignore close events) until code is entered"
        )
        (
            "c,code",
            "Sequence of keys to exit program (default: quit)",
            cxxopts::value<std::string>(),
            "<code>"
        )
        (
            "z,size",
            "Target geometry for images.  Images will be scaled to near this size while keeping "
                "original aspect ratio.  If only one value is specified, it is used for both "
                "width and height.  Pixels or percentages",
            cxxopts::value<std::string>(),
            "<width>[%][,<height>[%]]"
        )
        ;

    cxxopts::ParseResult res;
    try {
        res = spec.parse(argc, argv);
    }
    catch (cxxopts::OptionParseException const & e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << spec.help() << std::endl;
        ::exit(1);
    }

    if (res.count("help")) {
        std::cout << spec.help() << std::endl;
        ::exit(0);
    }

    Options options;
    if (res.count("imagedir")) {
        options.imageDirs = res["imagedir"].as<std::vector<std::string>>();
    }
    else {
        options.imageDirs.push_back(".");
    }

    if (res.count("lock")) {
        options.lock = true;
    }

    if (res.count("code")) {
        auto & code = options.exitCode;
        code = res["code"].as<std::string>();

        if (code.size() == 0) {
            std::cerr << "Cannot specify empty code" << std::endl;
            ::exit(1);
        }

        toLower(code);
    }

    if (res.count("size")) {
        auto parseSize = [](std::string const & s, size_t & val, bool & isPercent) {
            char const * beg = s.c_str();
            char * end;
            val = ::strtoul(beg, &end, 10);

            if (*end == '%') {
                ++end;
                isPercent = true;
            }
            else {
                isPercent = false;
            }

            bool const valid = static_cast<size_t>(end - beg) == s.size();
            if (!valid) {
                std::cerr << "Invalid geometry" << std::endl;
                ::exit(1);
            }
        };

        auto str = res["size"].as<std::string>();
        auto idx = str.find(',');

        if (idx >= 0) {
            auto const widthStr = str.substr(0, idx);
            auto const heightStr = str.substr(idx + 1);
            parseSize(widthStr, options.width, options.widthIsPercent);
            parseSize(heightStr, options.height, options.heightIsPercent);
        }
        else {
            parseSize(str.c_str(), options.width, options.widthIsPercent);
            options.height = options.width;
            options.heightIsPercent = options.widthIsPercent;
        }
    }

    return options;
}

/*******************************************************************/
// Shift the buffer left and add the character to the end
void addCodeChar(std::string & code, char c) {
    assert(code.size() > 0);

    if (code.size() == 1) {
        code[0] = c;
    }
    else {
        // shift characters left one position
        std::copy(code.begin() + 1, code.end(), code.begin());
        code.back() = c;
    }
}

/*******************************************************************/
int main(int argc, char ** argv) {
    Options options = parseCmdLine(argc, argv);

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    BabyOS babyos;
    babyos.setSize(sf::Vector2u(desktop.width, desktop.height));

    if (options.width > 0 && options.height > 0) {
        size_t const imgWidth = options.widthIsPercent ?
                desktop.width * options.width / 100 :
                options.width;
        size_t const imgHeight = options.heightIsPercent ?
                desktop.height * options.height / 100 :
                options.width;

        babyos.setImageSize(sf::Vector2u(imgWidth, imgHeight));
    }
    else {
        babyos.setImageSize(sf::Vector2u(
                    desktop.width / 4,
                    desktop.height / 3));
    }

    std::cout << "Loading images..." << std::flush;
    for (auto const & dir : options.imageDirs) {
        babyos.loadTexturesFrom(dir);
    }

    if (babyos.imageCount() == 0) {
        std::cerr << "No images loaded" << std::endl;
        ::exit(1);
    }

    std::cout << "Done" << std::endl;

    sf::RenderWindow window(desktop, "BabyOS", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);

    sf::Clock clock;
    // Rotating buffer of entered characters the same size as the exit code
    std::string enteredCode(options.exitCode.size(), '\0');

    while (window.isOpen()) {
        sf::Event event;
        
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    if (!options.lock) {
                        window.close();
                    }
                    break;
                case sf::Event::TextEntered:
                    // ASCII only
                    if (event.text.unicode <= 127) {
                        char const c = static_cast<char>(event.text.unicode);
                        addCodeChar(enteredCode, std::tolower(c));
                        if (enteredCode == options.exitCode) {
                            window.close();
                        }
                    }
                    break;
                case sf::Event::KeyPressed:
                    babyos.addRandomPicture();
                    break;
                default:
                    break;
            }
        }

        window.clear();
        babyos.update();
        babyos.draw(window);
        window.display();

        auto elapsed = clock.restart();
        if (elapsed < sf::microseconds(16666)) {
            sf::sleep(sf::microseconds(16666) - elapsed);
        }
    }

    return 0;
}
