#include "config.h"

#include "Menu.h"

#include "TGUI/TGUI.hpp"

namespace engine {
namespace ui {

Menu::Menu() {}

void Menu::AddItem(const std::wstring& name) {
    tgui::Button::Ptr button(*panel);
    button->load("../widgets/Black.conf");
    button->setSize(260, 60);
    button->setText(name);
//    button->bindCallback(tgui::Button::LeftMouseClicked);
//    button->setCallbackId(1);
}

void Menu::MoveTo(float x, float y) {
//     win->SetPosition(sf::Vector2f(x, y));
}

void Menu::MoveDelta(float x, float y) {
//     auto rc = win->GetAllocation();
//     win->SetPosition(sf::Vector2f(rc.left + x, rc.top + y));
}

void Menu::MoveCenter() {
//     auto rc = parent->getSize();
//     auto rc2 = win->GetAllocation();
//     rc2.left = (rc.x - rc2.width) / 2;
//     rc2.top = (rc.y - rc2.height) / 2;
//     win->SetAllocation(rc2);
}

}
}
