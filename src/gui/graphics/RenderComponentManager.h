#ifndef GP_GUI_GRAPHICS_RENDERCOMPONENTMANAGER_H
#define GP_GUI_GRAPHICS_RENDERCOMPONENTMANAGER_H

#include <cstring>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "common/Entity.h"
#include "common/SoA.h"
#include "common/SoAComponentManager.h"
#include "ShapeLibrary.h"
#include "Material.h"

namespace gp
{
  namespace graphics
  {
    struct RenderComponent {
      Entity entity;
      uint64_t renderOrder;
      Shape shape;
      glm::mat4 modelMatrix;
      glm::vec3 scale;
      glm::mat3 normalMatrix;
      glm::mat4 MV;
      glm::mat4 depthMVP;
      Material* material;
    };

    /// Render order
    static uint64_t const ShapeEnd = 63;
    static uint64_t const ShapeBegin = 60;
    static uint64_t const ProgramEnd   = ShapeBegin - 1;
    static uint64_t const ProgramBegin = ProgramEnd + 1 - (static_cast<uint64_t>(1) << Material::NUMBER_OF_TEXTURE_TYPES);

    void updateRenderOrder(uint64_t begin, uint64_t end, uint64_t data, uint64_t& renderOrder);
  }
}
SOA_UPGRADE_STRUCT(gp::graphics::RenderComponent, entity, renderOrder, shape, modelMatrix, scale, normalMatrix, MV, depthMVP, material)

namespace gp
{
  namespace graphics
  {
    class RenderComponentManager : public SoAComponentManager<RenderComponent> {
    public:
      typedef SoAComponentManager<RenderComponent>::EntityIterator EntityIterator;

      void setShape(Instance inst, Shape shape) {
        data.shape[inst] = shape;
        updateRenderOrder(ShapeBegin, ShapeEnd, shape, data.renderOrder[inst]);
      }

      void setModelMatrix(Instance inst, glm::mat4 const& modelMatrix) {
        data.modelMatrix[inst] = modelMatrix;
      }

      void setScale(Instance inst, glm::vec3 const& scale) {
        data.scale[inst] = scale;
      }

      /// \todo Sorting will be wrong if someone changes the active textures after setting the material
      void setMaterial(Instance inst, Material* material) {
        data.material[inst] = material;
        updateRenderOrder(ProgramBegin, ProgramEnd, material->activeTextures(), data.renderOrder[inst]);
      }

      void computeMatrices(glm::mat4 const& view, glm::mat4 const& depthViewProjection);
      void sortForRender();
    };
  }
}

#endif // GP_GUI_GRAPHICS_RENDERCOMPONENTMANAGER_H
