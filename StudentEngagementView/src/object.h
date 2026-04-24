#ifndef STUDENT_ENGAGEMENT_SRC_OBJECT_H
#define STUDENT_ENGAGEMENT_SRC_OBJECT_H
#include "vector.h"
#include "renderable.h"
#include <string>

namespace ste {

class Program;
class Object : public Renderable {
  Color _backgroundColor = 0;
  Color _textColor = 0;


public:
  Object* parent = nullptr;


  Point2 position = 0.0f;
  Vector2 scale = 1.0f;

  Color backgroundColor = Color(1);
  Color textColor = Color(0,0,0,1);

  std::string text = "";
  float textScale = 1.0f;
  bool vertCenterText = false;
  bool centerText = false;

  void (*onClick)(Program const* const prog) = nullptr;


  virtual void Hovering(Program const* const prog) {
    if (onClick) onClick(prog);
  }

  void Render(Program const* const prog) override;

  void RelativeTransform(std::shared_ptr<Object>& obj) const {
    obj->scale *= scale;
    obj->position += position;
    obj->position *= scale;
    if (parent != nullptr) {
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