#include <iostream>
#include <cstdlib>
#include <random>

#include <array>
#include <unordered_set>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "tess/tess.hpp"
#include "spatula/directions.hpp"
#include "simulacrum/random_walks.hpp"

namespace sim = simulacrum;

struct app_settings{
    std::string window_name{ "hex-walk" };
    sf::VideoMode video_mode{ 800, 600 };
    sf::Color bg_color = sf::Color::White;

    float unit_size = 10.f;
    std::uint32_t num_steps = 200;
};

// convert a hex coordinate to an sfml shape
template<std::integral Integer>
sf::ConvexShape hex_shape(const tess::pointed_fbasis& basis,
                          const tess::hex<Integer>& hex)
{
    sf::ConvexShape shape{6};
    // if sf::Shape had begin() -> output_iterator<Vector2f>
    // basis.vertices(hex, shape.begin());

    // compute the vertices for the shape
    std::array<sf::Vector2f, 6> verts;
    basis.vertices<sf::Vector2f>(hex, verts.begin());
    for (int i = 0; i < verts.size(); i++) {
        shape.setPoint(i, verts[i]);
    }
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(1.f);
    return shape;
}

int main()
{
    const app_settings settings{};
    const tess::pointed_fbasis basis{ static_cast<float>(settings.video_mode.width)/2.f,
                                      static_cast<float>(settings.video_mode.height)/2.f,
                                      settings.unit_size };

    std::random_device seed{};
    std::mt19937 rng{seed()};

    // initialize the hexes we're working with
    // and set some basic graphical settings
    std::vector<tess::hex<int>> hexes;
    hexes.reserve(settings.num_steps);
    auto into_hexes = std::inserter(hexes, hexes.begin());

    using pointed_hex = sp::pointed_hex::direction_name;
    const tess::hex<int> start{ 0, 0 };
    std::generate_n(into_hexes, settings.num_steps,
                    sim::uniform_walk<pointed_hex>(rng, start));

    std::vector<sf::ConvexShape> shapes;
    shapes.reserve(hexes.size());
    auto& origin = shapes.emplace_back(hex_shape(basis, hexes.front()));
    origin.setFillColor(sf::Color::Cyan);
    origin.setOutlineThickness(1.f);
    origin.setOutlineColor(sf::Color::White);

    sf::RenderWindow window{ settings.video_mode, settings.window_name };

    // busy loop until the user quits
    while (window.isOpen()) {
        sf::Event event{};

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                // click to advance the walk
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (shapes.size() < hexes.size()) {
                        auto& current_shape = shapes.back();
                        current_shape.setFillColor(sf::Color::Black);

                        const auto& next_hex = hexes[shapes.size()];
                        auto& next_shape = shapes.emplace_back(hex_shape(basis, next_hex));
                        next_shape.setFillColor(sf::Color::Cyan);
                        next_shape.setOutlineThickness(1.f);
                        next_shape.setOutlineColor(sf::Color::White);
                    }
                }
            }
        }

        window.clear(settings.bg_color);
        for (const auto& hex : shapes) {
            window.draw(hex);
        }
        window.display();
    }
    return EXIT_SUCCESS;
}