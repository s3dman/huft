#include <SFML/Graphics.hpp>

int main()
{
    auto window = sf::RenderWindow{ { 1000u, 1000u }, "huff-etv" };
    window.setFramerateLimit(144);

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear(sf::Color(255,255,255,255));
        window.display();
    }
}