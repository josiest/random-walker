// frameworks
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <simulacrum/random_walks.hpp>

// data types and data structures
#include <cstdint>
#include <string>
#include <vector>

// math and algorithms
#include <random>
#include <algorithm>

// i/o
#include <iostream>

namespace cardinal = simulacrum::cardinal;
namespace ranges = std::ranges;

auto as_pixel(std::uint32_t pixel_size)
{
    return [pixel_size](sf::Vector2i const & point) {

        sf::RectangleShape pixel;
        pixel.setSize(sf::Vector2f(pixel_size, pixel_size));
        pixel.setPosition(point.x*pixel_size, point.y*pixel_size);
        pixel.setFillColor(sf::Color::Black);
        return pixel;
    };
}

auto draw_pixel(sf::RenderWindow & window)
{
    return [&window](sf::RectangleShape const & pixel) { window.draw(pixel); };
}

int main(int argc, char * argv[])
{
    // do the random walk
    std::uint32_t N = 100;
    if (argc == 2) { try {
        N = std::stoi(argv[1]);
    }
    // fail if any error occured while converting th N-argument to int
    catch (...) {
        std::cout << "couldn't convert \"" << argv[1] << "\" to int" << std::endl;
        return EXIT_FAILURE;
    }}
    // fail if the wrong number of arguments were provided
    else if (argc > 2) {
        std::cout << "usage: walk [N]" << std::endl
                  << " N - number of steps to take" << std::endl;
        return EXIT_FAILURE;
    }

    std::uint32_t const width = 800;
    std::uint32_t const height = 600;
    std::uint32_t const pixel_size = 8;

    // simulate the random walk
    std::random_device seed;
    std::mt19937 rng(seed());

    sf::Vector2i const start(width/(2*pixel_size), height/(2*pixel_size));
    cardinal::walker homer(start);

    std::vector<sf::Vector2i> points;
    points.reserve(N);

    auto into_points = std::back_inserter(points);
    ranges::generate_n(into_points, N, cardinal::uniform_walk(rng, homer));

    // create the window
    sf::RenderWindow window(sf::VideoMode(width, height), "random-walk");

    // draw the points and display to the screen
    std::vector<sf::RectangleShape> pixels;
    pixels.reserve(N);
    auto into_pixels = std::back_inserter(pixels);

    window.clear(sf::Color::White);
    ranges::transform(points, into_pixels, as_pixel(pixel_size));
    ranges::for_each(pixels, draw_pixel(window));
    window.display();

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }
    return EXIT_SUCCESS;
}