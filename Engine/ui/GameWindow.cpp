#include "config.h"

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#include "GameWindow.h"

namespace engine {
namespace ui {

GameWindow::Ptr GameWindow::CreateWindowed(int w, int h, const std::string &n) {
  return Ptr(new GameWindow(w, h, n, false));
}

GameWindow::Ptr GameWindow::CreateFullscreen(int w, int h, const std::string &n) {
  return Ptr(new GameWindow(w, h, n, true));
}

GameWindow::GameWindow(int w, int h, const std::string &n, bool f) {
  window = new sf::RenderWindow(sf::VideoMode(w, h), n, f ? sf::Style::Fullscreen : sf::Style::Default);
}

GameWindow::~GameWindow() {
  delete static_cast<sf::RenderWindow*>(window);
}

void GameWindow::Loop() {
  auto& win = *static_cast<sf::RenderWindow*>(window);
  while (win.isOpen())
  {
    sf::Event event;
    while (win.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        win.close();
    }

    win.clear();
    win.display();
  }
}

}
}
