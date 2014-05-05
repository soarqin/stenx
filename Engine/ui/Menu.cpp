#include "config.h"

#include "Menu.h"

namespace engine {
namespace ui {

Menu::Menu() {
}

void Menu::AddItem(const std::wstring& name) {
  box->Pack(sfg::Button::Create(name));
}

void Menu::SetSpacing(float spacing) {
  box->SetSpacing(spacing);
}

void Menu::MoveTo(float x, float y) {
  win->SetPosition(sf::Vector2f(x, y));
}

void Menu::MoveDelta(float x, float y) {
  auto rc = win->GetAllocation();
  win->SetPosition(sf::Vector2f(rc.left + x, rc.top + y));
}

void Menu::MoveCenter() {
  auto rc = parent->getSize();
  auto rc2 = win->GetAllocation();
  rc2.left = (rc.x - rc2.width) / 2;
  rc2.top = (rc.y - rc2.height) / 2;
  win->SetAllocation(rc2);
}

}
}
