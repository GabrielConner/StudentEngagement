#ifndef STUDENT_ENGAGEMENT_SRC_OBJECT_H
#define STUDENT_ENGAGEMENT_SRC_OBJECT_H
#include "vector.h"
#include "renderable.h"
#include <string>

namespace ste {


struct MouseEvent {
  enum Type { ENTER, CLICK_PRESS, CLICK_RELEASE, LEAVE };

  Type type;
  Vector2 position;
};



class Program;
class Object : public Renderable {

  // Duplicates for stuff like changing color on hover
  Color _backgroundColor = 0;
  Color _textColor = 0;

public:
  std::string data = "";

  std::shared_ptr<Object> parent = nullptr;

  Point2 position = 0.0f;
  Vector2 scale = 1.0f;

  Color backgroundColor = Color(1);
  Color textColor = Color(0,0,0,1);


  std::string text = "";
  float textScale = 1.0f;
  bool vertCenterText = false;
  bool centerText = false;


  void (*onEnter)(Program* const prog, Object* obj, const MouseEvent& event) = nullptr;
  void (*onClickPress)(Program* const prog, Object* obj, const MouseEvent& event) = nullptr;
  void (*onClickRelease)(Program* const prog, Object* obj, const MouseEvent& event) = nullptr;
  void (*onLeave)(Program* const prog, Object* obj, const MouseEvent& event) = nullptr;



  void MouseEvent(Program* const prog, const MouseEvent& event) {
    switch (event.type) {
      case MouseEvent::ENTER:
        if (onEnter) onEnter(prog, this, event);
        break;
      case MouseEvent::CLICK_PRESS:
        if (onClickPress) onClickPress(prog, this, event);
        break;
      case MouseEvent::CLICK_RELEASE:
        if (onClickRelease) onClickRelease(prog, this, event);
        break;
      case MouseEvent::LEAVE:
        if (onLeave) onLeave(prog, this, event);
        break;
    }
  }

  void Render(Program const* const prog) override;

  void RelativeTransform(std::shared_ptr<Object>& obj) const {
    obj->scale *= scale;
    obj->position += position;
    obj->position *= scale;
    if (parent) {
      parent->RelativeTransform(obj);
    }
  }


  void SetCurrent() {
    _backgroundColor = backgroundColor;
    _textColor = textColor;
  }
  void Reset() {
    backgroundColor = _backgroundColor;
    textColor = _textColor;
  }

};

}; // namespace ste


#endif