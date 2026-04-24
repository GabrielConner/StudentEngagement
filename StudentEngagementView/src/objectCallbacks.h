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

inline void Darken(Program* const prog, Object* obj, const MouseEvent& event) {
  obj->backgroundColor = Vector4(obj->backgroundColor.x * 0.8f, obj->backgroundColor.y * 0.8f, obj->backgroundColor.z * 0.8f, obj->backgroundColor.w);
}

inline void Lighten(Program* const prog, Object* obj, const MouseEvent& event) {
  obj->backgroundColor = Vector4(obj->backgroundColor.x * 1.2f, obj->backgroundColor.y * 1.2f, obj->backgroundColor.z * 1.2f, obj->backgroundColor.w);
}

inline void Reset(Program* const prog, Object* obj, const MouseEvent& event) {
  obj->Reset();
}


}; // namespace callbacks
}; // namespace ste


#endif