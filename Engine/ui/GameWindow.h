#ifndef _GAMEWINDOW_H_
#define _GAMEWINDOW_H_

namespace engine {
namespace ui {

class GameWindow {
public:
  typedef std::shared_ptr<GameWindow> Ptr;
  static Ptr CreateWindowed(int, int, const std::string &);
  static Ptr CreateFullscreen(int, int, const std::string &);

  virtual ~GameWindow();

  void Loop();

protected:
  GameWindow(int, int, const std::string &, bool);

protected:
  void * window;
};

}
}

#endif // _GAMEWINDOW_H_
