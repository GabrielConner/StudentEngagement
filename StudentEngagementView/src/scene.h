#ifndef STUDENT_ENGAGEMENT_SRC_SCENE_H
#define STUDENT_ENGAGEMENT_SRC_SCENE_H

#include "renderable.h"
#include "object.h"
#include <vector>
#include <memory>

namespace ste {

class Scene : public Renderable {

std::vector<std::shared_ptr<Renderable>> renderList;

public:
  Color backgroundColor = 0;

  void (*start)(Program* const prog) = nullptr;



  void Render(Program const* const prog) override {
    for (auto& render : renderList)
      render->Render(prog);
  }

  Renderable* PositionIn(Vector2 pos) override {
    Renderable* ret = nullptr;
    for (auto& render : renderList) {
      ret = render->PositionIn(pos);
      if (ret)
        return ret;
    }
    return nullptr;
  }


  void AddRenderable(std::shared_ptr<Renderable> obj) {
    renderList.push_back(obj);
  }


  void AddObject(std::shared_ptr<Object> obj, std::shared_ptr<Object> parent) {
    if (!parent) {
      renderList.push_back(obj);
      return;
    }

    obj->parent = parent;
    parent->RelativeTransform(obj);
    renderList.push_back(obj);
    parent->children.push_back(obj);
  }

  Scene() = default;
};

}; // namespace ste


#endif