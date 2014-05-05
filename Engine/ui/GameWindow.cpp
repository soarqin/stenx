#include "config.h"

#include "GameWindow.h"

namespace engine {
namespace ui {

GameWindow::Ptr GameWindow::CreateWindowed(int w, int h, const std::string &n) {
  return Ptr(new GameWindow(w, h, n, false));
}

GameWindow::Ptr GameWindow::CreateFullscreen(int w, int h, const std::string &n) {
  return Ptr(new GameWindow(w, h, n, true));
}

GameWindow::GameWindow(int w, int h, const std::string &n, bool f) : window(sf::VideoMode(w, h), n, f ? sf::Style::Fullscreen : sf::Style::Default) {
}

GameWindow::~GameWindow() {
}

void GameWindow::Loop() {
//  window.resetGLStates();
  sf::Texture background_texture;
  sf::Sprite background_sprite;
  std::size_t x = window.getSize().x;
  std::size_t y = window.getSize().y;
  std::size_t z = x * y;
  background_texture.create(x, y);

  std::vector<sf::Uint8> pixels(z * 4);

  sf::Uint8 pixel_value = 139;

  for (std::size_t index = 0; index < z; ++index) {
    pixel_value = static_cast<sf::Uint8>(pixel_value ^ (index + 809));
    pixel_value = static_cast<sf::Uint8>(pixel_value << (index % 11));
    pixel_value = static_cast<sf::Uint8>(pixel_value * 233);

    pixels[index * 4 + 0] = static_cast<sf::Uint8>(pixel_value % 16 + 72); // R

    pixel_value ^= static_cast<sf::Uint8>(index);
    pixel_value = static_cast<sf::Uint8>(pixel_value * 23);

    pixels[index * 4 + 1] = static_cast<sf::Uint8>(pixel_value % 16 + 72); // G

    pixel_value ^= static_cast<sf::Uint8>(index);
    pixel_value = static_cast<sf::Uint8>(pixel_value * 193);

    pixels[index * 4 + 2] = static_cast<sf::Uint8>(pixel_value % 16 + 72); // B

    pixels[index * 4 + 3] = 255; // A
  }

  background_texture.update(pixels.data());

  background_sprite.setTexture(background_texture);

  sf::Event event;
  sf::Clock clock;
  while (window.isOpen())
  {
    while (window.pollEvent(event))
    {
      desktop.HandleEvent(event);
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.draw(background_sprite);

    auto microseconds = clock.getElapsedTime().asMicroseconds();

    // Only update every 5ms
    if (microseconds > 5000) {
      desktop.Update(static_cast<float>(microseconds) / 1000000.f);
      clock.restart();
    }

    sfgui.Display(window);
    window.display();
  }
}

Menu::Ptr GameWindow::AddMenu() {
  auto *menu = new Menu;
  menu->parent = &window;
  menu->win = sfg::Window::Create(sfg::Window::BACKGROUND);
  menu->box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
  menu->win->Add(menu->box);
  desktop.Add(menu->win);
  return Menu::Ptr(menu);
}

}
}
