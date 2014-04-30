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

GameWindow::GameWindow(int w, int h, const std::string &n, bool f): window(sf::VideoMode(w, h), n, f ? sf::Style::Fullscreen : sf::Style::Default), sfgui(), desktop() {
}

GameWindow::~GameWindow() {
}

void GameWindow::Loop() {
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      desktop.HandleEvent(event);
      if (event.type == sf::Event::Closed)
        window.close();
    }

    desktop.Update(1.0f);

    window.clear();
    sfgui.Display(window);
    window.display();
  }
}

}
}
