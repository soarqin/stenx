#ifndef _MENU_H_
#define _MENU_H_

namespace engine {
namespace ui {

class Menu {
public:
  typedef std::shared_ptr<Menu> Ptr;
  void AddItem(const std::wstring& name);

private:
};

}
}

#endif // _MENU_H_
