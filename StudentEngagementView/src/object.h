#ifndef STUDENT_ENGAGEMENT_SRC_OBJECT_H
#define STUDENT_ENGAGEMENT_SRC_OBJECT_H
#include "vector.h"
#include "renderable.h"
#include <string>

namespace ste {

class Object : public Renderable {
  Color _backgroundColor;
  Color _textColor;


public:
  Object* parent;


  Point2 position;
  Vector2 scale;

  Color backgroundColor;
  Color textColor;

  std::string text;
  float textScale;
  bool vertCenterText;
  bool centerText;


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

  Object() : position(0), scale(1), backgroundColor(1), textColor(0,0,0,1), text(""), textScale(1), vertCenterText(false), centerText(false), _backgroundColor(backgroundColor), _textColor(textColor) {}
};

}; // namespace ste


#endif