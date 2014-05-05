#include "config.h"
#include "ui/GameWindow.h"

using namespace engine::ui;

int main(int, char*[]) {
  GameWindow::Ptr win = GameWindow::CreateWindowed(640, 480, "test");
  auto menu = win->AddMenu();
  menu->SetSpacing(3);
  menu->AddItem(L"Test");
  menu->AddItem(L"Test2");
  menu->MoveCenter();
  menu->MoveDelta(0.f, -20.f);
  win->Loop();
  return 0;
}
