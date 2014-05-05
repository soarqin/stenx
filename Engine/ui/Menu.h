#ifndef _MENU_H_
#define _MENU_H_

#include "SFML/Graphics.hpp"
#include "SFGUI/SFGUI.hpp"

namespace engine {
namespace ui {

class Menu {
  friend class GameWindow;
public:
  typedef std::shared_ptr<Menu> Ptr;

  void AddItem(const std::wstring& name);
  void SetSpacing(float spacing);
  void MoveDelta(float x, float y);
  void MoveTo(float x, float y);
  void MoveCenter();

private:
  Menu();

private:
  sf::RenderWindow *parent;
  sfg::Window::Ptr win;
  sfg::Box::Ptr box;
};

}
}

#endif // _MENU_H_
