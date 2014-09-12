#ifndef _GAMEWINDOW_H_
#define _GAMEWINDOW_H_

#include "SFML/Graphics.hpp"
#include "Menu.h"


namespace tgui {
class Gui;
}

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
    sf::RenderWindow window;
    tgui::Gui *gui;
};

}
}

#endif // _GAMEWINDOW_H_
