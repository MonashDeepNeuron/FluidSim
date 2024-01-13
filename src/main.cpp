#include <SFML/Graphics.hpp>

// Define grid size
constexpr int N = 100;
constexpr int SIZE = (N + 2) * (N + 2);

int main() {
  auto window = sf::RenderWindow{{1920u, 1080u}, "CMake SFML Project"};
  window.setFramerateLimit(144);

  while (window.isOpen()) {
    for (auto event = sf::Event{}; window.pollEvent(event);) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear();
    window.display();
  }
}