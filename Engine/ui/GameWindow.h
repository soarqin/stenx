#ifndef _GAMEWINDOW_H_
#define _GAMEWINDOW_H_

#include "SFML/Graphics.hpp"
#include "SFGUI/SFGUI.hpp"
#include "Menu.h"

namespace engine {
namespace ui {

class GameWindow {
public:
  typedef std::shared_ptr<GameWindow> Ptr;
  static Ptr CreateWindowed(int, int, const std::string &);
  static Ptr CreateFullscreen(int, int, const std::string &);

  virtual ~GameWindow();

  void Loop();

  Menu::Ptr AddMenu();

protected:
  GameWindow(int, int, const std::string &, bool);

protected:
  sfg::SFGUI sfgui;
  sfg::Desktop desktop;
  sf::RenderWindow window;
};

}
}

#endif // _GAMEWINDOW_H_
