#ifndef GP_GUI_GRAPHICS_CONSTRAINTMANAGER_H
#define GP_GUI_GRAPHICS_CONSTRAINTMANAGER_H

#include "common/Entity.h"
#include "common/SoA.h"
#include "common/SoAComponentManager.h"
#include "engine/utils.h"

namespace gp
{
  namespace graphics
  {
    struct Line {
      GLfloat start[3];
      GLfloat end[3];
    };
    struct ConstraintRenderComponent {
      Entity entity;
      Line line;
    };
  }
}
SOA_UPGRADE_STRUCT(gp::graphics::ConstraintRenderComponent, entity, line)

namespace gp
{
  namespace graphics
  {
    class ConstraintManager : public SoAComponentManager<ConstraintRenderComponent> {
    public:
      typedef SoAComponentManager<ConstraintRenderComponent>::EntityIterator EntityIterator;

      void setLine(Instance inst, engine::Vector3f const& start, engine::Vector3f const& end) {
        for (unsigned j = 0; j < 3; ++j) {
          data.line[inst].start[j] = start[j];
          data.line[inst].end[j] = end[j];
        }
      }
    };
  }
}

#endif // GP_GUI_GRAPHICS_CONSTRAINTMANAGER_H
