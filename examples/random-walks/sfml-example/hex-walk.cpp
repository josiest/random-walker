#include <iostream>
#include <cstdlib>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

struct window_settings{
    std::string name{ "hex-walk" };
    sf::VideoMode video_mode{ 800, 600 };
    sf::Color bg_color = sf::Color::White;
};

sf::RenderWindow make_window(const window_settings& settings) {
    return sf::RenderWindow{ settings.video_mode, settings.name };
}

int main(int argc, char* argv[])
{
    const window_settings settings{};
    auto window = make_window(settings);
    window.clear(settings.bg_color);
    window.display();
    // busy loop until the user quits
    while (window.isOpen()) {
        sf::Event event{};

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }
    return EXIT_SUCCESS;
}