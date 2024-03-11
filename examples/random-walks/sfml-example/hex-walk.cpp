#include <iostream>
#include <cstdlib>
#include <array>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "tess/tess.hpp"

struct app_settings{
    std::string window_name{ "hex-walk" };
    sf::VideoMode video_mode{ 800, 600 };
    sf::Color bg_color = sf::Color::White;

    float unit_size = 50.f;
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

    // initialize the hexes we're working with
    // and set some basic graphical settings
    std::vector<tess::hex<int>> hexes;
    tess::hex_range(tess::hex<int>::zero, 3, std::back_inserter(hexes));

    std::vector<sf::ConvexShape> shapes;
    shapes.reserve(hexes.size());
    for (const auto& hex : hexes) {
        auto& shape = shapes.emplace_back(hex_shape(basis, hex));
        shape.setFillColor(sf::Color::Black);
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1.0f);
    }
    sf::RenderWindow window{ settings.video_mode, settings.window_name };

    // busy loop until the user quits
    while (window.isOpen()) {
        sf::Event event{};

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
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