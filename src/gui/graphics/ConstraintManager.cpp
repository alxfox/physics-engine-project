#include "RenderComponentManager.h"
    
void gp::graphics::updateRenderOrder(uint64_t begin, uint64_t end, uint64_t data, uint64_t& renderOrder)
{
  uint64_t mask = ( ((1ul << (end-begin+1ul)) - 1ul) << begin );
  renderOrder = (renderOrder & ~mask) | ( (data << begin) & mask );
}

void gp::graphics::RenderComponentManager::computeMatrices(glm::mat4 const& view, glm::mat4 const& depthViewProjection)
{
  for (unsigned i = 0; i < data.size(); ++i) {
    glm::mat4 model = data.modelMatrix[i] * glm::scale(data.scale[i]);
    data.MV[i] = view * model;
    data.normalMatrix[i] = glm::mat3(glm::transpose(glm::inverse(data.MV[i])));
    data.depthMVP[i] = depthViewProjection * model;
  }
}

void gp::graphics::RenderComponentManager::sortForRender()
{
  // Use insertion sort as it performs well on already sorted lists
  for (size_t i = 1; i < data.size(); ++i) {
    for (size_t j = i; j > 0 && data.renderOrder[j] < data.renderOrder[j-1]; --j) {
      swap(j, j-1);
    }
  }
}
