#ifndef STUDENT_ENGAGEMENT_SRC_OBJECT_CALLBACKS_H
#define STUDENT_ENGAGEMENT_SRC_OBJECT_CALLBACKS_H

#include "Program.h"
#include "vector.h"
#include "object.h"

namespace ste {
namespace callbacks {


inline void SwitchScene(Program* const prog, Object* obj, const MouseEvent& event) {
  prog->ChangeScene(obj->data);
}


inline void ButtonCycle(Program* const prog, Object* obj, const MouseEvent& event) {
  if (event.type == MouseEvent::ENTER)
    obj->backgroundColor = Vector4(obj->_backgroundColor.x * 0.8f, obj->_backgroundColor.y * 0.8f, obj->_backgroundColor.z * 0.8f, obj->_backgroundColor.w);
  else if (event.type == MouseEvent::CLICK_PRESS)
    obj->backgroundColor = Vector4(obj->_backgroundColor.x * 0.6f, obj->_backgroundColor.y * 0.6f, obj->_backgroundColor.z * 0.6f, obj->_backgroundColor.w);
  else if (event.type == MouseEvent::CLICK_RELEASE)
    obj->backgroundColor = Vector4(obj->_backgroundColor.x * 0.8f, obj->_backgroundColor.y * 0.8f, obj->_backgroundColor.z * 0.8f, obj->_backgroundColor.w);
  else {
    obj->backgroundColor = obj->_backgroundColor;
  }
}


inline void InputFieldEnter(Program* const prog, Object* obj, const MouseEvent& event) {
  prog->appendCharactersInto = &obj->text;
};

}; // namespace callbacks
}; // namespace ste


#endif