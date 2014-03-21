#include "config.h"
#include "ui/GameWindow.h"

using namespace engine::ui;

int main(int, char*[]) {
  GameWindow::Ptr win = GameWindow::CreateWindowed(640, 480, "test");
  win->Loop();
  return 0;
}
