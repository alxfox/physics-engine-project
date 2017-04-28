#ifndef GP_GUI_GRAPHICS_SHAPELIBRARY_H
#define GP_GUI_GRAPHICS_SHAPELIBRARY_H

#include <glad/glad.h>

namespace gp
{
namespace graphics
{
enum Shape {
  Cube = 0,
  Sphere,
  NUMBER_OF_SHAPES
};

class ShapeLibrary
{
private:
  enum BufferType {
    Position = 0,
    Normal,
    NUMBER_OF_BUFFER_TYPES
  };
  
  void setGeometry( Shape shape,
                    GLfloat*  vertex_positions,
                    size_t    vertex_positions_size,
                    GLfloat*  vertex_normals,
                    size_t    vertex_normals_size,
                    GLushort* triangles,
                    size_t    triangles_size );

  void refineIcosahedron( GLfloat*    icosahedron_vertex_positions,
                          GLushort    icosahedron_nvertices,
                          GLushort*   icosahedron_triangles,
                          GLushort    icosahedron_ntriangles,
                          GLfloat*&   sphere_vertex_positions,
                          GLushort&   sphere_nvertices,
                          GLushort*&  sphere_triangles,
                          GLushort&   sphere_ntriangles,
                          GLushort    nsubdivisions );
  
  GLuint m_vbo[NUMBER_OF_SHAPES][NUMBER_OF_BUFFER_TYPES];
  GLuint m_ebo[NUMBER_OF_SHAPES];
  GLuint m_vao[NUMBER_OF_SHAPES];
  GLuint m_nIndices[NUMBER_OF_SHAPES];

public:  
  ShapeLibrary();
  ~ShapeLibrary();
  
  void bind(Shape type);
  void draw(Shape type);
};

}
}

#endif // GP_GUI_GRAPHICS_SHAPELIBRARY_H
