#ifndef GP_GUI_GRAPHICS_PROGRAMLIBRARY_H
#define GP_GUI_GRAPHICS_PROGRAMLIBRARY_H

#include "Program.h"
#include <unordered_map>

namespace gp
{
namespace graphics
{
enum NormalInterpolation {
  Flat = 0,
  Smooth,
  NUMBER_OF_INTERPOLATION_TYPES
};
class ProgramLibrary
{
private:
  std::unordered_map<size_t, Program> m_shading[NUMBER_OF_INTERPOLATION_TYPES];

  void createShadowProgram();
  void createLinesProgram();
  void createShadingProgram(Program& program, size_t interpolationType, size_t textures);

public:
  Program shadow;
  Program lines;
  
  Program* shading(enum NormalInterpolation interpolationType, size_t textures);

  ProgramLibrary();
};

}
}

#endif // GP_GUI_GRAPHICS_PROGRAMLIBRARY_H
